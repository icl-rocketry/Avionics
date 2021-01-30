#ifndef LOGFRAME_H
#define LOGFRAME_H
#include <string>
//logframes
class logframe{
    //serialize method
    virtual std::string serialize() = 0; //purley virtaul seralize method
};

class telemetry_logframe:public logframe{
    
};
class system_logframe:public logframe{

};
class network_logframe:public logframe{

};


#endif