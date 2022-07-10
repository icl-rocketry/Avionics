#pragma once
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include <Eigen/Sparse>

#include "Storage/logController.h"
#include "Storage/systemstatus.h"
#include "sensorStructs.h"

class LocalizationKF
{
public:
    LocalizationKF();

    /**
     * @brief Reset Navigation Solution
     *
     */
    void reset();

    /**
     * @brief Propagate state transition
     *
     * x=Ax+Bu
     * P_bar=APA'+Q
     *
     * @param dt time step
     */

    void predict(const float &dt);

    /**
     * @brief Add new gps measurement to position estimate
     *
     *
     *
     * @param lat GPS Latitude (deg), WGS84 geodetic coordinates
     * @param lng GPS Longitude (deg), WGS84 geodetic coordinates
     * @param alt GPS Alttude (mm)
     * @param vn NORTH velocity (m/s)
     * @param ve EAST velocity (m/s)
     * @param vd DOWN velocity (m/s)
     */
    void gpsUpdate(const float &lat, const float &lng, const long &alt,
                   const long &vn, const long &ve, const long &vd);

    /**
     * @brief Barometer update
     *
     * @param altitude in m
     */
    void baroUpdate(const float &altitude);

    /**
     * @brief Accelerometer update
     *
     * @param an m/s^2
     * @param ae m/s^2
     * @param ad m/s^2
     */
    void accelUpdate(const Eigen::Ref<const Eigen::Vector3f>& accel);

    /**
     * @brief High-G accelerometer update
     *
     * @param an m/s^2
     * @param ae m/s^2
     * @param ad m/s^2
     */
    void HaccelUpdate(const Eigen::Ref<const Eigen::Vector3f>& accel);

    /**
     * @brief update the reference gps coordinates (the launch site)
     *
     * @param lat
     * @param lng
     * @param alt
     */
    void updateGPSReference(const float &lat, const float &lng, const long &alt);

    /**
     * @brief Get the Position vector (Pn, Pe, Pd)
     *
     * @return Eigen::Vector3f
     */

    Eigen::Vector3f getPosition()
    {
        return X({STATE::Pn,
                  STATE::Pe,
                  STATE::Pd});
    };
    /**
     * @brief Get the Velocity vector (Vn, Ve, Vd)
     *
     * @return Eigen::Vector3f
     */
    Eigen::Vector3f getVelocity()
    {
        return X({STATE::Vn,
                  STATE::Ve,
                  STATE::Vd});
    };
    /**
     * @brief Get the Acceleration Vector (An, Ae, Ad)
     *
     * @return Eigen::Vector3f
     */
    Eigen::Vector3f getAcceleration() { return X({STATE::An,
                                                  STATE::Ae,
                                                  STATE::Ad}); };
    /**
     * @brief Get the relative NED coordinates from the reference point in m
     *
     * @param lat Latitude in degrees
     * @param lng Longitude in degrees
     * @param alt Altitude in mm
     * @return Eigen::Vector3f
     */
    Eigen::Vector3f GPStoNED(const float &lat, const float &lng, const long &alt);

private:
    static constexpr float g = 9.81;
    /**
     * @brief State variable [pn, vn, an, pe, ve, ae, pd, vd, ad]
     *
     */
    Eigen::Vector<float, 9> X;
    /**
     * @brief Enum containing indicies of state variables
     *
     */
    enum STATE : uint8_t
    {
        Pn,
        Vn,
        An,
        Pe,
        Ve,
        Ae,
        Pd,
        Vd,
        Ad
    };

    /**
     * @brief State Covariance Matrix
     *
     */
    Eigen::Matrix<float, 9, 9> P;

    static constexpr float processVariance = 0.1;

    // ACCELEROMETER MEASUREMENTS

    static constexpr float accelVariance = .05;
    const Eigen::DiagonalMatrix<float, 3> R_ACCEL{{accelVariance, accelVariance, accelVariance}};
    const Eigen::Matrix<float, 3, 9> H_ACCEL{
        {0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 1}};

    static constexpr float HaccelVariance = 1.5 * 9.81;
    const Eigen::DiagonalMatrix<float, 3> R_HACCEL{{HaccelVariance, HaccelVariance, HaccelVariance}};

    // BARO MEASUREMENT

    static constexpr float baroVariance = 0.5;
    const Eigen::Matrix<float, 1, 9> H_BARO{{0, 0, 0, 0, 0, 0, 1, 0, 0}};

    // GPS MEASUREMENT
    // cep reference - https://gssc.esa.int/navipedia/index.php/Accuracy

    /**
     * @brief Gps position variance given by the CEP ~ 2.5m
     *
     */
    static constexpr float gpsPositionVariance = 2.123;
    /**
     * @brief Gps Altitude variance differs from horizontal variance but for now assume its the same.
     * This is in m
     *
     */
    static constexpr float gpsAltitudeVariance = 2.123;
    /**
     * @brief Gps velocity variance given as +- 0.05ms-1 @ 50%
     *
     */
    static constexpr float gpsVelocityVariance = 0.00557;
    /**
     * @brief Covaraince Matrix for the GPS measurements.
     * We assume the variance is constant here where the position variance is given
     * by the CEP and and velocity by the velocity accuracy in the ublox specifications.
     * This is a bold assumption and will likely be changed later when we have more time.
     * Further we assume no covaraince between the measurement as we cannot estimate this.
     *
     *
     */
    const Eigen::DiagonalMatrix<float, 6> R_GPS{{gpsPositionVariance,
                                                 gpsVelocityVariance,
                                                 gpsPositionVariance,
                                                 gpsVelocityVariance,
                                                 gpsAltitudeVariance,
                                                 gpsVelocityVariance}};

    /**
     * @brief Measurement Function matrix
     *
     */
    const Eigen::Matrix<float, 6, 9> H_GPS{
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 0}};

    Eigen::Vector3f _gpsReferenceECEF = Eigen::Vector3f::Zero();
    // trignometric values of lat and lng at reference coordinates
    float _gpsReferenceSLat;
    float _gpsReferenceCLat;
    float _gpsReferenceSLng;
    float _gpsReferenceCLng;
    // earth constants from wgs84 model
    static constexpr float earthMajorAxis = 6378137;              // equitorial radius (m)
    static constexpr float earthEccentricity2 = 0.00669437999014; // eccentricity squared
    // conversion constant
    static constexpr float degtorad = 0.01745329251;

    Eigen::Vector3f GPStoECEF(const float &lat, const float &lng, const long &alt);

    /**
     * @brief Templated kalmanUpdate to handle different sizes of updates
     *
     * @tparam Z_t Sensor Vector Type
     * @tparam R_t Measurement Noise Matrix Type
     * @tparam H_t Measurement Function Type
     * @param z Sensor Vector
     * @param R Measurement Noise Matrix
     * @param H Measurement Function
     *
     * System Uncertainty:
     * S=HP_barH'+R
     *
     * Kalman Gain:
     * K=P_barH'S^-1
     *
     * residual
     * y=z−HX
     *
     * state update
     * x = x + Ky
     *
     * Covariance update
     * P = (I-KH)P_bar -> unstable numerically
     * Alternate Form
     * P = (I−KH)P(I−KH)'+KRK'
     *
     */
    template <size_t Z_size, typename DerivedZ, typename DerivedR, typename DerivedH>
    void kalmanUpdate(const Eigen::MatrixBase<DerivedZ> &z, const Eigen::MatrixBase<DerivedR> &R, const Eigen::MatrixBase<DerivedH> &H)
    {
        // System Uncertainty
        Eigen::Matrix<float, Z_size, Z_size> S = (H * P * H.transpose()) + R;
        // Kalman Gain
        Eigen::Matrix<float, 9, Z_size> K = (P * H.transpose()) * S.inverse();
        // Residual
        Eigen::Vector<float, Z_size> y = z - (H * X);
        // state update
        X = X + (K * y);
        // Covariance update
        Eigen::Matrix<float, 9, 9> I_KH_temp = (Eigen::Matrix<float, 9, 9>::Identity() - (K * H));
        P = (I_KH_temp * P * I_KH_temp.transpose()) + (K * R * K.transpose());
    };
};
