/*
   We had help from the shader class in the learnOpenGL pdf text book and code repository.
   Here is the License: https://github.com/JoeyDeVries/LearnOpenGL/blob/master/LICENSE.md
*/

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include "glm/glm.hpp"

class Shader
{
public:
	// the program ID
	unsigned int shader_ID;
	Shader();
	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);
	// use/activate the shader
	void useShader();
	// utility uniform fucntions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec2(const std::string& name, float x, float y) const;
	
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;

	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w);

	void setMat2(const std::string& name, const glm::mat2& mat) const;
	
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	
	void setMat4(const std::string& name, const glm::mat4& mat) const;
};

#endif