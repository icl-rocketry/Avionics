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
    //global_system_flag
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
            &telemetry_logframe::rawGPSLong,
            &telemetry_logframe::rawGPSLat,
            &telemetry_logframe::rawGPSsat,
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
            &telemetry_logframe::rawTimestamp
            
            

        );
        return ret;
    }

public:
    //global_system_flag
    long rawGPSLong;
    long rawGPSLat;
    int rawGPSsat;
    float rawIMUAx;
    float rawIMUAy;
    float rawIMUAz;
    float rawIMUGx;
    float rawIMUGy;
    float rawIMUGz;
    float rawIMUMx;
    float rawIMUMy;
    float rawIMUMz;
    uint16_t rawIMUTemp;
    uint64_t rawTimestamp;

    std::string stringify()const{
        return getSerializer().stringify(*this) + "\n";
    };

    std::vector<uint8_t> serialize()const{
        return getSerializer().serialize(*this);
    };

};







#endif