#ifndef MICROSD_H
#define MICROSD_H

#include "Arduino.h"
#include "storage.h"
#include "SD.h"

class MicroSD: public Storage {

	public:
		MicroSD();
		void deleteFile(const std::string path);
		void renameFile(const std::string path1, const std::string path2);
		void appendFile(const std::string path, const std::string msg);
		void writeFile(const std::string path, const std::string msg);
		File readFile(const std::string path);
		void removeDir(const std::string path);
		void createDir(const std::string path);
};

#endif