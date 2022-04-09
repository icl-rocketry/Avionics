#pragma once

#include "rnp_packet.h"
#include "rnp_serializer.h"

#include <vector>

class TelemetryPacket : public RnpPacket{
    private:
    //serializer framework
        static constexpr auto getSerializer()
        {
            auto ret = RnpSerializer(
                &TelemetryPacket::pn,
                &TelemetryPacket::pe,
                &TelemetryPacket::pd,
                &TelemetryPacket::vn,
                &TelemetryPacket::ve,
                &TelemetryPacket::vd,
                &TelemetryPacket::an,
                &TelemetryPacket::ae,
                &TelemetryPacket::ad,
                &TelemetryPacket::roll,
                &TelemetryPacket::pitch,
                &TelemetryPacket::yaw,
                &TelemetryPacket::q0,
                &TelemetryPacket::q1,
                &TelemetryPacket::q2,
                &TelemetryPacket::q3,
                &TelemetryPacket::lat,
                &TelemetryPacket::lng,
                &TelemetryPacket::alt,
                &TelemetryPacket::sat,
                &TelemetryPacket::ax,
                &TelemetryPacket::ay,
                &TelemetryPacket::az,
                &TelemetryPacket::h_ax,
                &TelemetryPacket::h_ay,
                &TelemetryPacket::h_az,
                &TelemetryPacket::gx,
                &TelemetryPacket::gy,
                &TelemetryPacket::gz,
                &TelemetryPacket::mx,
                &TelemetryPacket::my,
                &TelemetryPacket::mz,
                &TelemetryPacket::baro_temp,
                &TelemetryPacket::baro_press,
                &TelemetryPacket::baro_alt,
                &TelemetryPacket::batt_voltage,
                &TelemetryPacket::batt_percent,
                &TelemetryPacket::launch_lat,
                &TelemetryPacket::launch_lng,
                &TelemetryPacket::launch_alt,
                &TelemetryPacket::system_status,
                &TelemetryPacket::system_time,
                &TelemetryPacket::rssi,
                &TelemetryPacket::snr
                

               
            );
            return ret;
        }
        
    public:
        ~TelemetryPacket();

        TelemetryPacket();
        /**
         * @brief Deserialize Telemetry Packet
         * 
         * @param data 
         */
        TelemetryPacket(const RnpPacketSerialized& packet);

        /**
         * @brief Serialize Telemetry Packet
         * 
         * @param buf 
         */
        void serialize(std::vector<uint8_t>& buf) override;

        
        //packet header
        //PacketHeader header{static_cast<uint8_t>(packet::TELEMETRY), packet_size()};
        //estimator output
        float pn, pe, pd; // position NED (m)
        float vn, ve, vd; // velocity NED (m/s)
        float an, ae, ad; // acceleration NED (g's)
        //orientation
        float roll,pitch,yaw; // orientation degrees
        float q0,q1,q2,q3; //quaternion representation
        //gps
        float lat,lng;
        long alt;//in mm
        uint8_t sat;
        //imu
        float ax, ay, az; // acceleration (g's)
        float h_ax,h_ay,h_az;// high g accel (g's)
        float gx, gy, gz; // angular rates (deg/s)
        float mx, my, mz;// magnetometer (uT)
        //barometer
        float baro_temp, baro_press,baro_alt;
        //battery
        uint16_t batt_voltage,batt_percent;
        //launch site
        float launch_lat,launch_lng;
        long launch_alt;
        //system details
        uint32_t system_status;
        uint64_t system_time;
        //radio details
        int16_t rssi; 
        float snr;


        static constexpr size_t size(){
            return getSerializer().member_size();
        }

};


