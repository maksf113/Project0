#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include "GLError.h"
#include "Graphics\VAO.h"
#include "Graphics\VBO.h"
#include "Graphics\EBO.h"
#include "Graphics\ShaderProgram.h"

class Renderer
{
private:
	GLFWwindow* m_window;
	VAO* va;
	VAO* triVa;
	ShaderProgram* shader;
	unsigned int vertCount;
	unsigned int triVertCount;
public:
	Renderer() : m_window(nullptr), va(nullptr), shader(nullptr) {}
	bool init()
	{
		if (!glfwInit()) 
		{
			std::cerr << "Failed to initialize GLFW" << std::endl;
			return false;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		m_window = glfwCreateWindow(1920, 1920, "Star - shaped Polygon", NULL, NULL);
		if (!m_window)
		{
			std::cerr << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(m_window);

		if(glewInit() != GLEW_OK)
		{
			std::cerr << "Failed to initialize GLEW" << std::endl;
			glfwDestroyWindow(m_window);
			glfwTerminate();
			return false;
		}

		glfwSwapInterval(1);
		return true;
	}
	void setMesh(const std::vector<float>& verts, const std::vector<float>& triVerts)
	{
		VBL layout;
		layout.push<float>(2);
		vertCount = verts.size() / 2;
		std::cout << "#vertices = " << vertCount << std::endl;
		va = new VAO;
		VBO vb(verts);
		va->addBuffer(vb, layout);
		triVertCount = triVerts.size() / 2;
		std::cout << "#triVertices = " << triVertCount << std::endl;
		triVa = new VAO;
		VBO triVb(triVerts);
		triVa->addBuffer(triVb, layout);
		shader = new ShaderProgram("Graphics/Shaders/default.vert",
			"Graphics/Shaders/default.frag");
		shader->bind();
		shader->setUniform("color", 0.8f, 0.0f, 0.7f);
	}
	void display()
	{
		GL(glClearColor(0.0, 0.0, 0.0, 1.0));
		GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GL(glViewport(0, 0, 1920, 1920));
		shader->bind();
		va->bind();
		GL(glPointSize(10.0f));
		GL(glDrawArrays(GL_POINTS, 0, vertCount));
		triVa->bind();
		GL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		GL(glDrawArrays(GL_TRIANGLES, 0, triVertCount));
	}
	void render()
	{
		while (!glfwWindowShouldClose(m_window))
		{
			display();
			glfwSwapBuffers(m_window);
			glfwPollEvents();
		}
	}
	void clear()
	{
		if (m_window)
		{
			glfwDestroyWindow(m_window);;
			m_window = nullptr;
		}
		delete shader;
		delete va;
		delete triVa;
		glfwTerminate();
	}
};