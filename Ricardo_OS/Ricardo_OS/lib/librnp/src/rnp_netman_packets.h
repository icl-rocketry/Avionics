#pragma once 

#include "rnp_packet.h"
#include "rnp_header.h"
#include "rnp_serializer.h"
#include "rnp_routingtable.h"

#include <array>




enum class NETMAN_TYPES:uint8_t{
    PING_REQ = 1,
    PING_RES = 2,
    SET_ADDRESS = 3,
    SET_ROUTE = 4,
    SAVE_CONF = 5, // this may benefit from a securtiy key
    RESET_NETMAN = 6 // ^^

};

class PingPacket: public RnpPacket{
    public:
        ~PingPacket();

        /**
         * @brief Construct a new Ping Packet object
         * 
         * @param sys_time 
         */
        PingPacket(uint32_t sys_time);

        /**
         * @brief Deserialize Ping Packet
         * 
         * @param packet 
         */
        PingPacket(RnpPacketSerialized& packet);

        /**
         * @brief Serialize into buf
         * 
         * @param buf 
         */
        void serialize(std::vector<uint8_t>& buf) override;

        //data members
        uint32_t sys_time;

        static constexpr size_t size(){return sizeof(sys_time);};
};

class SetAddressPacket: public RnpPacket{
    public:
        ~SetAddressPacket();

        /**
         * @brief Construct a new Ping Packet object
         * 
         * @param sys_time 
         */
        SetAddressPacket(uint8_t address);

        /**
         * @brief Deserialize Ping Packet
         * 
         * @param packet 
         */
        SetAddressPacket(RnpPacketSerialized& packet);

        /**
         * @brief Serialize into buf
         * 
         * @param buf 
         */
        void serialize(std::vector<uint8_t>& buf) override;

        //data members
        uint8_t address;

        static constexpr size_t size(){return sizeof(address);};
};

class SetRoutePacket: public RnpPacket{
    enum class ADDRESS_TYPE{
        NOTYPE = 0,
        STRING = 1
    };
    private:
        static constexpr auto getSerializer()
        {
            auto ret = RnpSerializer(
                &SetRoutePacket::destination,
                &SetRoutePacket::iface,
                &SetRoutePacket::metric,
                &SetRoutePacket::address_type,
                &SetRoutePacket::address_len
            );
            return ret;
        }
    public:
        ~SetRoutePacket();

        /**
         * @brief Construct a new Ping Packet object
         * 
         * @param sys_time 
         */
        SetRoutePacket(uint8_t destination,Route& route);

        /**
         * @brief Deserialize Ping Packet
         * 
         * @param packet 
         */
        SetRoutePacket(RnpPacketSerialized& packet);

        /**
         * @brief Serialize into buf
         * 
         * @param buf 
         */
        void serialize(std::vector<uint8_t>& buf) override;

        /**
         * @brief Returns the data in a route object for easier insertion into routing table
         * 
         * @return Route 
         */
        Route getRoute();

        //data members
        uint8_t destination;

        uint8_t iface;
        uint8_t metric;

        uint8_t address_type;
        uint8_t address_len;
        
        std::array<uint8_t,32> address_data; //address payload
        
        static constexpr size_t size(){
            return getSerializer().member_size() + sizeof(address_data);
        };
};

