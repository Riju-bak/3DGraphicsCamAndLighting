#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>	//Perhaps I don't need this

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



std::string GetFileContents(const char* filePath);

int GenVertexShader(const char* vertexFile);

int GenFragmentShader(const char* fragmentFile);

class Shader
{
public:
	//Ref id of shader program
	unsigned int ID;

	Shader(const char* vertexFile, const char* fragmentFile);

	void SetUniformi(const char* name, int value);
	void SetUniformf(std::string name, float value);

	void SetUniformMatrix4fv(const char* name, glm::mat4& transform);

	void SetUniformVec4(const char* name, const glm::vec4& vec);

	void Activate();

	void Delete();
	void SetUniformVec3(std::string name, const glm::vec3& vec3);
};
