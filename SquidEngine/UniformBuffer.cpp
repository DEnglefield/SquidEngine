
#pragma once
#include "UniformBuffer.h"


UniformBuffer::UniformBuffer(size_t bufferSize, unsigned int bindingID) {
	createUniformBuffer(bufferSize, bindingID);
};


void UniformBuffer::createUniformBuffer(size_t bufferSize, unsigned int bindingID) {
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
	glBufferData(GL_UNIFORM_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	setBinding(bindingID);

}


void UniformBuffer::setBinding(unsigned int bindingID) {
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingID, bufferID);
}


unsigned int UniformBuffer::addData(size_t size, void* item) {
	setData(highestWriteOffset, size, item);
	writeOffsets.push_back(highestWriteOffset);
	highestWriteOffset += size;
	return writeOffsets.size() - 1;
}


void UniformBuffer::setData(unsigned int offset, size_t size, void* item) {
	setData(writeOffsets[offset], size, item);
}


void UniformBuffer::setData(size_t beginOffset, size_t size, void* data) {
	glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
	glBufferSubData(GL_UNIFORM_BUFFER, beginOffset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

