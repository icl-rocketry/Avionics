#pragma once

#include <iostream>
#include <cstring>

#include <functional>
#include <optional>

#include <assert.h>


using flightVariable_t = std::optional<float>;
using flightVariableFunc_t = std::function<flightVariable_t(int)>;
using conditionOperator_t = std::function<bool(bool,bool)>;
using flightVariableOperator_t = std::function<bool(float,float)>;

namespace ConditionOperator{
    static flightVariableOperator_t LESSTHAN = [](float a,float b){return a < b;};
    static flightVariableOperator_t MORETHAN = [](float a,float b){return a >= b;};
    static conditionOperator_t AND = [](bool a,bool b){return a && b;};
    static conditionOperator_t OR = [](bool a,bool b){return a || b;};
}

class Condition{
    public:
        Condition(flightVariableFunc_t flightVariable,int flightVariableIndex, float threshold, flightVariableOperator_t op):
        _flightVariable(flightVariable),
        _flightVariableIndex(flightVariableIndex),
        _threshold(threshold),
        _op(std::move(op))
        {}

        bool operator()()
        {
            std::optional<float> flightvar = _flightVariable(_flightVariableIndex); // get flightvariable
            //check flight variable returned okay
            
            
            if (flightvar){ 
                return _op(flightvar.value(),_threshold); // perform operation
            }else{
                return false;
            }
        };

    private:
        flightVariableFunc_t _flightVariable;
        int _flightVariableIndex;
        float _threshold;
        flightVariableOperator_t _op;
        
};


class ConditionCombination{
    public :
        //conditions represented as functors so that Condition combinations can also be compared
        ConditionCombination(std::function<bool()> condition1, std::function<bool()> condition2, conditionOperator_t op):
        _condition1(std::move(condition1)),
        _condition2(std::move(condition2)),
        _op(std::move(op))
        {}

        bool operator()()
        {
            return _op(_condition1(),_condition2());
        }

    private:
        std::function<bool()> _condition1;
        std::function<bool()> _condition2;
        conditionOperator_t _op;

};
