#ifndef LOGFRAME_H
#define LOGFRAME_H
#include <string>
#include <array>

//logframes
class logframe{
    virtual std::string serialize(); //purley virtaul seralize method
};

class telemetry_logframe: public logframe{
private:  
    static constexpr auto getSerializer()
    {
        auto ret = serializer(
            &telemetry_logframe::ax,
            &telemetry_logframe::ay,
            &telemetry_logframe::az,
            &telemetry_logframe::gx,
            &telemetry_logframe::gy,
            &telemetry_logframe::gz,
            &telemetry_logframe::mx,
            &telemetry_logframe::my,
            &telemetry_logframe::mz,
            &telemetry_logframe::gps_lat,
            &telemetry_logframe::gps_long,
            &telemetry_logframe::gps_speed,
            &telemetry_logframe::gps_alt,
            &telemetry_logframe::baro_alt,
            &telemetry_logframe::baro_temp,
            &telemetry_logframe::baro_press,
            &telemetry_logframe::batt_volt,
            &telemetry_logframe::batt_percent
            &telemetry_logframe::rawGPSLong,
            &telemetry_logframe::rawGPSLong,
            &telemetry_logframe::rawGPSLat,
            &telemetry_logframe::rawGPSSpeed,
            &telemetry_logframe::rawGPSAlt,
            &telemetry_logframe::rawGPSsat,
            &telemetry_logframe::rawGPShdop,
            &telemetry_logframe::rawIMUAx,
            &telemetry_logframe::rawIMUAy,
            &telemetry_logframe::rawIMUAz,
            &telemetry_logframe::rawIMUGx,
            &telemetry_logframe::rawIMUGy,
            &telemetry_logframe::rawIMUGz,
            &telemetry_logframe::rawIMUMx,
            &telemetry_logframe::rawIMUMy,
            &telemetry_logframe::rawIMUMz,
            &telemetry_logframe::rawIMUTemp,
            &telemetry_logframe::rawBaroTemp,
            &telemetry_logframe::rawBaroPres,
            &telemetry_logframe::rawBaroAlt,
            &telemetry_logframe::battVolt,
            &telemetry_logframe::battPercent,
            &telemetry_logframe::rawTimestamp,
            &telemetry_logframe::estimatorX,
            &telemetry_logframe::estimatorY,
            &telemetry_logframe::estimatorZ,
            &telemetry_logframe::estimatorVX,
            &telemetry_logframe::estimatorVY,
            &telemetry_logframe::estimatorVZ,
            &telemetry_logframe::estimatorAX,
            &telemetry_logframe::estimatorAY,
            &telemetry_logframe::estimatorAZ,
            &telemetry_logframe::estimatorYaw,
            &telemetry_logframe::estimatorPitch,
            &telemetry_logframe::estimatorRoll,
            &telemetry_logframe::estimatorP,
            &telemetry_logframe::estimatorQ,
            &telemetry_logframe::estimatorR, 
            &telemetry_logframe::estimatorL,
            &telemetry_logframe::estimatorM,
            &telemetry_logframe::estimatorN,
            &telemetry_logframe::estimatorTimestamp
            )
        return ret;
    }
    
    static constexpr size_t packet_size() {
        return getSerializer().member_size();
    }

public:   
    double rawGPSLong; //gps.long
    double rawGPSLat; //gps.lat
    double rawGPSSpeed; //gps.speed
    double rawGPSAlt; //gps.alt
    uint8_t rawGPSsat; //gps.sat
    double rawGPShdop; //gps.hdop
    double rawIMUAx; //imu.ax
    double rawIMUAy; //imu.ay
    double rawIMUAz; //imu.az
    double rawIMUGx; //imu.gx
    double rawIMUGy; //imu.gy
    double rawIMUGz; //imu.gz
    double rawIMUMx; //imu.mx
    double rawIMUMy; //imu.my
    double rawIMUMz; //imu.mz
    double rawIMUTemp; //imu.temp
    double rawBaroTemp; //baro.temp
    double rawBaroPres; //baro.pres
    double rawBaroAlt; //baro.alt
    int battVolt; //batt.volt
    int battPercent; //batt.percent
    long rawTimestamp; //raw.timestamp
    double estimatorX; //estimator.x
    double estimatorY; //estimator.y
    double estimatorZ; //estimator.z
    double estimatorVX; //estimator.vx
    double estimatorVY; //estimator.vy
    double estimatorVZ; //estimator.vz
    double estimatorAX;//estimator.ax
    double estimatorAY; //estimator.ay
    double estimatorAZ; //estimator.az
    double estimatorYaw; //estimator.yaw
    double estimatorPitch; //estimator.pitch
    double estimatorRoll; //estimator.roll
    double estimatorP; //estimator.p
    double estimatorR; //estimator.r
    double estimatorQ; //estimator.q
    double estimatorL; //estimator.l
    double estimatorM; //estimator.m
    double estimatorN; //estimator.n
    long estimatorTimestamp; //estimator.timestamp


    std::string stringfy()const{
        return getSerializer().stringify();
    };
};
class system_logframe:public logframe{
private:  
    static constexpr auto getSerializer()
    {
        auto ret = serializer(
            &system_logframe::systemStatus,
            &system_logframe::systemFlag,
            &system_logframe::message
        
            )
        return ret;
    }
    
    static constexpr size_t packet_size() {
        return getSerializer().member_size();
    }
public:
    //global_system_flag
    //verbose_messa
    uint32_t systemStatus;
    uint32_t systemFlag;
    std::string message;
    
    std::string stringfy()const{
        return getSerializer().stringify();
    };

};
class network_logframe:public logframe{
    // Packet Header
    std::string seralize(){return "hi";};

};

class engine_logframe:public logframe{

};









#endif