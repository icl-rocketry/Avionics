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
        RoutingTable(){};

        RoutingTable(int sources, int destinations):
        _table(sources)
        {};



        int size(int dim){
            switch(dim){
                case 0: //number of rows
                {
                    return _table.size();
                    break;
                }
                case 1: // total size
                {
                    int sum = 0;
                    for (int i = 0; i<_table.size();i++){
                        sum += _table[i].size();
                    }
                    return sum;
                    break;
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

        template<typename T> RoutingTableEntry& operator()(T source_idx, T destination_idx) { //assignment operator -> if an out of bound error occurs we want to provide a sacrifical object so we can continue but log the error
           
            std::vector<RoutingTableEntry> entries = _table.at(static_cast<uint8_t>(source_idx));
            if (destination_idx >= entries.size()){
                throw std::out_of_range("requested destination_idx is out of range of routing table");
                
                return dummy_entry;
                
            }else{
                return entries.at(static_cast<uint8_t>(destination_idx));
            }
            
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
        const RoutingTableEntry error_entry{Interface::ERROR,255}; // error entry return
        RoutingTableEntry dummy_entry{Interface::ERROR,255}; // dummy entry to return from assingment operator
};


#endif