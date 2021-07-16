#pragma once
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include <Eigen/Sparse>

#include "Storage/logController.h"
#include "Storage/systemstatus.h"
#include "sensorStructs.h"



class LocalizationKF{
    public:
        LocalizationKF(LogController& logcontroller);
                
        /**
         * @brief Initialize Filter 
         * 
         */
        void setup();

        /**
         * @brief Propagate state transition 
         * 
         * x=Ax+Bu
         * P_bar=APA'+Q
         * 
         * @param linear_acceleration Acceleration (ms-2) in NED frame with gravtiy removed
         * @param dt time step
         */
        void predict(const Eigen::Vector3f& linear_acceleration,float dt);
        /**
         * @brief Add new gps measurement to position estimate
         * 
         *
         * System Uncertainty:
         * S=HP_barH'+R_gps
         * as H is the identiy matrix:
         * S=P_bar+R_gps
         * 
         * Kalman Gain:
         * K=P_barH'S^-1
         * Again as H is the identiy matrix:
         * K=P_barS^-1
         * 
         * 
         * K=P_bar(P_bar+R_gps)^-1
         * 
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
         * Noting that H_gps is the identity matrix we get
         * P = (I-K)P(I-K)'+KRK'
         * 
         * 
         * @param lat GPS Latitude (deg), WGS84 geodetic coordinates
         * @param lng GPS Longitude (deg), WGS84 geodetic coordinates
         * @param alt GPS Alttude (mm)
         * @param vn NORTH velocity (m/s)
         * @param ve EAST velocity (m/s)
         * @param vd DOWN velocity (m/s)
         */
        void gpsUpdate(const long lat, const long lng, const long alt,
                            const long vn, const long ve, const long vd);
        /**
         * @brief Add new baro measurement to position estimate
         * 
         */
        void baroUpdate(){}; //not implemented yet

        /**
         * @brief update the reference gps coordinates (the launch site)
         * 
         * @param lat 
         * @param lng 
         * @param alt 
         */
        void updateGPSReference(const long lat, const long lng, const long alt);

        /**
         * @brief Get the Position vector (Pn, Pe, Pd)
         * 
         * @return Eigen::Vector3f 
         */

        Eigen::Vector3f getPosition(){return X({(uint8_t)STATE::Pn,
                                                (uint8_t)STATE::Pe,
                                                (uint8_t)STATE::Pd});
                                                };
        /**
         * @brief Get the Velocity vector (Vn, Ve, Vd)
         * 
         * @return Eigen::Vector3f 
         */
        Eigen::Vector3f getVelocity(){return X({(uint8_t)STATE::Vn,
                                                (uint8_t)STATE::Ve,
                                                (uint8_t)STATE::Vd});
                                                };

        /**
         * @brief Get the relative NED coordinates from the reference point in m
         * 
         * @param lat Latitude in degrees
         * @param lng Longitude in degrees
         * @param alt Altitude in mm
         * @return Eigen::Vector3f 
         */
        Eigen::Vector3f GPStoNED(const long lat, const long lng, const long alt);

    private:
        LogController& _logcontroller;

        /**
         * @brief State variable [pn, vn, pe, ve, pd, vd]
         * 
         */
        Eigen::Vector<float,6> X = Eigen::Vector<float,6>::Zero();
        /**
         * @brief Enum containing indicies of state variables
         * 
         */
        enum class STATE:uint8_t{
            Pn,
            Vn,
            Pe,
            Ve,
            Pd,
            Vd
        };

        /**
         * @brief State Covariance Matrix
         * 
         */
        Eigen::Matrix<float,6,6> P;

        const float accelVariance = 0;

        //GPS MEASUREMENT

        /**
         * @brief Gps position variance given by the CEP ~ 2.5m
         * 
         */
        const float gpsPositionVariance = 0;
        /**
         * @brief Gps Altitude variance differs from horizontal variance.
         * 
         */
        const float gpsAltitudeVariance = 0;
        /**
         * @brief Gps velocity variance given as +- 0.05ms-1
         * 
         */
        const float gpsVelocityVariance = 0;
        /**
         * @brief Covaraince Matrix for the GPS measurements.
         * We assume the variance is constant here where the position variance is given
         * by the CEP and and velocity by the velocity accuracy in the ublox specifications.
         * This is a bold assumption and will likely be changed later when we have more time.
         * Further we assume no covaraince between the measurement as we cannot estimate this.
         * 
         * 
         */
        const Eigen::DiagonalMatrix<float,6> R_GPS{{gpsPositionVariance,
                                                    gpsVelocityVariance,
                                                    gpsPositionVariance,
                                                    gpsVelocityVariance,
                                                    gpsAltitudeVariance,
                                                    gpsVelocityVariance}};


        const float baroVariance = 0;
        
        
        

        Eigen::Vector3f _gpsReferenceECEF = Eigen::Vector3f::Zero();
        // trignometric values of lat and lng at reference coordinates
        float _gpsReferenceSLat;
        float _gpsReferenceCLat;
        float _gpsReferenceSLng;
        float _gpsReferenceCLng;
        //earth constants from wgs84 model
        const float earthMajorAxis = 6378137; // equitorial radius (m)
        const float earthEccentricity2 = 0.00669437999014; // eccentricity squared
        //conversion constant 
        const float degtorad = 0.01745329251;

        Eigen::Vector3f GPStoECEF(const long lat, const long lng, const long alt);
        
        


        

};