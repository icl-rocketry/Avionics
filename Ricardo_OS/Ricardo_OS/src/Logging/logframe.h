#ifndef LOGFRAME_H
#define LOGFRAME_H
#include <string>
#include <array>

//logframes
class logframe{
    //serialize method

    //virtual std::string serialize(); //purley virtaul seralize method


};

class telemetry_logframe:public logframe{

    //gps.long
    //gps.lat
    //gps.speed
    //gps.alt
    //gps.sat
    //gps.hdop
    //imu.ax
    //imu.ay
    //imu.az
    //imu.gx
    //imu.gy
    //imu.gz
    //imu.mx
    //imu.my
    //imu.mz
    //imu.temp
    //baro.temp
    //baro.pres
    //baro.alt
    //batt.volt
    //batt.percent
    //raw.timestamp
    //estimator.x
    //estimator.y
    //estimator.z
    //estimator.vx
    //estimator.vy
    //estimator.vz
    //estimator.ax
    //estimator.ay
    //estimator.az
    //estimator.yaw
    //estimator.pitch
    //estimator.roll
    //estimator.p
    //estimator.r
    //estimator.q
    //estimator.l
    //estimator.m
    //estimator.n
    //estimator.timestamp

    
    // I want to die thinking about writing this
    
    std::string seralize(){return "hi";};
};
class system_logframe:public logframe{
    //global_system_flag
    //verbose_messa

    std::string seralize(){return "hi";};

};
class network_logframe:public logframe{
    // Packet Header
    std::string seralize(){return "hi";};

};









#endif