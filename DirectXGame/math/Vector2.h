#pragma once
#include <cmath>

/// <summary>
/// 2次元ベクトル
/// </summary>
struct Vector2 final {
	float x;
	float y;

	Vector2 operator-(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
	Vector2 operator*(float k) const { return Vector2(x * k, y * k); }
	float operator*(const Vector2& v) const { return (x * v.x) + (y * v.y); }

	float Lenght() const { return std::sqrtf((*this) * (*this)); }
};
