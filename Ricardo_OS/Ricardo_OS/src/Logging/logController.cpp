#include "logController.h"
#include "config.h"
#include "storageController.h"


LogController::LogController(StorageController* storagecontroller):
_storagecontroller(storagecontroller),
telemetry_frame_buffer(10), // preallocate to 10 frames
system_frame_buffer(10), // preallocate to 10 frames
network_frame_buffer(10), // preallocate to 10 frames
engine_frame_buffer(10) // preallocate to 10 frames
{};

void LogController::log(state_t &estimator_state) {

}

void LogController::log(raw_measurements_t &raw_sensors) {
	
}

void LogController::log(PacketHeader &header) {
	
}

void LogController::log(std::string message) {
	//will create a new log frame each time it is called
}
void LogController::log(uint32_t status,uint32_t flag,std::string reason) {
    //will create a new log frame each time it is called
    

	
}
void LogController::log(uint32_t status,uint32_t flag) {
    //will create a new log frame each time it is called
	
}

void LogController::update(){
// do we check if buffer is full or time based?
}
void LogController::write_to_file(){
    
}