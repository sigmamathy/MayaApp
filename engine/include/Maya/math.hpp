#pragma once

#include "./core.hpp"

namespace Maya {

template<class Ty>
concept NumberType = std::is_arithmetic_v<Ty>;

// A class template for various type of matrices.
// std::uint8_t is used for rows and columns since dimensions larger than 255x255 are not intented to be used.
template<NumberType Ty, std::uint8_t R, std::uint8_t C>
class Matrix
{
public:
	// Matrix size constant
	using ElementType = Ty;
	static constexpr std::uint8_t Row = R, Column = C;
	static constexpr std::uint16_t Size = R * C;
	// Rows and column should never be zero.
	static_assert(R != 0u && C != 0u);

	// Value uninitialized constructor
	constexpr Matrix() = default;

	// Identity matrix multiplied by some value
	// @param value: the scaler value of the identity matrix
	explicit constexpr Matrix(Ty value)
	{
		for (std::uint8_t j = 0u; j < Column; j++)
			for (std::uint8_t i = 0u; i < Row; i++)
				if (i == j) Get(i, j) = value;
				else Get(i, j) = Ty(0);
	}

	// Construct with multple elements (no transpose)
	// @param args: array of components, needs to be exactly same size as R * C
	template<class... Tys> requires (sizeof...(Tys) == Size && (std::is_convertible_v<Tys, Ty> && ...))
		constexpr Matrix(Tys... args) : components{ Ty(args)... } {}

	// A copy constructor between matrices (allows different type)
	template<class Ty2>
	constexpr Matrix(Matrix<Ty2, R, C> const& mat)
	{
		for (std::uint16_t i = 0u; i < Size; i++)
			components[i] = Ty(mat[i]);
	}

	// Directly access element via indexing
	constexpr Ty& operator[](std::uint16_t index)
	{
		return components[index];
	}

	// Directly access element via indexing
	constexpr Ty const& operator[](std::uint16_t index) const
	{
		return components[index];
	}

	// Access element by specifying rows and columns
	constexpr Ty& Get(std::uint8_t row, std::uint8_t col)
	{
		return components[row * Column + col];
	}

	// Access element by specifying rows and columns
	constexpr Ty const& Get(std::uint8_t row, std::uint8_t col) const
	{
		return components[row * Column + col];
	}

	// Direct access the element array
	constexpr std::array<Ty, std::size_t(Size)>& Data() {
		return components;
	}

	// Direct access the element array
	constexpr std::array<Ty, std::size_t(Size)> const& Data() const {
		return components;
	}

	// Overloads the += operator, same definition as operator+
	template<class Ty2>
	constexpr auto operator+=(Matrix<Ty2, R, C> const& mat)
	{
		for (std::uint16_t i = 0u; i < R * C; i++)
			components[i] += mat[i];
		return *this;
	}

	// Overloads the -= operator, same definition as operator-
	template<class Ty2>
	constexpr auto operator-=(Matrix<Ty2, R, C> const& mat)
	{
		for (std::uint16_t i = 0u; i < R * C; i++)
			components[i] += mat[i];
		return *this;
	}

	// Overloads the *= operator, same definition as operator*
	template<NumberType STy>
	constexpr auto operator*=(STy scale)
	{
		for (std::uint16_t i = 0u; i < R * C; i++)
			components[i] *= scale;
		return *this;
	}

private:
	std::array<Ty, std::size_t(Size)> components;
};

// The default method of printing matrix (for debug purpose)
// @param os: expects std::cout
template<class Ty, std::uint8_t R, std::uint8_t C>
std::ostream& operator<<(std::ostream& os, Matrix<Ty, R, C> const& mat)
{
	for (std::uint8_t i = 0u; i < R; i++)
	{
		os << mat.Get(i, 0);
		for (std::uint8_t j = 1u; j < C; j++)
			os << ", " << mat.Get(i, j);
		if (i < R - 1) os << ',';
		os << '\n';
	}
	return os;
}

// Check equality of two matrices, operator overloaded
template<class Ty1, class Ty2, std::uint8_t R, std::uint8_t C>
constexpr bool operator==(Matrix<Ty1, R, C> const& mat1, Matrix<Ty2, R, C> const& mat2)
{
	for (std::uint16_t i = 0u; i < R * C; i++)
		if (mat1[i] != mat2[i])
			return false;
	return true;
}

// Add two matrices, operator overloaded
template<class Ty1, class Ty2, std::uint8_t R, std::uint8_t C>
constexpr auto operator+(Matrix<Ty1, R, C> const& mat1, Matrix<Ty2, R, C> const& mat2)
{
	Matrix<decltype(mat1[0] + mat2[0]), R, C> res;
	for (std::uint16_t i = 0u; i < R * C; i++)
		res[i] = mat1[i] + mat2[i];
	return res;
}

// Subtract two matrices, operator overloaded
template<class Ty1, class Ty2, std::uint8_t R, std::uint8_t C>
constexpr auto operator-(Matrix<Ty1, R, C> const& mat1, Matrix<Ty2, R, C> const& mat2)
{
	Matrix<decltype(mat1[0] - mat2[0]), R, C> res;
	for (std::uint16_t i = 0u; i < R * C; i++)
		res[i] = mat1[i] - mat2[i];
	return res;
}

// Negate matrix, operator overloaded
template<class Ty, std::uint8_t R, std::uint8_t C>
constexpr auto operator-(Matrix<Ty, R, C> const& mat)
{
	Matrix<Ty, R, C> res;
	for (std::uint16_t i = 0u; i < R * C; i++)
		res[i] = -mat[i];
	return res;
}

// Scaler multiplication of matrix, operator overloaded
template<class Ty, NumberType STy, std::uint8_t R, std::uint8_t C>
constexpr auto operator*(Matrix<Ty, R, C> const& mat, STy scale)
{
	Matrix<decltype(mat[0] * scale), R, C> res;
	for (std::uint16_t i = 0u; i < R * C; i++)
		res[i] = mat[i] * scale;
	return res;
}

// Scaler multiplication of matrix, operator overloaded
template<class Ty, NumberType STy, std::uint8_t R, std::uint8_t C>
constexpr auto operator*(STy scale, Matrix<Ty, R, C> const& mat)
{
	Matrix<decltype(scale* mat[0]), R, C> res;
	for (std::uint16_t i = 0u; i < R * C; i++)
		res[i] = scale * mat[i];
	return res;
}

// Scaler multiplication of matrix with division, operator overloaded
template<class Ty, NumberType STy, std::uint8_t R, std::uint8_t C>
constexpr auto operator/(Matrix<Ty, R, C> const& mat, STy scale)
{
	Matrix<decltype(mat[0] / scale), R, C> res;
	for (std::uint16_t i = 0u; i < R * C; i++)
		res[i] = mat[i] / scale;
	return res;
}

// Matrix multiplication, operator overloaded
// @param mat1: first matrix (the number of columns needs to be exactly the same as the number of rows of mat2)
// @param mat2: second matrix (same condition)
template<class Ty1, class Ty2, std::uint8_t R1, std::uint8_t R2C1, std::uint8_t C2>
constexpr auto operator*(Matrix<Ty1, R1, R2C1> const& mat1, Matrix<Ty2, R2C1, C2> const& mat2)
{
	Matrix<decltype(mat1[0] * mat2[0]), R1, C2> res(0);
	for (std::uint8_t i = 0u; i < R1; i++)
		for (std::uint8_t j = 0u; j < C2; j++)
			for (std::uint8_t k = 0u; k < R2C1; k++)
				res.Get(i, j) += mat1.Get(i, k) * mat2.Get(k, j);
	return res;
}

// Check if the matrix is a zero matrix
template<class Ty, std::uint8_t R, std::uint8_t C>
constexpr bool IsZero(Matrix<Ty, R, C> const& mat)
{
	for (std::uint16_t i = 0u; i < R * C; i++)
		if (mat[i] != Ty(0))
			return false;
	return true;
}

// Hadmard product of matrices
template<class Ty1, class Ty2, std::uint8_t R, std::uint8_t C>
constexpr auto Hadamard(Matrix<Ty1, R, C> const& mat1, Matrix<Ty2, R, C> const& mat2)
{
	Matrix<decltype(mat1[0] * mat2[0]), R, C> res;
	for (std::uint16_t i = 0u; i < R * C; i++)
		res[i] = mat1[i] * mat2[i];
	return res;
}

// Transpose of matrix
template<class Ty, std::uint8_t R, std::uint8_t C>
constexpr auto Transpose(Matrix<Ty, R, C> const& mat)
{
	Matrix<Ty, C, R> res;
	for (std::uint8_t j = 0u; j < R; j++)
		for (std::uint8_t i = 0u; i < C; i++)
			res.Get(i, j) = mat.Get(j, i);
	return res;
}

// Integer type matrices
using Imat2x2 = Matrix<int, 2, 2>;
using Imat2x3 = Matrix<int, 2, 3>;
using Imat2x4 = Matrix<int, 2, 4>;
using Imat3x2 = Matrix<int, 3, 2>;
using Imat3x3 = Matrix<int, 3, 3>;
using Imat3x4 = Matrix<int, 3, 4>;
using Imat4x2 = Matrix<int, 4, 2>;
using Imat4x3 = Matrix<int, 4, 3>;
using Imat4x4 = Matrix<int, 4, 4>;

// Unsigned integer type matrices
using Umat2x2 = Matrix<unsigned int, 2, 2>;
using Umat2x3 = Matrix<unsigned int, 2, 3>;
using Umat2x4 = Matrix<unsigned int, 2, 4>;
using Umat3x2 = Matrix<unsigned int, 3, 2>;
using Umat3x3 = Matrix<unsigned int, 3, 3>;
using Umat3x4 = Matrix<unsigned int, 3, 4>;
using Umat4x2 = Matrix<unsigned int, 4, 2>;
using Umat4x3 = Matrix<unsigned int, 4, 3>;
using Umat4x4 = Matrix<unsigned int, 4, 4>;

// Float type matrices
using Fmat2x2 = Matrix<float, 2, 2>;
using Fmat2x3 = Matrix<float, 2, 3>;
using Fmat2x4 = Matrix<float, 2, 4>;
using Fmat3x2 = Matrix<float, 3, 2>;
using Fmat3x3 = Matrix<float, 3, 3>;
using Fmat3x4 = Matrix<float, 3, 4>;
using Fmat4x2 = Matrix<float, 4, 2>;
using Fmat4x3 = Matrix<float, 4, 3>;
using Fmat4x4 = Matrix<float, 4, 4>;

// Integer type matrices (square)
using Imat2 = Imat2x2;
using Imat3 = Imat3x3;
using Imat4 = Imat4x4;

// Unsigned integer type matrices (square)
using Umat2 = Umat2x2;
using Umat3 = Umat3x3;
using Umat4 = Umat4x4;

// Float type matrices (square)
using Fmat2 = Fmat2x2;
using Fmat3 = Fmat3x3;
using Fmat4 = Fmat4x4;

// A class template for various type of vectors, inherit from Matrix
// std::uint8_t is used for vector size since dimensions larger than 255 are not intented to be used.
template<NumberType Ty, std::uint8_t Sz>
class Vector : public Matrix<Ty, Sz, 1>
{
public:
	// Inherit super functions
	using ElementType = Ty;
	static constexpr std::uint8_t Row = Sz, Column = 1;
	static constexpr std::uint8_t Size = Sz;
	using Matrix<Ty, Sz, 1>::Matrix;
	using Matrix<Ty, Sz, 1>::operator[];

	// Value uninitialized constructor
	constexpr Vector() = default;

	// Fills all component with certain value
	// @param value: the initialized value
	constexpr Vector(Ty value)
	{
		for (std::uint8_t i = 0u; i < Size; i++)
			Get(i) = value;
	}

	// Copy constructor of matrices
	template<class Ty2>
	constexpr Vector(Matrix<Ty2, Sz, 1> const& mat)
	{
		for (std::uint8_t i = 0u; i < Size; i++)
			Get(i) = (Ty)mat.Get(i, 0);
	}

	// Directly access element via indexing (overrided matrix method)
	constexpr Ty& Get(std::uint8_t index)
	{
		return (*this)[index];
	}

	// Directly access element via indexing (overrided matrix method)
	constexpr Ty const& Get(std::uint8_t index) const
	{
		return (*this)[index];
	}

	// Access multiple elements at the same time and extract it as a vector
	// @param indices: a list of index that indicates elements position inside the vector
	template<class... Tys>
		requires (sizeof...(Tys) > 1
	&& (std::is_convertible_v<Tys, std::uint8_t> && ...))
		constexpr auto Get(Tys... indices) const
	{
		Vector<Ty, sizeof...(Tys)> res;
		std::array<std::uint8_t, sizeof...(Tys)> const inds
			= { static_cast<std::uint8_t>(indices)... };
		for (std::uint8_t i = 0u; i < sizeof...(Tys); i++)
			res[i] = Get(inds[i]);
		return res;
	}

	// Overloads the += operator
	template<class Ty2>
	constexpr auto operator+=(Vector<Ty2, Sz> const& vec)
	{
		for (std::uint8_t i = 0u; i < Size; i++)
			Get(i) += Ty(vec[i]);
		return *this;
	}

	// Overloads the -= operator
	template<class Ty2>
	constexpr auto operator-=(Vector<Ty2, Sz> const& vec)
	{
		for (std::uint8_t i = 0u; i < Size; i++)
			Get(i) -= Ty(vec[i]);
		return *this;
	}

	// Overloads the *= operator
	template<NumberType STy>
	constexpr auto operator*=(STy scale)
	{
		for (std::uint8_t i = 0u; i < Size; i++)
			Get(i) *= Ty(scale);
		return *this;
	}

	// Calculate the norm of vector (aka magnitude)
	constexpr std::common_type_t<Ty, float> Norm() const
	{
		float sum = 0.0f;
		for (std::uint8_t i = 0u; i < Size; i++)
			sum += Ty(Get(i) * Get(i));
		return std::sqrt(sum);
	}
};

// The default method of printing vector (for debug purpose)
// @param os: expects std::cout
template<class Ty, std::uint8_t Sz>
std::ostream& operator<<(std::ostream& os, Vector<Ty, Sz> const& vec)
{
	os << '[' << vec[0];
	for (std::uint8_t i = 1u; i < Sz; i++)
		os << ", " << vec[i];
	return os << ']';
}

// Add two vectors, operator overloaded
template<class Ty1, class Ty2, std::uint8_t Sz>
constexpr auto operator+(Vector<Ty1, Sz> const& vec1, Vector<Ty2, Sz> const& vec2)
{
	Vector<decltype(vec1[0] + vec2[0]), Sz> res;
	for (std::uint8_t i = 0u; i < Sz; i++)
		res[i] = vec1[i] + vec2[i];
	return res;
}

// Subtract two vectors, operator overloaded
template<class Ty1, class Ty2, std::uint8_t Sz>
constexpr auto operator-(Vector<Ty1, Sz> const& vec1, Vector<Ty2, Sz> const& vec2)
{
	Vector<decltype(vec1[0] - vec2[0]), Sz> res;
	for (std::uint8_t i = 0u; i < Sz; i++)
		res[i] = vec1[i] - vec2[i];
	return res;
}

// Negate vector, operator overloaded
template<class Ty, std::uint8_t Sz>
constexpr auto operator-(Vector<Ty, Sz> const& vec)
{
	Vector<Ty, Sz> res;
	for (std::uint8_t i = 0u; i < Sz; i++)
		res[i] = -vec[i];
	return res;
}

// Scaler multiplication of vector, operator overloaded
template<class Ty, NumberType STy, std::uint8_t Sz>
constexpr auto operator*(Vector<Ty, Sz> const& vec, STy scale)
{
	Vector<decltype(vec[0] * scale), Sz> res;
	for (std::uint8_t i = 0u; i < Sz; i++)
		res[i] = vec[i] * scale;
	return res;
}

// Scaler multiplication of vector, operator overloaded
template<class Ty, NumberType STy, std::uint8_t Sz>
constexpr auto operator*(STy scale, Vector<Ty, Sz> const& vec)
{
	return vec * scale;
}

// Scaler multiplication of vector with division, operator overloaded
template<class Ty, NumberType STy, std::uint8_t Sz>
constexpr auto operator/(Vector<Ty, Sz> const& vec, STy scale)
{
	Vector<decltype(vec[0] * scale), Sz> res;
	for (std::uint8_t i = 0u; i < Sz; i++)
		res[i] = vec[i] / scale;
	return res;
}

// Dot product of two vectors
template<class Ty1, class Ty2, std::uint8_t Sz>
constexpr auto Dot(Vector<Ty1, Sz> const& vec1, Vector<Ty2, Sz> const& vec2)
{
	decltype(vec1[0] * vec2[0]) res = 0;
	for (std::uint8_t i = 0u; i < Sz; i++)
		res += vec1[i] * vec2[i];
	return res;
}

// Cross product of two vectors (2 dimensional)
template<class Ty1, class Ty2>
constexpr auto Cross(Vector<Ty1, 2> const& vec1, Vector<Ty2, 2> const& vec2)
{
	return vec1[0] * vec2[1] - vec1[1] * vec2[0];
}

// Cross product of two vectors (3 dimensional)
template<class Ty1, class Ty2>
constexpr auto Cross(Vector<Ty1, 3> const& vec1, Vector<Ty2, 3> const& vec2)
{
	return Vector<decltype(vec1[0] * vec2[0]), 3>
	{
		vec1[1] * vec2[2] - vec1[2] * vec2[1],
			vec1[2] * vec2[0] - vec1[0] * vec2[2],
			vec1[0] * vec2[1] - vec1[1] * vec2[0]
	};
}

// Hadamard product of vectors
template<class Ty1, class Ty2, std::uint8_t Sz>
constexpr auto Hadamard(Vector<Ty1, Sz> const& vec1, Vector<Ty2, Sz> const& vec2)
{
	Vector<decltype(vec1[0] * vec2[0]), Sz> res;
	for (std::uint8_t i = 0u; i < Sz; i++)
		res[i] = vec1[i] * vec2[i];
	return res;
}

// Normalize a vector
template<class Ty, std::uint8_t Sz>
constexpr auto Normalize(Vector<Ty, Sz> const& vec)
{
	using common_type = std::common_type_t<Ty, float>;
	Ty sum = 0;
	for (std::uint8_t i = 0u; i < Sz; i++)
		sum += vec[i] * vec[i];
	auto invsqrt = (common_type)1.0f / std::sqrt(float(sum));
	Vector<common_type, Sz> res;
	for (std::uint8_t i = 0u; i < Sz; i++)
		res[i] = vec[i] * invsqrt;
	return res;
}

// Integer type vectors
using Ivec2 = Vector<int, 2>;
using Ivec3 = Vector<int, 3>;
using Ivec4 = Vector<int, 4>;

// Unsigned integer type vectors
using Uvec2 = Vector<unsigned int, 2>;
using Uvec3 = Vector<unsigned int, 3>;
using Uvec4 = Vector<unsigned int, 4>;

// Float type vectors
using Fvec2 = Vector<float, 2>;
using Fvec3 = Vector<float, 3>;
using Fvec4 = Vector<float, 4>;

}