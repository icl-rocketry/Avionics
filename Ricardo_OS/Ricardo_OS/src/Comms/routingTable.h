#ifndef ROUTING_TABLE_H
#define ROUTING_TABLE_H
#include "interfaces/interfaces.h"
#include "nodes.h"
#include <vector>
#include <iostream>
#include <iomanip>

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

//TODO
//convert to class maybe a json object type instead??


struct RoutingTableEntry{
    Interface gateway;
    uint8_t metric;
};
/*
const RoutingTableEntry routingtable[2][5] =
{
    {{Interface::LOOPBACK,0},{Interface::LORA,1},{Interface::LORA,2},{Interface::CAN,1},{Interface::USBSerial,1}}, 
    {{Interface::LORA,1},{Interface::LOOPBACK,0},{Interface::USBSerial,1},{Interface::LORA,2},{Interface::USBSerial,1}}
};
*/

class RoutingTable{
    public:
        RoutingTable();

        RoutingTable(int sources, int destinations):
        _table(sources,std::vector<RoutingTableEntry>(destinations))
        {};



        int size(int dim){
            switch(dim){
                case 0: //number of rows
                {
                    return _table.size();
                    break;
                }
                case 1: // total size
                    int sum = 0;
                    for (int i = 0; i<_table.size();i++){
                        sum += _table[i].size();
                    }
                    return sum;
            }
        };

        void push_row(std::vector<RoutingTableEntry> new_row){
            _table.push_back(new_row);
        }

        friend std::ostream &operator<<(std::ostream &os, const RoutingTable &rt){//pretty printing
            for (int i = 0; i < rt._table.size();i++){
                os << "Node: " << i << " |";
                std::vector<RoutingTableEntry> entries = rt._table.at(i);
                for (int j = 0; j< entries.size(); j++){
                    os << "destination: " << j << " gateway: " << static_cast<uint8_t>(entries.at(j).gateway) << " metric: " << static_cast<uint8_t>(entries.at(j).metric) << " |";
                }
                os << "\n";
            }
            return os;
        };
        

        template<typename T> bool push_entry(T source_node,RoutingTableEntry new_entry){
            if (source_node < _table.size()){//checking index is within bounds
                _table.at(static_cast<uint8_t>(source_node)).push_back(new_entry);
                return true;
            }else{
                return false;
            }
        }

        template<typename T> std::vector<RoutingTableEntry>& operator()(T source_idx) {
            return _table.at(static_cast<uint8_t>(source_idx));
        };
        template<typename T> std::vector<RoutingTableEntry> operator()(T source_idx) const{
            return _table.at(static_cast<uint8_t>(source_idx));
        };

        template<typename T> RoutingTableEntry& operator()(T source_idx, T destination_idx) {
            std::vector<RoutingTableEntry> entries = _table.at(static_cast<uint8_t>(source_idx));
            return entries.at(static_cast<uint8_t>(destination_idx));
        };

        template<typename T> RoutingTableEntry operator()(T source_idx, T destination_idx) const{
            std::vector<RoutingTableEntry> entries = _table.at(static_cast<uint8_t>(source_idx));
            return entries.at(static_cast<uint8_t>(destination_idx));
        };



    private:
        std::vector< std::vector<RoutingTableEntry> > _table;
};


#endif