#pragma once

#include <rnp_packet.h>
#include <rnp_serializer.h>

#include <vector>

enum class HITL_PACKET_TYPES:uint8_t{
    HITL_COMMAND = 0,//normal command packet
    PICKLE_RICK_SENSORS = 1 // contains raw data for all sensors on pickle rick
};

enum class HITL_COMMANDS:uint8_t{
    HITL_ENABLE,
    HITL_DISABLE
};


class PickleRickSensorsPacket : public RnpPacket{
    private:
    //serializer framework
        static constexpr auto getSerializer()
        {
            auto ret = RnpSerializer(
                &PickleRickSensorsPacket::ax, 
                &PickleRickSensorsPacket::ay,
                &PickleRickSensorsPacket::az,
                &PickleRickSensorsPacket::gx,
                &PickleRickSensorsPacket::gy,
                &PickleRickSensorsPacket::gz,
                &PickleRickSensorsPacket::h_ax,
                &PickleRickSensorsPacket::h_ay,
                &PickleRickSensorsPacket::h_az,
                &PickleRickSensorsPacket::mx,
                &PickleRickSensorsPacket::my,
                &PickleRickSensorsPacket::mz,
                &PickleRickSensorsPacket::baro_alt,
                &PickleRickSensorsPacket::baro_temp,
                &PickleRickSensorsPacket::baro_press,
                &PickleRickSensorsPacket::gps_lat,
                &PickleRickSensorsPacket::gps_lng,
                &PickleRickSensorsPacket::gps_alt,
                &PickleRickSensorsPacket::gps_v_n,
                &PickleRickSensorsPacket::gps_v_e,
                &PickleRickSensorsPacket::gps_v_d,
                &PickleRickSensorsPacket::gps_sat,                    
                &PickleRickSensorsPacket::gps_pdop,            
                &PickleRickSensorsPacket::gps_fix,  
                &PickleRickSensorsPacket::gps_updated, 
                &PickleRickSensorsPacket::gps_valid,
                &PickleRickSensorsPacket::imu_error,
                &PickleRickSensorsPacket::haccel_error,
                &PickleRickSensorsPacket::mag_error,
                &PickleRickSensorsPacket::baro_error,
                &PickleRickSensorsPacket::gps_error
                             
            );
            return ret;
        }
        
    public:
        ~PickleRickSensorsPacket();

        PickleRickSensorsPacket();
        /**
         * @brief Deserialize Hitl Packet
         * 
         * @param data 
         */
        PickleRickSensorsPacket(const RnpPacketSerialized& packet);

        /**
         * @brief Serialize Hitl Packet
         * 
         * @param buf 
         */
        void serialize(std::vector<uint8_t>& buf) override;


        float ax; 
        float ay;
        float az;
        float gx;
        float gy;
        float gz;
        
        float h_ax;
        float h_ay;
        float h_az;

        float mx;
        float my;
        float mz;

        float baro_alt;
        float baro_temp;
        float baro_press;

        float gps_lat;
        float gps_lng;
        long gps_alt;

        long gps_v_n;
        long gps_v_e;
        long gps_v_d;

        uint8_t gps_sat;                    
        uint16_t gps_pdop;              // poisitonal dillution of precision * 10^-2
        uint8_t gps_fix;  
        uint8_t gps_updated; //bools stored at uint8_t for ease 
        uint8_t gps_valid;

        uint8_t imu_error;
        uint8_t haccel_error;
        uint8_t mag_error;
        uint8_t baro_error;
        uint8_t gps_error;

        static constexpr size_t size(){
            return getSerializer().member_size();
        }

};

