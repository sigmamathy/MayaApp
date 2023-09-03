#pragma once

#include "./vertex_array.hpp"

namespace Maya {

class Shader
{
public:
	Shader(std::string const& vertex, std::string const& fragment, bool is_file_name = true);

	// Set a uniform vector
	// @param name: the name of the uniform
	// @param vec: Type could be int, unsigned int or float, size could be 1 to 4
	template<class Ty, std::uint8_t Sz>
	void SetUniform(std::string const& name, Vector<Ty, Sz> const& vec);

	// Set a uniform matrix
	// @param name: the name of the uniform
	// @param mat: Type must be float, rows and columns could be 1 to 4
	template<std::uint8_t R, std::uint8_t C>
	void SetUniform(std::string const& name, Matrix<float, R, C> const& mat);

	// Set uniform, it follows the implementation of uniform vectors
	// @param name: the name of the uniform
	// @param args: sizeof...(args) must be 1 to 4
	template<NumberType... Tys>
	void SetUniform(std::string const& name, Tys... args)
	{
		Vector<std::common_type_t<Tys...>, sizeof...(Tys)> vec = { args... };
		SetUniform(name, vec);
	}

	// Draw a vertex array
	void Draw(VertexArray& vao);

	// Draw a vertex array
	void Draw(std::string const& vao);

private:
	unsigned int shaderid;
	std::unordered_map<std::string, int> uniform_location_cache;

private:
	Shader(Shader const&) = delete;
	Shader& operator=(Shader const&) = delete;
	~Shader();
	void Bind();
	int GetUniformLocation(std::string const& name);
	friend class PrivateControl;

};

}