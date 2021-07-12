#pragma once
#include <Eigen/Core>
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
         * @brief Propagate state transition 
         * 
         * x=Ax+Bu
         * P_bar=APA'+Q
         * 
         * @param linear_acceleration Acceleration in NED frame with gravtiy removed
         * @param dt time step
         */

        void predict(const Eigen::Vector3f& linear_acceleration,float dt);
        /**
         * @brief Add new gps measurement to position estimate
         * 
         * S=HP_barH'+R_gps
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
        void baroUpdate(); //not implemented yet

        /**
         * @brief update the reference gps coordinates (the launch site)
         * 
         * @param lat 
         * @param lng 
         * @param alt 
         */
        void updateGPSReference(const long lat, const long lng, const long alt);

        /**
         * @brief reset filter
         * 
         */
        void reset();
        /**
         * @brief Get the Position vector (Pn, Pe, Pd)
         * 
         * @return Eigen::Vector3f 
         */

        Eigen::Vector3f getPosition(){return X({STATE::Pn,STATE::Pe,STATE::Pd});};
        /**
         * @brief Get the Velocity vector (Vn, Ve, Vd)
         * 
         * @return Eigen::Vector3f 
         */
        Eigen::Vector3f getVelocity(){return X({STATE::Vn,STATE::Ve,STATE::Vd});};

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
        
        

        Eigen::Vector3f _gpsReferenceECEF = Eigen::Vector3f::Zero();

       





        Eigen::Vector3f GPStoECEF(const long lat, const long lng, const long alt);
        Eigen::Vector3f GPStoNED(const long lat, const long lng, const long alt);

};