#pragma once


#include <vector>
#include <optional>
#include <variant>
#include <utility>
#include <sstream>
#include <string>
#include <iostream>





struct Route{
    uint8_t iface;
    uint8_t metric;
    std::variant<std::monostate,std::string> address;
};

//overload monostate stream operator so that std::visit plays nicley with 
// std::ostream& operator<<(std::ostream& os, const std::monostate& var)
// {
//     os << "- NO ADDRESS - |";
//     return os;
// }

class RoutingTable{
    public:
        RoutingTable(){};

        RoutingTable(int destinations):
        _table(destinations)
        {};

        size_t size(){return _table.size();};

        void setRoute(uint8_t destination,const Route& entry){
            if (destination >= _table.size()){
                _table.resize(destination+1);
            }
            _table.at(destination) = {true,entry};
        };

        std::optional<Route> getRoute(uint8_t destination){
            if (destination >= _table.size()){
                //out of bounds
                return {};
            }
            std::pair<bool,Route> result = _table.at(destination);
            if (!std::get<bool>(result)){
                // no data at index
                return {};
            }
            return {std::get<Route>(result)};
            
        };

        void deleteRoute(uint8_t destination){
            if (destination >= _table.size()){
                //out of bounds
                return; 
            }
            if (destination == (_table.size() - 1)){
                _table.erase(_table.end()); //we can fully delete the last element
                return;
            }
            _table.at(destination) = {false,Route()};
        }

        void clearTable(){
            _table.clear();
        }

        void loadJson(){}; // loads table from supplied json string
        void saveJson(){}; // saves table as json string

        std::stringstream printTable(){
            std::stringstream sout;
            sout << ">>>ROUTING TABLE<<<"<<"\n";
            sout << "|destination|iface|metric|link layer address|" << "\n";
            for (size_t i = 0; i<_table.size();i++){
                auto elem = _table.at(i);
                sout<<"| "<<i;
                if (!std::get<bool>(elem)){
                    sout<< " | - NO ROUTE - " << "\n";
                }else{
                    Route r = std::get<Route>(elem);
                    sout<<" | "<<(int)r.iface<<" | "<<(int)r.metric<<" | ";
                    if (std::holds_alternative<std::monostate>(r.address)){ // cant visit the variant as std::monostate does not have a ostream operator defined 
                        sout<<" - NO ADDRESS - |";
                    }else if (std::holds_alternative<std::string>(r.address)){
                        sout<<std::get<std::string>(r.address)<<" |";
                        //std::visit([&sout](const auto &elem) { sout << elem << "|"; }, r.address);
                        // std::visit([&sout](auto& lla){ sout << lla << "|"; }, r.address);
                    }else{
                        sout<<" INVALID ADDRESS TYPE |";
                    }
                }
                sout<< "\n";
            }
            return sout;
        }


    private:
        std::vector<std::pair<bool,Route>> _table;

};

