#ifndef ROUTING_TABLE_H
#define ROUTING_TABLE_H
#include "interfaces/interfaces.h"
#include "nodes.h"

/*currently static routing tables created for ease but could be rewritten to be dynamic later on
format of routing table
routingtable[Destination Node][Current node]= { Gateway , Metric }

*/

struct RoutingTable{
    Interface gateway;
    uint8_t metric;
};

RoutingTable routingtable[2][4]
{
    {{Interface::LOOPBACK,0},{Interface::LORA,1},{Interface::LORA,2},{Interface::CAN,1}}, 
    {{Interface::LORA,1},{Interface::LOOPBACK,0},{Interface::USBSerial,1},{Interface::LORA,2}}
};







#endif