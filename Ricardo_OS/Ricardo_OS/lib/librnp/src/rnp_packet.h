#pragma once 

#include "rnp_header.h"
#include <vector>

class RnpPacket {
    public:
        ~RnpPacket();

        /**
         * @brief Construct a new Rnp Packet object
         * 
         * @param packetService 
         * @param packetType 
         * @param packet_size 
         */
        RnpPacket(uint8_t packetService, uint8_t packetType, uint16_t packet_size);

        /**
         * @brief Construct a new Rnp Packet object with header
         * 
         * @param header 
         */
        RnpPacket(RnpHeader header);

        //Serialization
        /**
         * @brief Serialize packet to provided buffer.
         * If this is called on a non-casted instance, it will only return the serialized header...
         * 
         * @param buf 
         */
        virtual void serialize(std::vector<uint8_t>& buf);

        RnpHeader header; 
};

class RnpPacketSerialized: public RnpPacket {
    public:
        ~RnpPacketSerialized();
        //Deserialization 
        /**
         * @brief Extract header from serialized byte stream
         * 
         * @param bytes byte stream of serialized packet
         */
        RnpPacketSerialized(const std::vector<uint8_t> &bytes);

        //Serialization
        /**
         * @brief Re-Serialize header back into packet. Make sure to do this if the header is modified. 
         * 
         * 
         */
        void reserializeHeader();
        
        /**
         * @brief Copies the internally stored packet to buf. Does automatically re-serialize header into buf
         * 
         * @param buf 
         */
        void serialize(std::vector<uint8_t>& buf) override;
       
        /**
         * @brief Extract packet body from packet
         * 
         * @return std::vector<uint8_t> 
         */
        std::vector<uint8_t> getBody();

        /**
         * @brief Get the size of the packet Body
         * 
         * @return size_t 
         */
        size_t getBodySize(); 

        std::vector<uint8_t> packet; // contains serialized packet and header

};