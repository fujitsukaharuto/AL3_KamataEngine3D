#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <vector>

Matrix4x4 Multiply(const Matrix4x4& matrix1, const Matrix4x4& matrix2);

Matrix4x4 MakeIdentity4x4();

Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Matrix4x4 MakeRotateXMatrix(float rad);

Matrix4x4 MakeRotateYMatrix(float rad);

Matrix4x4 MakeRotateZMatrix(float rad);

Matrix4x4 MakeRotateXYZMatrix(const Vector3& rota);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

Matrix4x4 Inverse(const Matrix4x4& matrix);

float Lerp(float v1, float v2, float t);

Vector3 Leap(const Vector3& v1, const Vector3& v2, float t);

Vector3 Sleap(const Vector3& v1, const Vector3& v2, float t);

Vector3 CatmullRom(const std::vector<Vector3>& control, float t);

Vector3 CatmullRomStartPoint(const std::vector<Vector3>& control, float t);

Vector3 CatmullRomEndPoint(const std::vector<Vector3>& control, float t);

Vector3 Multiply(const Vector3& v, const Matrix4x4& m);

Matrix4x4 Sub(const Matrix4x4& m1, const Matrix4x4& m2);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
