#pragma once

#include <array>
#include <cassert>
#include <cmath>

struct vec3
{
public:
	std::array<float, 3> elements {};

	vec3();
	vec3(const float x, const float y, const float z)
	{
		elements = { x, y, z };
	}
	
	float x() const { return elements[0]; }
	float y() const { return elements[1]; }
	float z() const { return elements[2]; }
	float r() const { return elements[0]; }
	float g() const { return elements[1]; }
	float b() const { return elements[2]; }

	friend float dot(const vec2& lhs, const vec2& rhs)
	{
		return lhs.x() * rhs.x() + lhs.y() * rhs.y();
	}

	float length_squared() const
	{
		return x() * x() + y() * y();
	}

	float length() const
	{
		return std::sqrt(length_squared());
	}

	vec2 unit_vector() const
	{
		const vec2 result = (*this / length());
		return result;
	}

	float& operator[](const size_t index)
	{
		assert(index < 2);
		return elements[index];
	}

	vec2 operator-() const
	{
		const auto result = vec2(-x(), -y());
		return result;
	}

	friend vec2& operator+=(vec2& lhs, vec2 rhs)
	{
		lhs[0] += rhs[0];
		lhs[1] += rhs[1];
		return lhs;
	}

	friend vec2 operator+(vec2 lhs, const vec2& rhs)
	{
		lhs += rhs;
		return lhs;
	}

	friend vec2& operator-=(vec2& lhs, const vec2 rhs)
	{
		return lhs += -rhs;
	}

	friend vec2 operator-(vec2 lhs, const vec2& rhs)
	{
		return lhs -= rhs;
	}

	friend vec2& operator*=(vec2& lhs, const float rhs)
	{
		lhs[0] *= rhs;
		lhs[1] *= rhs;
		return lhs;
	}

	friend vec2 operator*(vec2 lhs, const float rhs)
	{
		lhs *= rhs;
		return lhs;
	}

	friend vec2 operator*(const float lhs, const vec2 rhs)
	{
		return rhs * lhs;
	}

	friend vec2& operator/=(vec2& lhs, const float rhs)
	{
		lhs *= 1.0f / rhs;
		return lhs;
	}

	friend vec2 operator/(const vec2 lhs, const float rhs)
	{
		return lhs * (1.0f / rhs);
	}
};
