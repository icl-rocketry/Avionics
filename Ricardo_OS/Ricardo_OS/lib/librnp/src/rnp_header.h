#pragma once

#include <vector>
#include <variant>
#include <sstream>
#include "rnp_serializer.h"

class RnpHeader {
        
    private:

        static constexpr auto getSerializer()
        {
            auto ret = RnpSerializer(
                &RnpHeader::start_byte,
                &RnpHeader::packet_len,
                &RnpHeader::uid,
                &RnpHeader::source_service,
                &RnpHeader::destination_service,
                &RnpHeader::type,
                &RnpHeader::source,
                &RnpHeader::destination,
                &RnpHeader::hops
            );
            return ret;
        }

       

       
    public:
        RnpHeader();
        RnpHeader(uint8_t packetSerivce, uint8_t packetType, uint16_t packetSize); // Initialise a packet
        RnpHeader(const std::vector<uint8_t> &data); // Deserialization constructor
        ~RnpHeader();
        
        /*
            Serializes the header for network transport.
        */

        //todo implement serialize with offset
        /**
         * @brief Serializes header and places serialized daa at the end of the provided buffer
         * 
         * @param buf 
         */
        void serialize(std::vector<uint8_t>& buf) const;

        //header packet defintion
        uint8_t start_byte = 0xAF; // Marks the begin of the header
        uint16_t packet_len = 0x00; // packet length not including header length
        uint16_t uid = 0x00000000; // unique id -> used for request response needs to be unique for source desintaiton pair
        uint8_t source_service = 0x00; //source service id
        uint8_t destination_service = 0x00; //service id
        uint8_t type = 0x00; // Type of the packet
        uint8_t source = 0x00; // Source interface ID for the packet
        uint8_t destination = 0x00; // Destination interface ID for the packet
        uint8_t hops = 0x00; // number of hops, increments each time the packet is sent

        //non-serialized members
        uint8_t src_iface;
        std::variant<std::monostate,std::string> lladdress;

        static constexpr size_t size(){
                return getSerializer().member_size();
        }
        
        static std::stringstream print(RnpHeader& header){
            std::stringstream aout;
            aout<< ">>>HEADER<<<\n";
            aout<<  "start_byte: " << (int)header.start_byte << "\n";  
            aout<<  "packet_len: " << (int)header.packet_len<< "\n"; 
            aout<<  "uid: " << (int)header.uid<< "\n"; 
            aout<<  "source_service: " << (int)header.source_service<< "\n"; 
            aout<<  "destination_service: " << (int)header.destination_service<< "\n"; 
            aout<<  "type: " << (int)header.type<< "\n"; 
            aout<<  "source: " << (int)header.source<< "\n";
            aout<<  "destination: " << (int)header.destination<< "\n"; 
            aout<<  "hops: " << (int)header.hops<< "\n";
            return aout;
        }
};