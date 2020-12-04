#ifndef SENSOR_STRUCTS_H
#define SENSOR_STRUCTS_H
/*
definition of structs used within sensor classes
*/


struct raw_measurements_t{

    //IMU RAW DATA
    float ax,ay,az;
    float gx,gy,gz;
    float mx,my,mz;

    int16_t imu_temp;

    
    //GPS RAW DATA
    float gps_lat,gps_long,gps_speed,gps_alt,gps_course,gps_hdop,gps_sat;

    //BARO RAW DATA
    float baro_alt,baro_temp,baro_press;

    //BATTERTY RAW DATA
    int batt_volt,batt_percent;

    //SYSTEM TIME
    uint32_t system_time;

};

struct state_t{
    float lat,lng;
    float q[4]; //quaternion output
    float ypr[3]; //yaw pitch roll
    float x,y,z; //world coordinates(m) relative to launch position
    float vx,vy,vz; //rocket frame of reference velocity m/s
    float ax,ay,az; //rocket frame of reference aceleration m/s^2
    uint8_t estimator_state; 
};

#endif