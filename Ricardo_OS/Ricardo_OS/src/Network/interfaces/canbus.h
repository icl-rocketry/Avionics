#pragma once

#include "Storage/systemstatus.h"
#include "Storage/logController.h"

#include <memory>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>

#include "rnp_interface.h"
#include "rnp_packet.h"

#include "driver/gpio.h"
#include "driver/can.h"

#include "ricardo_pins.h"

struct CanBusInterfaceInfo:public RnpInterfaceInfo{
    uint8_t maxSendBufferElements;
    uint8_t maxReceiveBufferElements;

    bool sendBufferOverflow;
    bool receiveBufferOverflow;
};

struct RnpCanIdentifier
{
    //Can Identifier members, the first 3 members make up the can packet unique identifier while the last member is the segmentation id.
    /**
     * @brief Source Address of packet
     * 
     */
    uint8_t source;
    /**
     * @brief Destination Address of packet
     * 
     */
    uint8_t destination;
    /**
     * @brief  Can physial layer packet id NOT the rnp packet id. only 5 bits!
     * 
     * 
     */
    uint8_t can_packet_id; 
    /**
     * @brief Segment ID, 0 indicates the start of a packet. If counter overflows, index starts at 1 not zero
     * 
     */
    uint8_t seg_id;
    
    static constexpr size_t size = 29; // 29bit identifier

    RnpCanIdentifier(const RnpHeader &header,uint8_t can_packet_id) : source(header.source),
                                                destination(header.destination),
                                                can_packet_id(can_packet_id),
                                                seg_id(0)
                                                {};

    RnpCanIdentifier(uint32_t can_id) : source(can_id & 0xFF),
                                        destination((can_id >> 8) & 0xFF),
                                        can_packet_id((can_id >> 16) & 0x1F),
                                        seg_id((can_id >> 21) & 0xFF)
                                        {};

    /**
     * @brief Returns the 29 bit can identifier. Masks the can_packet_id to the first 5 bits!
     * 
     * @return uint32_t 
     */
    uint32_t getIdentifier() const {return source | (destination << 8) | ((can_packet_id & 0x1F) << 16) | (seg_id << 21);};
    /**
     * @brief Extracts the unique packet identifer from the can identifier i.e ignores the segmentation part of the id.
     * 
     * @param can_id 
     * @return uint32_t 
     */
    static uint32_t getCanPacketUID(uint32_t can_id){return can_id & 0x1FFFFF;};
};

class CanBus: public RnpInterface{
    public:
        CanBus(SystemStatus& systemstatus,LogController& logcontroller,uint8_t id,std::string name="Can0");
        void setup() override;

        void sendPacket(RnpPacket& data) override;
        void update() override;
        const RnpInterfaceInfo* getInfo() override{return &_info;};
        
        /**
         * @brief Set the Hardware Acceptance Filter. Will uninstall and reinstall can driver 
         * NOT IMPLEMENTED YET!
         * @param mask 
         */
        void setAcceptanceFilter(uint32_t mask);

    private:
        SystemStatus& _systemstatus;
        LogController& _logcontroller;

        CanBusInterfaceInfo _info;

        // CAN DRIVER CONFIG //
        static constexpr can_general_config_t can_general_config = {.mode = CAN_MODE_NORMAL,
                                                                    .tx_io = (gpio_num_t) TxCan,
                                                                    .rx_io = (gpio_num_t) RxCan,
                                                                    .clkout_io = CAN_IO_UNUSED,
                                                                    .bus_off_io = CAN_IO_UNUSED,
                                                                    .tx_queue_len = 5,
                                                                    .rx_queue_len = 5,
                                                                    .alerts_enabled = CAN_ALERT_NONE,
                                                                    .clkout_divider = 0,
                                                                    };
        // static constexpr can_general_config_t can_general_config = CAN_GENERAL_CONFIG_DEFAULT(TxCan,RxCan,CAN_MODE_NORMAL);

        static constexpr can_timing_config_t can_timing_config = CAN_TIMING_CONFIG_1MBITS();
        static constexpr can_filter_config_t can_filter_config = CAN_FILTER_CONFIG_ACCEPT_ALL();
        // CAN DRIVER CONFIG //

        uint8_t packet_counter{0};
        /**
         * @brief Generates an incrementing sequence of 5bit unsigned integers which wraps back to zero on overflow
         * 
         * @return uint8_t 
         */
        uint8_t generateCanPacketId();

        static constexpr uint32_t receive_buffer_expiry = 120*1000; //2 minute expiry time 

        struct send_buffer_element_t{
            RnpCanIdentifier canidentifier;
            std::vector<uint8_t> bytedata; 
        };

        /**
         * @brief Send buffer Container
         * 
         */
        std::queue<send_buffer_element_t> _sendBuffer;

        struct receive_buffer_element_t
        {
            std::vector<uint8_t> bytedata;
            size_t expected_size;
            uint8_t seg_id;
            uint32_t last_time_modified;
        };

        /**
         * @brief Receive Buffer Container
         * 
         */
        std::unordered_map<uint32_t,receive_buffer_element_t> _receiveBuffer;


        void processReceivedPackets();

        void processSendBuffer();

        /**
         * @brief How long before a cleanup is called
         * 
         */
        static constexpr uint32_t cleanup_delta = 60*1000; // every minute
        uint32_t prevTime;
        void cleanupReceiveBuffer();
        




};



