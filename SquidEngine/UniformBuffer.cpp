
#pragma once
#include "UniformBuffer.h"


UniformBuffer::UniformBuffer(size_t bufferSize, unsigned int bindingID) {
	createUniformBuffer(bufferSize, bindingID);
};


void UniformBuffer::createUniformBuffer(size_t bufferSize, unsigned int binding) {
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
	glBufferData(GL_UNIFORM_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	setBinding(binding);

}


void UniformBuffer::setBinding(unsigned int binding) {
	bindingID = binding;
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingID, bufferID);
}


void UniformBuffer::addData(size_t size, void* item) {
	setData(highestWriteOffset, size, item);
	highestWriteOffset += size;
}


void UniformBuffer::setData(size_t beginOffset, size_t size, void* data) {
	glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
	glBufferSubData(GL_UNIFORM_BUFFER, beginOffset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

