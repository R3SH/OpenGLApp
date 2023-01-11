#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, int size)
{
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::SetData(const void* data, int size) const
{
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

void VertexBuffer::SetSubData(const void* data, int size, int offset)
{
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}
