#include "commandBuffer.h"

CommandBuffer::CommandBuffer() {
};

void CommandBuffer::addCommand(Command command) {
	buffer.push_back(command);
}