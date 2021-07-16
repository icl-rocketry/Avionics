#include "localizationkf.h"
#include "Storage/logController.h"

#include <Eigen/Core>
#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include <Eigen/Sparse>

#include <math.h>

LocalizationKF::LocalizationKF(LogController& logcontroller):
_logcontroller(logcontroller)
{};

void LocalizationKF::setup(){
    //this current intializaiton method means the filter will take time to converge to the solution
    X = Eigen::Vector<float,6>::Zero();
    P = Eigen::Matrix<float,6,6>::Identity(); // initialize as identity matrix for ease -> can update this later
};

void LocalizationKF::predict(const Eigen::Vector3f& linear_acceleration,float dt){
    const Eigen::Matrix<float,6,6> A{{1, dt, 0, 0, 0, 0},
                                     {0, 1, 0, 0, 0, 0},
                                     {0, 0, 1, dt, 0, 0},
                                     {0, 0, 0, 1, 0, 0},
                                     {0, 0, 0, 0, 1, dt},
                                     {0, 0, 0, 0, 0, 1}}; // construct state transition sub matrix

    const float dt2 = dt*dt;//temporary for readability
    const float dt3 = dt2*dt;
    const Eigen::Matrix<float,6,3> B{{dt2/2, 0, 0},
                                     {dt, 0, 0},
                                     {0, dt2/2, 0},
                                     {0, dt, 0},
                                     {0, 0, dt2/2},
                                     {0, 0,dt}}; // construct control sub matrix

    //dt^4 terms are considered negligible so have been estimated as zero
    const Eigen::Matrix<float,6,6> Q = accelVariance * Eigen::Matrix<float,6,6> {{0, dt3/2, 0, 0, 0, 0},
                                                                                 {dt3/2, dt2, 0, 0, 0, 0},
                                                                                 {0, 0, 0, dt3/2, 0, 0},
                                                                                 {0, 0, dt3/2, dt2, 0, 0},
                                                                                 {0, 0, 0, 0, 0, dt3/2},
                                                                                 {0, 0, 0, 0, dt3/2, dt2}}; // construct state transition sub matrix


    //update State 
    X = (A*X) + (B*linear_acceleration);
    //update Covariance estimate
    P = A*P*A.transpose() + Q; 
}

void LocalizationKF::gpsUpdate(const long lat, const long lng, const long alt,const long vn, const long ve, const long vd){
    //measurement matrix
    Eigen::Vector3f positionNED = GPStoNED(lat,lng,alt); 
    const Eigen::Vector<float,6> z{{positionNED(0),
                                    ((float)vn)/1000.0f, //conversion to m/s
                                    positionNED(1),
                                    ((float)ve)/1000.0f,
                                    positionNED(2),
                                    ((float)vd)/1000.0f}};

    //System Uncertainty
    Eigen::Matrix<float,6,6> S_GPS = P+R_GPS.toDenseMatrix();
    //Kalman Gain
    Eigen::Matrix<float,6,6> K_GPS = P*(S_GPS).inverse();
    //Reisdual
    Eigen::Vector<float,6> Y_GPS = z-X;
    //State update
    X = X + (K_GPS*Y_GPS);
    //Covariance update
    P = ( (Eigen::Matrix<float,6,6>::Identity() - K_GPS) * P * ((Eigen::Matrix<float,6,6>::Identity() - K_GPS).transpose()) ) + (K_GPS*R_GPS*K_GPS.transpose());
        
}

void LocalizationKF::updateGPSReference(const long lat, const long lng, const long alt){
    _gpsReferenceECEF = GPStoECEF(lat,lng,alt);
    _gpsReferenceSLat = sin(lat * degtorad);
    _gpsReferenceCLat = cos(lat * degtorad);
    _gpsReferenceSLng = sin(lng * degtorad);
    _gpsReferenceCLng = cos(lng * degtorad);
}

Eigen::Vector3f LocalizationKF::GPStoECEF(const long lat, const long lng,const long alt){
    const float slat = sin(lat * degtorad);
    const float clat = cos(lat * degtorad);
    const float slng = sin(lng * degtorad);
    const float clng = cos(lng * degtorad);

    const float ecefN = earthMajorAxis/sqrt(1-(earthEccentricity2*slat*slat));
    const float h = alt/1000.0f; //conversion from mm to m

    float ecefX = (ecefN + h)*clat*clng;
    float ecefY = (ecefN + h)*clat*slng;
    float ecefZ = (((1-earthEccentricity2)*ecefN) + h)*slat; 

    return Eigen::Vector3f{ecefX,ecefY,ecefZ};
}

Eigen::Vector3f LocalizationKF::GPStoNED(const long lat, const long lng, const long alt){
    Eigen::Matrix3f  _tangentPlane{{-_gpsReferenceSLng, _gpsReferenceCLng, 0},
                                   {-_gpsReferenceSLat*_gpsReferenceCLng, -_gpsReferenceSLat*_gpsReferenceSLng, _gpsReferenceCLat},
                                   {_gpsReferenceCLat*_gpsReferenceCLng, _gpsReferenceCLat*_gpsReferenceSLng, _gpsReferenceSLat}};
    Eigen::Vector3f ENU = _tangentPlane * (GPStoECEF(lat,lng,alt) - _gpsReferenceECEF);
    Eigen::Vector3f NED({ENU(1),ENU(0),-ENU(2)});
    return NED;
}