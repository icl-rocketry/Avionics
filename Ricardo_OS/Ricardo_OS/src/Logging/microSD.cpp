#include "microSD.h"
#include "SD.h"


void MicroSD::MicroSD() {
	if(!SD.begin()) {
		// Card not mounted, handle this error
	}
}

void MicroSD::deleteFile(const std::string path) {

}

void MicroSD::renameFile(const std::string path1, const std::string path2) {

}

void MicroSD::appendFile(const std::string path, const std::string msg){

}

void MicroSD::writeFile(const std::string path, const std::string msg) {

}

File MicroSD::readFile(const std::string path) {

}

void MicroSD::removeDir(const std::string path) {

}

void MicroSD::createDir(const std::string path) {

}