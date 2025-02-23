#pragma once
#include <GL/glew.h>
#include <vector>

class VBO
{
private:
	GLuint m_id;
	GLenum m_target;
	GLenum m_mode;
public:
	VBO(GLenum target = GL_ARRAY_BUFFER, GLenum mode = GL_STATIC_DRAW);
	template<typename T>
	VBO(const std::vector<T> data, GLenum target = GL_ARRAY_BUFFER, GLenum mode = GL_STATIC_DRAW);
	void bind() const;
	void unbind() const;
	tamplate<typename T>
	void data(std::vector<T> data);
};

VBO::VBO(GLenum target) : m_target(target), m_mode(mode)
{
	glGenBuffers(1, &m_id);
}

template<typename T>
inline VBO::VBO(const std::vector<T> data, GLenum target, GLenum mode) : m_target(target), m_mode(mode)
{
	glGenBuffers(1, &m_id);
	bind();
	glBuffer
}

void VBO::bind() const
{
	glBindBuffer(m_target, m_id);
}
void VBO::unbind() const
{
	glBindBuffer(m_target, 0);
}

tamplate<typename T>
void VBO::data(std::vector<T> data)
{
	glBufferData(m_target, data.data());
}