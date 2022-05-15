#include "localizationkf.h"
#include "Storage/logController.h"

#include <Eigen/Core>
#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include <Eigen/Sparse>

#include <math.h>

LocalizationKF::LocalizationKF() : X(Eigen::Vector<float, 9>::Zero()),
                                   P(Eigen::Matrix<float, 9, 9>::Identity()){};

void LocalizationKF::reset()
{
    // this current intializaiton method means the filter will take time to converge to the solution
    X = Eigen::Vector<float, 9>::Zero();
    P = Eigen::Matrix<float, 9, 9>::Identity();
};

void LocalizationKF::predict(const float &dt)
{
    const float dt2 = dt * dt; // temporary for readability
    const float dt3 = dt2 * dt;

    const Eigen::Matrix<float, 9, 9> A{
        {1, dt, dt2 / 2, 0, 0, 0, 0, 0, 0},
        {0, 1, dt, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, dt, dt2 / 2, 0, 0, 0},
        {0, 0, 0, 0, 1, dt, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, dt, dt2 / 2},
        {0, 0, 0, 0, 0, 0, 0, 1, dt},
        {0, 0, 0, 0, 0, 0, 0, 0, 1}};

    // dt^4 terms are considered negligible so are zero
    const Eigen::Matrix<float, 9, 9> Q{
        {0, dt3 / 2, dt2 / 2, 0, 0, 0, 0, 0, 0},
        {dt3 / 2, dt2, dt, 0, 0, 0, 0, 0, 0},
        {dt2 / 2, dt, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, dt3 / 2, dt2 / 2, 0, 0, 0},
        {0, 0, 0, dt3 / 2, dt2, dt, 0, 0, 0},
        {0, 0, 0, dt2 / 2, dt, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, dt3 / 2, dt2 / 2},
        {0, 0, 0, 0, 0, 0, dt3 / 2, dt2, dt},
        {0, 0, 0, 0, 0, 0, dt2 / 2, dt, 1}};

    // update State
    X = (A * X);
    // update Covariance estimate
    P = A * P * A.transpose() + (Q * processVariance);
}

void LocalizationKF::gpsUpdate(const float &lat, const float &lng, const long &alt,
                               const long &vn, const long &ve, const long &vd)
{
    // measurement matrix
    Eigen::Vector3f positionNED = GPStoNED(lat, lng, alt);
    const Eigen::Vector<float, 6> z{{positionNED(0),
                                     ((float)vn) / 1000.0f, // conversion to m/s
                                     positionNED(1),
                                     ((float)ve) / 1000.0f,
                                     positionNED(2),
                                     ((float)vd) / 1000.0f}};
    kalmanUpdate<6>(z, R_GPS.toDenseMatrix(), H_GPS);
}

void LocalizationKF::baroUpdate(const float &altitude)
{
    kalmanUpdate<1>(Eigen::Matrix<float, 1, 1>{altitude}, Eigen::Matrix<float, 1, 1>{baroVariance}, H_BARO);
}

void LocalizationKF::accelUpdate(const Eigen::Ref<const Eigen::Vector3f>& accel)
{
    kalmanUpdate<3>(accel, R_ACCEL.toDenseMatrix(), H_ACCEL);
}

void LocalizationKF::HaccelUpdate(const Eigen::Ref<const Eigen::Vector3f>& accel)
{
    kalmanUpdate<3>(accel, R_HACCEL.toDenseMatrix(), H_ACCEL);
}

void LocalizationKF::updateGPSReference(const float &lat, const float &lng, const long &alt)
{
    _gpsReferenceECEF = GPStoECEF(lat, lng, alt);
    _gpsReferenceSLat = sin(lat * degtorad);
    _gpsReferenceCLat = cos(lat * degtorad);
    _gpsReferenceSLng = sin(lng * degtorad);
    _gpsReferenceCLng = cos(lng * degtorad);
}

Eigen::Vector3f LocalizationKF::GPStoECEF(const float &lat, const float &lng, const long &alt)
{
    const float slat = sin(lat * degtorad);
    const float clat = cos(lat * degtorad);
    const float slng = sin(lng * degtorad);
    const float clng = cos(lng * degtorad);

    const float ecefN = earthMajorAxis / sqrt(1 - (earthEccentricity2 * slat * slat));
    const float h = alt / 1000.0f; // conversion from mm to m

    float ecefX = (ecefN + h) * clat * clng;
    float ecefY = (ecefN + h) * clat * slng;
    float ecefZ = (((1 - earthEccentricity2) * ecefN) + h) * slat;

    return Eigen::Vector3f{ecefX, ecefY, ecefZ};
}

Eigen::Vector3f LocalizationKF::GPStoNED(const float &lat, const float &lng, const long &alt)
{
    Eigen::Matrix3f _tangentPlane{{-_gpsReferenceSLng, _gpsReferenceCLng, 0},
                                  {-_gpsReferenceSLat * _gpsReferenceCLng, -_gpsReferenceSLat * _gpsReferenceSLng, _gpsReferenceCLat},
                                  {_gpsReferenceCLat * _gpsReferenceCLng, _gpsReferenceCLat * _gpsReferenceSLng, _gpsReferenceSLat}};
    Eigen::Vector3f ENU = _tangentPlane * (GPStoECEF(lat, lng, alt) - _gpsReferenceECEF);
    Eigen::Vector3f NED({ENU(1), ENU(0), -ENU(2)});
    return NED;
}