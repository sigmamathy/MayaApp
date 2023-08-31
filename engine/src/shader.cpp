#include "./private_control.hpp"
#include <fstream>

namespace Maya {

static unsigned create_shader(unsigned type, std::string const& src)
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
		std::cout << "Following error found in "
			<< (type == GL_VERTEX_SHADER ? "vertex shader:\n" : "fragment shader:\n") << buffer;
	}
#endif
	return shader;
}

Shader::Shader(std::string const& vertex, std::string const& fragment)
{
	shaderid = glCreateProgram();
	unsigned int vshader = create_shader(GL_VERTEX_SHADER, vertex);
	unsigned int fshader = create_shader(GL_FRAGMENT_SHADER, fragment);
	glAttachShader(shaderid, vshader);
	glAttachShader(shaderid, fshader);
	glLinkProgram(shaderid);
	glDeleteShader(vshader);
	glDeleteShader(fshader);
}

static void ReadFile(std::string const& file, std::string& output)
{
	std::ifstream ifs(file);
#if MAYA_DEBUG
	if (!ifs.is_open()) {
		std::cout << "Cannot open file \"" + file + "\"";
		return;
	}
#endif
	std::string line;
	while (getline(ifs, line))
		output += line + '\n';
}

Shader& PrivateControl::AssignShader_Temp(std::string const& name, std::string const& vertex, std::string const& fragment)
{
	auto* ptr = new Shader(vertex, fragment);
	shaders.emplace(name, ptr);
	return *ptr;
}

Shader& AssignShader(std::string const& name, std::string const& vertex_file, std::string const& fragment_file)
{
	std::string vsrc, fsrc;
	ReadFile(vertex_file, vsrc), ReadFile(fragment_file, fsrc);
	return PrivateControl::Instance().AssignShader_Temp(name, vsrc, fsrc);
}

Shader& GetShader(std::string const& name)
{
	return *PrivateControl::Instance().shaders.at(name);
}

Shader::~Shader()
{
	glDeleteProgram(shaderid);
}

void Shader::Bind()
{
	auto& ctrl = PrivateControl::Instance();
	if (this == ctrl.current_shader) return;
	ctrl.current_shader = this;
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

void Shader::Draw(VertexArray& vao)
{
	Bind();
	PrivateControl::Instance().ShaderDrawVAO(vao);
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