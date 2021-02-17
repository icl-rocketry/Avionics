#ifndef LOGFRAME_H
#define LOGFRAME_H
#include <string>
//logframes
class logframe{
    //serialize method
    virtual std::string serialize(); //purley virtaul seralize method

};

class telemetry_logframe:public logframe{

    std::string seralize(){return "hi";};
};
class system_logframe:public logframe{

    std::string seralize(){return "hi";};

};
class network_logframe:public logframe{

    std::string seralize(){return "hi";};

};


#endif