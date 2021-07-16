#ifndef ROUTING_TABLE_H
#define ROUTING_TABLE_H


#include "interfaces/interfaces.h"
#include "nodes.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <exception>

/*
routing table class implementation using nested vectors with ability to modify during runtime. Also has nice printing.
TODO! 
add logging if bounds are out of bounds lol
*/



struct RoutingTableEntry{
    INTERFACE gateway;
    uint8_t metric;
};
/*
const RoutingTableEntry routingtable[2][5] =
{
    {{INTERFACE::LOOPBACK,0},{INTERFACE::LORA,1},{INTERFACE::LORA,2},{INTERFACE::CAN,1},{INTERFACE::USBSerial,1}}, 
    {{INTERFACE::LORA,1},{INTERFACE::LOOPBACK,0},{INTERFACE::USBSerial,1},{INTERFACE::LORA,2},{INTERFACE::USBSerial,1}}
};
*/

class RoutingTable{
    public:
        RoutingTable(){};

        RoutingTable(int sources, int destinations):
        _table(sources,std::vector<RoutingTableEntry>(destinations))
        {};



        int size(int dim){
            switch(dim){
                case 0: //number of rows
                {
                    return _table.size();
                }
                case 1: // total size
                {
                    int sum = 0;
                    for (int i = 0; i<_table.size();i++)
                    {
                        sum += _table[i].size();
                    }
                    return sum;
                }
                default: 
                {
                    return 0;
                    
                }
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


        template<typename T> RoutingTableEntry operator()(T source_idx, T destination_idx) const{ //the get operator -> returns an error entry so we can discard the dodgy packet 
            
            std::vector<RoutingTableEntry> entries = _table.at(static_cast<uint8_t>(source_idx));
            if (destination_idx >= entries.size()){
                
                return error_entry;
                
            }else{
                return entries.at(static_cast<uint8_t>(destination_idx));
            }
        };



    private:
        std::vector< std::vector<RoutingTableEntry> > _table;
        const RoutingTableEntry error_entry{INTERFACE::ERROR,255}; // error entry return
       
};


#endif