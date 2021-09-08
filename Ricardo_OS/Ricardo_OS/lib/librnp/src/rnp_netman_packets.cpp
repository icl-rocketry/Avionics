#include "rnp_netman_packets.h"
#include "rnp_packet.h"
#include "rnp_header.h"
#include "rnp_serializer.h"
#include "rnp_networkmanager.h"
#include "rnp_routingtable.h"

#include <cstring>

PingPacket::~PingPacket()
{};

PingPacket::PingPacket(uint32_t _sys_time):
RnpPacket(static_cast<uint8_t>(DEFAULT_SERVICES::NETMAN),
          static_cast<uint8_t>(NETMAN_TYPES::PING_REQ),
          size()),
sys_time(_sys_time)
{};

PingPacket::PingPacket(RnpPacketSerialized& packet):
RnpPacket(packet.header)
{
    std::memcpy(&sys_time,packet.getBody().data(),size());
};

void PingPacket::serialize(std::vector<uint8_t> & buf){
    RnpPacket::serialize(buf);
    size_t bufsize = buf.size();
    buf.resize(bufsize + size());
    std::memcpy(buf.data() + bufsize,&sys_time,size());  
};

SetAddressPacket::~SetAddressPacket()
{};

SetAddressPacket::SetAddressPacket(uint8_t _address):
RnpPacket(static_cast<uint8_t>(DEFAULT_SERVICES::NETMAN),
          static_cast<uint8_t>(NETMAN_TYPES::SET_ADDRESS),
          size()),
address(_address)
{};

SetAddressPacket::SetAddressPacket(RnpPacketSerialized& packet):
RnpPacket(packet.header)
{
    std::memcpy(&address,packet.getBody().data(),size());
};

void SetAddressPacket::serialize(std::vector<uint8_t> & buf){
    RnpPacket::serialize(buf);
    size_t bufsize = buf.size();
    buf.resize(bufsize + size());
    std::memcpy(buf.data() + bufsize,&address,size());  
};

SetRoutePacket::~SetRoutePacket()
{};

SetRoutePacket::SetRoutePacket(uint8_t dest,Route& route):
RnpPacket(static_cast<uint8_t>(DEFAULT_SERVICES::NETMAN),
          static_cast<uint8_t>(NETMAN_TYPES::SET_ROUTE),
          size()),
destination(dest),
iface(route.iface),
metric(route.metric)
{
    switch(route.address.index()){
        case 0:
        {
            //std::monostate -> no link layer addressing
            address_type = (uint8_t)ADDRESS_TYPE::NOTYPE;
            address_len = 0;
            break;
        }
        case 1:
        {
            //std::string addressing
            const std::string& address_string = std::get<std::string>(route.address);
            address_type = (uint8_t)ADDRESS_TYPE::STRING;
            address_len = address_string.size();
            std::memcpy(address_data.data(),address_string.data(),address_len);
            break;
        }
        default:
        {
            //unrecognized type
            address_type = (uint8_t)ADDRESS_TYPE::NOTYPE;
            address_len = 0;
            break;
        }
    }
};

SetRoutePacket::SetRoutePacket(RnpPacketSerialized& packet):
RnpPacket(packet.header)
{
    getSerializer().deserialize(*this,packet.getBody());
    const size_t offset = header.size() + getSerializer().member_size(); // the std::array is not included inthe serialziation framework 
    std::memcpy(address_data.data(),packet.packet.data() + offset,address_len); // only copy what we neeed
};

void SetRoutePacket::serialize(std::vector<uint8_t> & buf){
    RnpPacket::serialize(buf);
    const size_t bufsize = buf.size();
    const size_t member_size = getSerializer().member_size();
    buf.resize(bufsize + size());
    //CAREFUL -> 2 seperate copy operations as address_Data is not included inthe serializer
    std::memcpy(buf.data() + bufsize,
                getSerializer().serialize(*this).data(),
                member_size);
    std::memcpy(buf.data() + bufsize + member_size,
                address_data.data(),
                address_len); // only copy the necessary length -> rest of the elements in the vector are default constructed to zero
}

Route SetRoutePacket::getRoute() 
{
    Route ret{iface,metric};
    switch(address_type){
        case (uint8_t)ADDRESS_TYPE::NOTYPE:
        {
            return ret;
        }
        case (uint8_t)ADDRESS_TYPE::STRING:
        {
            std::string address(reinterpret_cast<char*>(address_data.data()),address_len); // reinterpret cast used here as underlying type of string is char* while uint8_t typedefs to unsigned char*
            ret.address = address;
            return ret;
        }
        default:
        {
            return ret;
        }
    }
};