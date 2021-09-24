#pragma once
#include "rnp_packet.h"
#include "rnp_serializer.h"

#include <vector>

class CommandPacket: public RnpPacket{
    private:
        static constexpr auto getSerializer()
        {
            auto ret = RnpSerializer(
                &CommandPacket::command,
                &CommandPacket::arg
            );
            return ret;
        }
    public:
        ~CommandPacket();
        CommandPacket(uint8_t command, uint32_t arg);

        /**
         * @brief Deserialize Command Packet from serialized data
         * 
         * @param packet 
         */
        CommandPacket(RnpPacketSerialized& packet);

        /**
         * @brief Serialize into provided buffer
         * 
         * @param buf 
         */
        void serialize(std::vector<uint8_t>& buf) override;

        //data members
        uint8_t command;
        uint32_t arg;


        static constexpr size_t size(){
            return getSerializer().member_size();
        }
};


class SimpleCommandPacket{};
class MagCalCommandPacket{};