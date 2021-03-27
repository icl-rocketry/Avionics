#pragma once



class FlightVariable{
    public:
        FlightVariable();
        double get(){return _value;};
        virtual void update() = 0;
        virtual ~FlightVariable() = 0;
    private:
        double _value;

};

FlightVariable::~FlightVariable(){}; 

class Time_Since_Launch:public FlightVariable{
    public:
        Time_Since_Launch();

        void update();
    
};
