#ifndef SENSOR_STRUCTS_H
#define SENSOR_STRUCTS_H
/*
definition of structs used within sensor classes
*/

#include <Eigen/Core>
#include <Eigen/Eigen>
#include <Eigen/Geometry>

namespace SensorStructs
{

    struct ACCELGYRO_6AXIS_t{
        float ax; 
        float ay;
        float az;
        float gx;
        float gy;
        float gz;

        float temp;
    };

    struct ACCEL_3AXIS_t{
        float ax;
        float ay;
        float az;
    };
    struct MAG_3AXIS_t{
        float mx;
        float my;
        float mz;

        float temp;
    };
    struct BARO_t{
         // BARO RAW DATA
        float alt;
        float temp;
        float press;
    };
    
    struct GPS_t{
        float lat;
        float lng;
        // gps position -> 10^-7 to avoid floating math - alitude in mm
        long alt;
        // gps velocity mm/s
        long v_n;
        long v_e;
        long v_d;

        uint8_t sat;                    // gps satilites
        uint16_t pdop;              // poisitonal dillution of precision * 10^-2
        /* 0 = no fix
        1 = dead reckoning (requires external sensors)
        2 = 2D (not quite enough satellites in view)
        3 = 3D (the standard fix)
        4 = GNSS + dead reckoning (requires external sensors)
        5 = Time fix only*/
        uint8_t fix;  // gps fix type
        bool updated; // flag if gps values have been updated
        bool valid;
        uint16_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
    };

    struct BATT_t{
        /**
         * @brief Battery types
         * 0 = Local Battery
         * 1 = PDU idk this needs to be put on the todo 
         * 
         * 
         */
        uint8_t type;
        uint16_t volt;
        uint16_t percent;
    };

    struct raw_measurements_t
    {
        ACCELGYRO_6AXIS_t accelgyro;
        ACCEL_3AXIS_t accel;
        MAG_3AXIS_t mag;
        BARO_t baro;
        GPS_t gps;
        BATT_t batt;

        uint64_t system_time;
    };

    struct state_t
    {

        Eigen::Quaternionf orientation; //(quaternion)
        Eigen::Vector3f eulerAngles;    //(deg) (roll pitch yaw)
        Eigen::Vector3f position;       //(m) relative to callibration site (NED)
        Eigen::Vector3f velocity;       //(m/s) (NED)
        Eigen::Vector3f acceleration;   //(g's) (NED)
        Eigen::Vector3f angularRates;   //(deg/s)

        // Launch Site
        float gps_launch_lat, gps_launch_long;
        long gps_launch_alt;
        float baro_ref_alt;

        //times -> all must be initialized to zero
        uint32_t ignitionTime{0};
        uint32_t launchTime{0};
        uint32_t apogeeTime{0};

        /**
         * @brief Estimator state
         * 
         * 
            NOMINAL = 0
            PARTIAL_NO_IMU = 1
            PARTIAL_NO_IMU_NO_GPS = 2
            PARTIAL_NO_IMU_NO_BARO = 3
            PARTIAL_NO_MAG = 4
            PARTIAL_NO_GPS = 5
            PARTIAL_NO_GPS_NO_BARO = 6
            PARTIAL_NO_BARO = 7
            NO_HOME = 8
            NOSOLUTION = 9
         * 
         */
        uint8_t estimator_state;
    };

}

#endif