#include "iface.h"
#include <vector>
#include "interfaces.h"
#include "../packets.h"

Iface::~Iface(){};

void Iface::updateSourceInterface(std::vector<uint8_t> &packet_data,std::vector<uint8_t> &destination,INTERFACE interface){
    // deserialize packet header
    PacketHeader packetheader = PacketHeader(packet_data);
    packetheader.src_interface = static_cast<uint8_t>(interface);
    //Serialize header back into original packet data vector
    packetheader.serialize(destination);
}