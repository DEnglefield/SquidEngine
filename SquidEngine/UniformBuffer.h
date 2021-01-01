#pragma once
#include <glad/glad.h>
#include <vector>

class UniformBuffer{
protected:
	unsigned int bufferID;

	size_t highestWriteOffset;
	std::vector<size_t> writeOffsets;

	void createUniformBuffer(size_t bufferSize, unsigned int bindingID);

public:
	UniformBuffer(size_t bufferSize, unsigned int bindingID);
	void setBinding(unsigned int bindingID);
	unsigned int addData(size_t size, void* item);
	void setData(unsigned int offset, size_t size, void* item);
	void setData(size_t beginOffset, size_t size, void* data);
	inline void destroy() { glDeleteBuffers(1, &bufferID); }
};