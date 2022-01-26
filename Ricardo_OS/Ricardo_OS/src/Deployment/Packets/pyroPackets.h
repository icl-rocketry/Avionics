#pragma once

#include "rnp_packet.h"
#include "rnp_serializer.h"

#include <vector>

namespace PyroPacket{
    uint8_t getPyroID(const RnpPacketSerialized& packet); // extracts pyro id from a pyro packet
    enum class TYPES:uint8_t{
        PYROINFOPACKET = 0
    };
};

class PyroInfoPacket : public RnpPacket{
    private:
    //serializer framework
        static constexpr auto getSerializer()
        {
            auto ret = RnpSerializer(
                &PyroInfoPacket::pyroID,
                &PyroInfoPacket::continuity
            );
            return ret;
        }
        
    public:
        ~PyroInfoPacket();

        PyroInfoPacket();
        /**
         * @brief Deserialize PyroInfo Packet
         * 
         * @param data 
         */
        PyroInfoPacket(const RnpPacketSerialized& packet);

        /**
         * @brief Serialize PyroInfo Packet
         * 
         * @param buf 
         */
        void serialize(std::vector<uint8_t>& buf) override;

        uint8_t pyroID;
        bool continuity;

        static constexpr size_t size(){
            return getSerializer().member_size();
        }

};

