#pragma once

#include <array>
#include <cassert>
#include <cmath>

struct vec2
{
public:
	std::array<float, 2> elements {};

	vec2();
	vec2(const float x, const float y)
	{
		elements = { x, y };
	}
	
	float x() const { return elements[0]; }
	float y() const { return elements[1]; }
	float u() const { return elements[0]; }
	float v() const { return elements[1]; }
	float s() const { return elements[0]; }
	float t() const { return elements[1]; }

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
