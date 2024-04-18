#pragma once
#include <cmath>

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	Vector3& operator+=(const Vector3& v){
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
	Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }

	Vector3 operator*(float k) const { return Vector3(x * k, y * k, z * k); }
	Vector3 operator/(float k) const { return Vector3(x / k, y / k, z / k); }
	float operator*(const Vector3& v) const { return (x * v.x) + (y * v.y) + (z * v.z); }

	// Function

	float Lenght() const { return std::sqrtf((*this) * (*this)); }
	Vector3 Normalize() const {
		const float leng = Lenght();
		return leng == 0 ? *this : *this / leng;
	}

};