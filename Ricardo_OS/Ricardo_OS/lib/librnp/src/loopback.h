#pragma once

#include <memory>
#include <vector>
#include <string>


#include "rnp_interface.h"

struct LoopbackInfo : public RnpInterfaceInfo{
    // who knows what should go here
};

class Loopback : public RnpInterface{
    public:
        Loopback(std::string name = "loopback1");

        void setup() override;
        void update() override;

        void sendPacket(RnpPacket& data) override;
        //void send(std::vector<uint8_t>& data) override;
        
        const RnpInterfaceInfo* getInfo() override {return &info;};
        std::string getName() override {return _name;};
        ~Loopback(){};

    private:
        LoopbackInfo info;
        const std::string _name; // for easier human identification

};