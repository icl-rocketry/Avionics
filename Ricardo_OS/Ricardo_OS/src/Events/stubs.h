#pragma once

#include <string>
#include <iostream>
#include <optional>
#include <functional>


struct Engine{
    void operator()(int arg,int id){
            std::cout<<"Engine: "+std::to_string(id)+" Action with arg: " + std::to_string(arg)<<std::endl;
        };
};

class EngineHandler{
    public:
        std::function<void(int arg)> get(int idx){
            return [idx,this](int arg){e1(arg,idx);};
        };
    private:
        Engine e1;
        
};

struct deployer{
    void operator()(int arg,int id){
            std::cout<<"Deployment: "+std::to_string(id)+"  Action with arg: " + std::to_string(arg)<<std::endl;
        };
};

class DeploymentHandler{
    public:
        std::function<void(int arg)> get(int idx){
            return [idx,this](int arg){d1(arg,idx);};;
        };
    private:
        deployer d1;
        
};

