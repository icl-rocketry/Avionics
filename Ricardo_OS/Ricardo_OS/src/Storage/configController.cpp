#include "configController.h"
#include "storageController.h"
#include "logController.h"
#include <ArduinoJson.h>
#include <SdFat.h>
#include <string.h>



ConfigController::ConfigController(StorageController* storagecontroller,LogController* logcontroller):
configDoc(16384),//max 16kb config size
_storagecontroller(storagecontroller),
_logcontroller(logcontroller),
_configOK(true)
{};

void ConfigController::load(){
    // File _file; // file object
    // _file = _storagecontroller->open(configuration_file_path.c_str(),STORAGE_DEVICE::MICROSD,O_RDONLY); //try to open file at directory
    // if(!_file){

    //     _configOK = false;
    //     _logcontroller->log("Error opening config file!");
    //     return;
    // }
    // DeserializationError jsonError = deserializeJson(configDoc,_file);
    // if (jsonError){
    //     _configOK = false;
    //     _logcontroller->log("Error deserializing JSON! - " + std::string(jsonError.c_str()));
    //     return;
    // }
    // _logcontroller->log("Config Loaded");
    // temporary
    std::string jsonstring = "{\n   \n    \"DeployerList\":\n\t[\n\t\t{\n\t\t\t\"id\" : 0,\n\t\t\t\"type\" : \"net_actuator\",// ur mom is a type\n\t\t\t\"address\":69,\n\t\t\t\"destination_service\":21\n\t\t},\n        {\n\t\t\t\"id\" : 1,\n\t\t\t\"type\" : \"net_actuator\",// ur mom is a type\n\t\t\t\"address\":200,\n\t\t\t\"destination_service\":32\n\t\t}\n\t],\n    \"EngineList\":\n    [\n        {\n\t\t\t// ur mom is a simp engine\n            \"id\":0,\n            \"type\":\"SimpleEngine\",\n            \"igniter\":\n            {\n                \"type\":\"net_actuator\",\n                \"address\":10,\n                \"destination_service\": 69,\n                \"param\":10 \n            }\n        },\n        {\n\t\t\t// ur mom is a simp engine\n            \"id\":1,\n            \"type\":\"SimpleEngine\",\n            \"igniter\":\n            {\n                \"type\":\"net_actuator\",\n                \"address\":42,\n                \"destination_service\": 1,\n                \"param\":10 \n            }\n        }\n    ],\n    \"ControllerList\":\n    [\n        {\n            \"id\":0,\n            \"type\": \"pid\",\n            \"doohickeyType\": \"Engine\",\n            \"doohickeyID\":0,\n            \"setpoint\":0,\n            \"stateVariable\":2, //this is a really bad example lol\n            \"Kp\":1,\n            \"Ki\":10,\n            \"Kd\":5\n        }\n    ],\n    \"Events\": // id is the index of the pyro/engine in their respective arrays\n    [\n        {\n            \"single_fire\": true,\n            \"cooldown\":0,\n            \"action\":[\n                {\n                    \"type\":\"engine\",\n                    \"id\":0,\n                    \"param\":1\n                },\n                {\n                    \"type\":\"deployment\",\n                    \"id\":1,\n                    \"param\":50\n                }\n            ],\n            \"condition\":{\n                \"operator\":\"OR\",\n                \"condition\":[\n                    {\n                    \"operator\": \"MORETHAN\",\n                    \"flightVar\": \"Acceleration\",\n                    \"component\": 1,\n                    \"threshold\": 10\n                    },\n                    {\n                    \"operator\": \"MORETHAN\",\n                    \"flightVar\": \"Velocity\",\n                    \"component\": 1,\n                    \"threshold\": 15\n                    }\n                ]\n            }\n\n        }\n    ]\n}\n";
    // deserializeJson(configDoc, jsonstring);
    _configOK = true;

}

