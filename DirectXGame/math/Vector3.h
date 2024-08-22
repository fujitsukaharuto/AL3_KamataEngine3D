#pragma once
#include <cmath>

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;


	/// <summary>
	/// 比較演算子
	/// </summary>
	
	bool operator==(const Vector3& v) const { return (x == v.x && y == v.y && z == v.z); }


	/// <summary>
	/// 加算
	/// </summary>

	Vector3& operator+=(const Vector3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
	
	
	/// <summary>
	/// 除算
	/// </summary>
	
	Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }


	/// <summary>
	/// 乗算・除算
	/// </summary>

	Vector3 operator*(float k) const { return Vector3(x * k, y * k, z * k); }
	Vector3 operator/(float k) const { return Vector3(x / k, y / k, z / k); }
	
	
	/// <summary>
	/// 内積
	/// </summary>
	
	float operator*(const Vector3& v) const { return (x * v.x) + (y * v.y) + (z * v.z); }


	// Function

	float Lenght() const { return std::sqrtf((*this) * (*this)); }
	Vector3 Normalize() const {
		const float leng = Lenght();
		return leng == 0 ? *this : *this / leng;
	}
	Vector3 Cross(const Vector3& v) const { return Vector3(
		this->y * v.z - this->z * v.y,
		this->z * v.x - this->x * v.z,
		this->x * v.y - this->y * v.x);
	}
	float Dot(const Vector3& v) const {
		return this->x * v.x + this->y * v.y + this->z * v.z;
	}

};