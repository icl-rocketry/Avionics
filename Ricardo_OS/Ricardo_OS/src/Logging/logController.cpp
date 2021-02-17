#include "logController.h"
#include "config.h"
#include "storageController.h"


LogController::LogController(StorageController* storagecontroller):
_storagecontroller(storagecontroller)
{};

void LogController::log(state_t &estimator_state) {

}

void LogController::log(raw_measurements_t &raw_sensors) {
	
}

void LogController::log(PacketHeader &header) {
	
}