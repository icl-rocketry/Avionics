#ifndef LOGFRAME_H
#define LOGFRAME_H
#include <string>
//logframes
class logframe{
    //serialize method
    virtual std::string serialize(); //purley virtaul seralize method
};

class telemetry_logframe:public logframe{

    std::string seralize(){};
};
class system_logframe:public logframe{

    std::string seralize(){};

};
class network_logframe:public logframe{

    std::string seralize(){};

};


#endif