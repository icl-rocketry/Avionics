#ifndef ROUTING_TABLE_H
#define ROUTING_TABLE_H
#include "interfaces/interfaces.h"
#include "nodes.h"

/*currently static routing tables created for ease but could be rewritten to be dynamic later on
format of routing table
routingtable[Current node][Destination Node]= { Gateway , Metric }

WARNING: IF YOU EDIT THIS TABLE PLEASE MAKE SURE YOU GET IT CORRECT
IF YOU DONT YOU MAY MAKE PACKETS CYCLE THE NETWORK CONTINOUSLY NOT DOING
ANYTHING USEFUL....

MAKE SURE YOU GET THE LOOPBACK INTERFACE CORRECT WORSE THINGS COULD HAPPEN LIKE
REFERECNING A DELETED POINTER... NOT GOOD

might be actually worth implementing ttl in packets to prevent packets looping infitley

*/

struct RoutingTable{
    Interface gateway;
    uint8_t metric;
};

const RoutingTable routingtable[2][5] =
{
    {{Interface::LOOPBACK,0},{Interface::LORA,1},{Interface::LORA,2},{Interface::CAN,1},{Interface::USBSerial,1}}, 
    {{Interface::LORA,1},{Interface::LOOPBACK,0},{Interface::USBSerial,1},{Interface::LORA,2},{Interface::USBSerial,1}}
};


#endif