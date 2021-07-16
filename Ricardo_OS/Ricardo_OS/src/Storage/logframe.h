#ifndef LOGFRAME_H
#define LOGFRAME_H
#include <string>
#include <array>
#include "serializableElement.h"
#include <Arduino.h>
#include <vector>

//logframes

/* this isnt used anywhere...
class logframe{
    virtual std::string stringify() const; //purley virtaul seralize method
};*/


class system_logframe{
private:  
    static constexpr auto getSerializer()
    {
        auto ret = serializer(
            &system_logframe::logLevel,
            &system_logframe::timestamp,
            &system_logframe::message,
            &system_logframe::systemFlag,
            &system_logframe::systemStatus
            
            

        );
        return ret;
    }

public:
    //global_SYSTEM_FLAG
    std::string logLevel;
    uint64_t timestamp;
    std::string message;
    uint32_t systemFlag;
    uint32_t systemStatus;
    
    std::string stringify()const{
        return getSerializer().stringify(*this) + "\n";
    };

};

class network_logframe{
private:
    static constexpr auto getSerializer()
    {
        auto ret = serializer(
            &network_logframe::test
        );
        return ret;
    }
public:
    // todo
    double test;
    
    std::string stringify()const{
        return getSerializer().stringify(*this) + "\n";
    };

};

class engine_logframe{
 // todo
};

class telemetry_logframe{
private:  
    static constexpr auto getSerializer()
    {
        auto ret = serializer(
            &telemetry_logframe::gps_long,
            &telemetry_logframe::gps_lat,
            &telemetry_logframe::gps_alt,
            &telemetry_logframe::gps_v_n,
            &telemetry_logframe::gps_v_e,
            &telemetry_logframe::gps_v_d,
            &telemetry_logframe::gps_sat,
            &telemetry_logframe::gps_fix,
            &telemetry_logframe::ax,
            &telemetry_logframe::ay,
            &telemetry_logframe::az,
            &telemetry_logframe::gx,
            &telemetry_logframe::gy,
            &telemetry_logframe::gz,
            &telemetry_logframe::mx,
            &telemetry_logframe::my,
            &telemetry_logframe::mz,
            &telemetry_logframe::imu_temp,
            &telemetry_logframe::baro_alt,
            &telemetry_logframe::baro_temp,
            &telemetry_logframe::baro_press,
            &telemetry_logframe::batt_volt,
            &telemetry_logframe::batt_percent,
            &telemetry_logframe::roll,
            &telemetry_logframe::pitch,
            &telemetry_logframe::yaw,
            &telemetry_logframe::pn,
            &telemetry_logframe::pe,
            &telemetry_logframe::pd,
            &telemetry_logframe::vn,
            &telemetry_logframe::ve,
            &telemetry_logframe::vd,
            &telemetry_logframe::an,
            &telemetry_logframe::ae,
            &telemetry_logframe::ad,
            &telemetry_logframe::timestamp
            
           

        );
        return ret;
    }

public:
    //gps
    long gps_long, gps_lat,gps_alt;
    long gps_v_n, gps_v_e, gps_v_d;
    uint8_t gps_sat, gps_fix;
    //imu
    float ax, ay, az;
    float gx, gy, gz;
    float mx, my, mz;
    uint16_t imu_temp;
    //baro
    float baro_alt,baro_temp,baro_press;
    //battery
    uint16_t batt_volt,batt_percent;
    //orientation
    float roll, pitch, yaw;
    //position
    float pn,pe,pd;
    //velocity
    float vn,ve,vd;
    //linear acceleration
    float an,ae,ad;
    
    uint64_t timestamp;

    std::string stringify()const{
        return getSerializer().stringify(*this) + "\n";
    };

    std::vector<uint8_t> serialize()const{
        return getSerializer().serialize(*this);
    };

};







#endif