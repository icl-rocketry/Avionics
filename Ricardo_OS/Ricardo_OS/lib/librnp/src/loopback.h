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
        
        const RnpInterfaceInfo* getInfo() override {return &info;};

        ~Loopback(){};

    private:
        LoopbackInfo info;
        
};