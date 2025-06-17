#pragma once
#include <string>
#include <glad/glad.h>

namespace Renderer {
	class ShaderProgram {
	public:
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader); //в конструктор подаётся исходник шейдеров.
		
		~ShaderProgram();
		bool isCompiled() const { return m_isCompiled; }
		void use() const;//этой функцией будем включать шейдеры

		ShaderProgram() = delete;
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;

	private:
		bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID); //исходный код шейдера, тип шейдера. На выход идентификатор шейдера.

		bool m_isCompiled = false; //Показывает, нормально ли скомп. и слинковались все шейдеры.
		GLuint m_ID = 0; //идентификатор шейдера

	};
}