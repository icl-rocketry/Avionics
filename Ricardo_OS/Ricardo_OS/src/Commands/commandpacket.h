#pragma once
#include "rnp_packet.h"
#include "rnp_serializer.h"



#include <Eigen/Core>

#include <vector>

using command_t = uint8_t; // define the type of command id type
namespace CommandPacket{
    command_t getCommand(const RnpPacketSerialized& packet); // extracts command id from a command serivce packet
    enum class TYPES:uint8_t{
        SIMPLE = 0,
        MAGCAL = 10,
        MESSAGE_RESPONSE = 100,
        TELEMETRY_RESPONSE = 101

    };
};


class SimpleCommandPacket: public RnpPacket{
    private:
        static constexpr auto getSerializer()
        {
            auto ret = RnpSerializer(
                &SimpleCommandPacket::command,
                &SimpleCommandPacket::arg
            );
            return ret;
        }
    public:
        ~SimpleCommandPacket();
        SimpleCommandPacket(uint8_t command, uint32_t arg);

        /**
         * @brief Deserialize Command Packet from serialized data
         * 
         * @param packet 
         */
        SimpleCommandPacket(const RnpPacketSerialized& packet);

        /**
         * @brief Serialize into provided buffer
         * 
         * @param buf 
         */
        void serialize(std::vector<uint8_t>& buf) override;

        //data members
        command_t command;

        uint32_t arg;


        static constexpr size_t size(){
            return getSerializer().member_size();
        }
};

class MagCalCommandPacket: public RnpPacket{
    private:
        static constexpr auto getSerializer()
        {
            auto ret = RnpSerializer(
                &MagCalCommandPacket::command,
                &MagCalCommandPacket::fieldMagnitude,
                &MagCalCommandPacket::declination,
                &MagCalCommandPacket::inclination,
                &MagCalCommandPacket::A11,
                &MagCalCommandPacket::A12,
                &MagCalCommandPacket::A13,
                &MagCalCommandPacket::A21,
                &MagCalCommandPacket::A22,
                &MagCalCommandPacket::A23,
                &MagCalCommandPacket::A31,
                &MagCalCommandPacket::A32,
                &MagCalCommandPacket::A33,
                &MagCalCommandPacket::b1,
                &MagCalCommandPacket::b2,
                &MagCalCommandPacket::b3
            );
            return ret;
        }
    public:
        ~MagCalCommandPacket();
        MagCalCommandPacket(uint8_t command);

        /**
         * @brief Deserialize Command Packet from serialized data
         * 
         * @param packet 
         */
        MagCalCommandPacket(const RnpPacketSerialized& packet);

        /**
         * @brief Serialize into provided buffer
         * 
         * @param buf 
         */
        void serialize(std::vector<uint8_t>& buf) override;

        /**
         * @brief get A matrix as a matrix object
         * 
         * @return Eigen::Matrix3f 
         */
        Eigen::Matrix3f getA();
        /**
         * @brief get B vecotr as a vector object
         * 
         * @return Eigen::Vector3f 
         */
        Eigen::Vector3f getB();

        //data members
        command_t command;

        float fieldMagnitude;
        float declination;
        float inclination;
        float A11;
        float A12;
        float A13;
        float A21;
        float A22;
        float A23;
        float A31;
        float A32;
        float A33;
        float b1;
        float b2;
        float b3;


        
        static constexpr size_t size(){
            return getSerializer().member_size();
        }
};


