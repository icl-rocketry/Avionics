#include "eventHandler.h"

#include <ArduinoJson.h>
#include <stdexcept>
#include <memory>

#include "event.h"
#include "condition.h"
#include "flightVariables.h"




void EventHandler::setup(JsonArrayConst event_config)
{
    //for each in the list of cool tings

    if (event_config.isNull())
    {
        return; // dont do nothing we have nothin to do
    }

    size_t event_list_size = event_config.size();
    if (event_list_size == 0){
        _logcontroller.log("EventHandler no events provided!");
        return;
    }

    _eventList.resize(event_list_size); // allocate elements

    int eventID = 0;

    for (JsonObjectConst jsonEvent : event_config)
    {
        #ifdef _RICDEBUG
        _decisiontree = "";
        #endif

        bool fire_mode = jsonEvent["single_fire"];
        uint16_t actionCooldown = jsonEvent["cooldown"];

        JsonVariantConst conditionJson = jsonEvent["condition"];
        condition_t eventCondition = configureCondition(conditionJson);

        JsonVariantConst actionJson = jsonEvent["action"];
        action_t eventAction;
    
        if (actionJson.isNull()){ //either it doesnt exist or "action":null
            eventAction = [](){}; // null action
        }else{
            eventAction = configureAction(actionJson);
        }

        _eventList.at(eventID) = std::make_unique<Event>(eventID,
                                                 eventCondition,
                                                 eventAction,
                                                 fire_mode,
                                                 actionCooldown,
                                                 _logcontroller);

        #ifdef _RICDEBUG
        _logcontroller.log(_decisiontree);
        #endif

        eventID++; //increment id
    }
};


action_t EventHandler::configureAction(JsonVariantConst actions){

    if (actions.is<JsonObject>()){

        JsonObjectConst actionJson = actions.as<JsonObject>();
        std::string actionType = actionJson["type"];
        int actionID = actionJson["id"];
        int actionParam = actionJson["param"];

        if (actionType == "engine")
        {
            return [actionParam,actionID,this](){_enginehandler.getActionFunc(actionID)(actionParam);}; 
        }
        else if (actionType == "deployment")
        {
            return [actionParam,actionID,this](){_deploymenthandler.getActionFunc(actionID)(actionParam);};
        }
        else if (actionType == "null")
        {
            return [](){};
        }
        else
        {
            #ifdef _RICDEBUG
            std::string actionJsonSer;
            serializeJson(actionJson,actionJsonSer);
            throw std::runtime_error("Bad Action Type: " + actionJsonSer);
            #endif
            _logcontroller.log("Bad Action Type, continuing with null action");
            return [](){};
        }
    }else if (actions.is<JsonArray>()) {
        std::vector<action_t> action_vec;

        for (JsonObjectConst action : actions.as<JsonArray>()) {
            action_vec.push_back(configureAction(action));
        }

        return [action_vec = std::move(action_vec)]() {
            for (auto& action : action_vec) {
                action();
            }
        };
    }else{
        #ifdef _RICDEBUG
        std::string actionJsonSer;
        serializeJson(actions,actionJsonSer);
        throw std::runtime_error("Bad Action config: " + actionJsonSer);
        #endif
        // return null i.e no action
        _logcontroller.log("Bad Action config, continuing with null action");
        return [](){}; // null action
    }
}

condition_t EventHandler::configureCondition(JsonVariantConst condition, uint8_t recursion_level)
{
    if (recursion_level > condition_recursion_max_depth){
        throw std::runtime_error("EventHandler max recursion depth reached!"); // this needs to fail quietly too
    }
    if (condition.is<JsonObject>())
    {

        auto conditionJson = condition.as<JsonObject>();

        if (conditionJson.containsKey("condition")){

            auto subconditionarray = conditionJson["condition"];
            size_t arraysize = subconditionarray.size();

            if (arraysize == 0){
                throw std::runtime_error("EventHandler no conditions provided"); // we can fail safe here by returning false
            }
            if (arraysize == 1){
                return configureCondition(subconditionarray.getElement(0),recursion_level); // no change to recursion depth
            }

            conditionOperator_t op;
            if (conditionJson["operator"].as<std::string>() == "AND"){
                op = ConditionOperator::AND;
            }else if (conditionJson["operator"].as<std::string>() == "OR"){
                op = ConditionOperator::OR;
            }else{
                throw std::runtime_error("Invalid Operator Supplied");
            }
            
            //generate decision tree

            //get initial condition
            #ifdef _RICDEBUG
            _decisiontree += "  (";
            #endif

            auto conditionCombination = configureCondition(subconditionarray.getElement(0),recursion_level + 1);
            
            for (int i = 1; i < arraysize;i++){
                #ifdef _RICDEBUG
                _decisiontree += "  ";
                _decisiontree += conditionJson["operator"].as<std::string>();
                _decisiontree += "  ";
                #endif

                conditionCombination = ConditionCombination(conditionCombination,
                                                             configureCondition(subconditionarray.getElement(i),recursion_level + 1),
                                                             op);
                #ifdef _RICDEBUG
                _decisiontree += " ) ";
                #endif

            }
            

            return conditionCombination;

        }else if (conditionJson.containsKey("flightVar")){

            // what happens if config requests that flight var is time since event on its self?
            // if it is the only condition and u are testing time_triggered then the event will never trigger
            // if the conditon is 'and' with another condition it will again never trigger
            // if the conditon is or'ed then if the other condition is met, the event will trigger. If the conditon comparing
            // time trigered is less than, the result is no different to just testing the other conditon. If using more than, 
            // we effectivley latch the output of the event, it will always be on meaning the action will be continually called
            
            flightVariableOperator_t op;
            if (conditionJson["operator"].as<std::string>() == "LESSTHAN"){
                op = ConditionOperator::LESSTHAN;
            }else if (conditionJson["operator"].as<std::string>() == "MORETHAN"){
                op = ConditionOperator::MORETHAN;
            }else{
                throw std::runtime_error("Invalid Operator Supplied");
            }

            #ifdef _RICDEBUG
            serializeJson(conditionJson,_decisiontree);
            #endif

            return Condition(_flightvariables.get(conditionJson["flightVar"].as<std::string>()), 
                             conditionJson["component"].as<int>(),
                             conditionJson["threshold"].as<float>(),
                             op);
        }else{
            std::string condition_string;
            serializeJson(conditionJson,condition_string);
            throw std::runtime_error("EventHandler Bad Condition Type " + condition_string);
        }


    }else{
        
        throw std::runtime_error("EventHandler Invalid type deserialized : " + condition.as<std::string>() + " depth : " + std::to_string(recursion_level));
    }

}

void EventHandler::update(const SensorStructs::state_t& state)
{
    //copy rocket state to private rocket state -> idea behind this is in the future 
    //copy allows event updates to happen in a different task to the estimator
    rocketState = state;

    for (auto &event : _eventList)
    {
        event->update();
    }

};

uint32_t EventHandler::timeTriggered(uint8_t eventID)
{
    try
    {
        return _eventList.at(eventID)->timeTriggered();
    }
    catch (std::out_of_range const &)
    {
        return 0; //this is bad
    }


}

