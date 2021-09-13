#include "loopback.h"

#include <memory>
#include <vector>
#include <string>

#include "rnp_interface.h"

Loopback::Loopback(std::string name):
RnpInterface((uint8_t)DEFAULT_INTERFACES::LOOPBACK,name)
{};

void Loopback::setup()
{};

void Loopback::update()
{};

void Loopback::sendPacket(RnpPacket& data){
    if (data.header.source != data.header.destination){
        //source and destination do not match. dump packet
        return;
    }
    if (_packetBuffer == nullptr){
        // no buffer present
        return;
    }
    
    std::vector<uint8_t> serializedData;
    data.serialize(serializedData);
    auto packet_ptr = std::make_unique<RnpPacketSerialized>(serializedData);
    packet_ptr->header.src_iface = getID();
    _packetBuffer->push(std::move(packet_ptr));
};
