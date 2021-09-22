#pragma once

#include <utility>
#include <tuple>

#include <Eigen/Core>
#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include <Eigen/Sparse>

//mag callibration class

// https://teslabs.com/articles/magnetometer-calibration/



struct MagCalibrationParameters{
    Eigen::Matrix3f A_1;
    Eigen::Vector3f b;
    uint8_t error;
};


class MagCalibration{ // all mag units in Gauss
    public:
        /**
         * @brief Construct a new Mag Cal object
         * 
         * @param fieldMagnitude in gauss default is set to 1 as correct magnitude is not necessary for orientation estimation
         * @param inclination in deg?
         * @param declination in deg?
         */
        MagCalibration(float fieldMagnitude = 1, float inclination = 0, float declination = 0);

        enum class STATUS:uint8_t{
            OK=0,
            ERR_VECT_SIZE_MISMATCH=1,
            ERR_EIG=2 // could not compute eigen vectors
        };
        /**
         * @brief calculate calibration coefficents
         * 
         */
        MagCalibrationParameters calibrate(std::vector<float>& mx,std::vector<float>& my,std::vector<float>& mz);

    public:
        struct ellipsoidParameters{
            Eigen::Matrix3f M;
            Eigen::Vector3f n;
            float d;
            bool error;
        };

        // https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=1290055 [Q. Li et al]
        // input -> array (M,N) where M = 3 (x,y,z) and N is number of samples
        ellipsoidParameters ellipsoidFit(Eigen::Matrix<float,3,Eigen::Dynamic>& magData);

        // get these values from the wmm model 
        float _fieldMagnitude;
        float _inclination;
        float _declination;


};