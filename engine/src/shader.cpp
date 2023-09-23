#include <Maya/shader.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>

namespace Maya {

static unsigned CreateShader(unsigned type, std::string const& src)
{
	auto shader = glCreateShader(type);
	const char* cstr = src.c_str();
	glShaderSource(shader, 1, &cstr, nullptr);
	glCompileShader(shader);
#if MAYA_DEBUG
	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		std::string buffer;
		buffer.resize(512);
		glGetShaderInfoLog(shader, 512, NULL, &buffer[0]);
		MAYA_LOG("Warning (Shader::Shader): OpenGL reports the following error about the "
			<< (type == GL_VERTEX_SHADER ? "vertex shader:\n" : "fragment shader:\n") << buffer);
	}
#endif
	return shader;
}

static std::string ReadFile(std::string const& file)
{
	std::ifstream ifs(file);
#if MAYA_DEBUG
	if (!ifs.is_open()) {
		MAYA_LOG("Error (Shader::Shader): the file \"" << file << "\" cannot be found");
		return "";
	}
#endif
	std::string output, line;
	while (getline(ifs, line))
		output += line + '\n';
	return output;
}

static Shader* current_shader = nullptr;

Shader::Shader(std::string const& vertex, std::string const& fragment, bool is_file_name)
{
	shaderid = glCreateProgram();

	unsigned int vshader = CreateShader(GL_VERTEX_SHADER, is_file_name ? ReadFile(vertex) : vertex);
	unsigned int fshader = CreateShader(GL_FRAGMENT_SHADER, is_file_name ? ReadFile(fragment) : fragment);
	glAttachShader(shaderid, vshader);
	glAttachShader(shaderid, fshader);
	glLinkProgram(shaderid);
	glDeleteShader(vshader);
	glDeleteShader(fshader);
}

Shader::~Shader()
{
	glDeleteProgram(shaderid);
}

void Shader::Bind()
{
	if (this == current_shader) return;
	current_shader = this;
	glUseProgram(shaderid);
}

int Shader::GetUniformLocation(std::string const& name)
{
	if (uniform_location_cache.count(name))
		return uniform_location_cache[name];
	int location = glGetUniformLocation(shaderid, name.c_str());
	uniform_location_cache[name] = location;
	return location;
}

#define MAYA_UNIFORM_VECTOR_FUNCTION(ty, sz, fn)\
	template<> void Shader::SetUniform(std::string const& name, Vector<ty, sz> const& vec)\
	{ Bind(); int location = GetUniformLocation(name); fn(location, 1, &vec[0]); }

MAYA_UNIFORM_VECTOR_FUNCTION(int, 1, glUniform1iv)
MAYA_UNIFORM_VECTOR_FUNCTION(int, 2, glUniform2iv)
MAYA_UNIFORM_VECTOR_FUNCTION(int, 3, glUniform3iv)
MAYA_UNIFORM_VECTOR_FUNCTION(int, 4, glUniform4iv)

MAYA_UNIFORM_VECTOR_FUNCTION(unsigned, 1, glUniform1uiv)
MAYA_UNIFORM_VECTOR_FUNCTION(unsigned, 2, glUniform2uiv)
MAYA_UNIFORM_VECTOR_FUNCTION(unsigned, 3, glUniform3uiv)
MAYA_UNIFORM_VECTOR_FUNCTION(unsigned, 4, glUniform4uiv)

MAYA_UNIFORM_VECTOR_FUNCTION(float, 1, glUniform1fv)
MAYA_UNIFORM_VECTOR_FUNCTION(float, 2, glUniform2fv)
MAYA_UNIFORM_VECTOR_FUNCTION(float, 3, glUniform3fv)
MAYA_UNIFORM_VECTOR_FUNCTION(float, 4, glUniform4fv)

#define MAYA_UNIFORM_MATRIX_FUNCTION(r, c, fn)\
	template<> void Shader::SetUniform(std::string const& name, Matrix<float, r, c> const& mat)\
	{ Bind(); int location = GetUniformLocation(name); fn(location, 1, true, &mat[0]); }

MAYA_UNIFORM_MATRIX_FUNCTION(2, 2, glUniformMatrix2fv)
MAYA_UNIFORM_MATRIX_FUNCTION(2, 3, glUniformMatrix2x3fv)
MAYA_UNIFORM_MATRIX_FUNCTION(2, 4, glUniformMatrix2x4fv)

MAYA_UNIFORM_MATRIX_FUNCTION(3, 2, glUniformMatrix3x2fv)
MAYA_UNIFORM_MATRIX_FUNCTION(3, 3, glUniformMatrix3fv)
MAYA_UNIFORM_MATRIX_FUNCTION(3, 4, glUniformMatrix3x4fv)

MAYA_UNIFORM_MATRIX_FUNCTION(4, 2, glUniformMatrix4x2fv)
MAYA_UNIFORM_MATRIX_FUNCTION(4, 3, glUniformMatrix4x3fv)
MAYA_UNIFORM_MATRIX_FUNCTION(4, 4, glUniformMatrix4fv)

}