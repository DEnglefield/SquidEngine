#pragma once
#include <glad/glad.h>
#include <vector>

class UniformBuffer{
protected:
	unsigned int bufferID;
	unsigned int bindingID;
	size_t highestWriteOffset;

	void createUniformBuffer(size_t bufferSize, unsigned int binding);

public:
	UniformBuffer(size_t bufferSize, unsigned int binding);
	void setBinding(unsigned int bindingID);
	void addData(size_t size, void* item);
	void setData(size_t beginOffset, size_t size, void* data);
	inline void destroy() { glDeleteBuffers(1, &bufferID); };
	inline unsigned int getBinding() { return bindingID; };
};