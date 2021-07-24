#ifndef SENSOR_STRUCTS_H
#define SENSOR_STRUCTS_H
/*
definition of structs used within sensor classes
*/

#include <Eigen/Core>
#include <Eigen/Eigen>
#include <Eigen/Geometry>




struct raw_measurements_t{

    
    //IMU RAW DATA
    //right hand coordinate system, x points to ricardo logo side of board
    float ax,ay,az;
    float gx,gy,gz;
    float mx,my,mz;

    int16_t imu_temp;


    
    //GPS RAW DATA
    long gps_lat,gps_long,gps_alt; // gps poisition -> 10E7 to avoid floating math - alitude in mm
    long gps_v_n,gps_v_e,gps_v_d; // gps velocity mm/s
    int gps_sat; // gps satilites
    uint16_t gps_pdop; // poisitonal dillution of precision * 10^-2
    /* 0 = no fix
    1 = dead reckoning (requires external sensors)
    2 = 2D (not quite enough satellites in view)
    3 = 3D (the standard fix)
    4 = GNSS + dead reckoning (requires external sensors)
    5 = Time fix only*/
    uint8_t gps_fix; // gps fix type
    bool gps_updated; // flag if gps values have been updated
    bool gps_valid;
    uint16_t gps_year;
    uint8_t gps_month;
    uint8_t gps_day;
    uint8_t gps_hour;
    uint8_t gps_minute;
    uint8_t gps_second;

    //BARO RAW DATA
    float baro_alt,baro_temp,baro_press;

    //BATTERTY RAW DATA
    uint16_t batt_volt,batt_percent;

    //SYSTEM TIME
    uint64_t system_time;

};


struct state_t{

    Eigen::Quaternionf orientation; //(quaternion)
    Eigen::Vector3f eulerAngles;//(deg) (roll pitch yaw)
    Eigen::Vector3f position;//(m) relative to callibration site (NED)
    Eigen::Vector3f velocity;//(m/s) (NED)
    Eigen::Vector3f acceleration; //(g's) (NED)
    Eigen::Vector3f angularRates;//(deg/s)
    float thrust; //estimated thrust

    //Launch Site 
    long gps_launch_lat,gps_launch_long,gps_launch_alt;
    float baro_ref_alt;



    //0 nominal -> full navigation solution available
    //1 partial -> orientation solution available ONLY
    //2 no solution -> no solution available
    uint8_t estimator_state; 
};

#endif