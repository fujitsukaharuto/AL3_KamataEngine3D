#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"

Matrix4x4 Multiply(Matrix4x4& matrix1, Matrix4x4& matrix2);

Matrix4x4 MakeIdentity4x4();

Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Matrix4x4 MakeRotateXMatrix(float rad);

Matrix4x4 MakeRotateYMatrix(float rad);

Matrix4x4 MakeRotateZMatrix(float rad);

Matrix4x4 MakeRotateXYZMatrix(const Vector3& rota);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

Vector3 Leap(const Vector3& v1, const Vector3& v2, float t);

Vector3 Sleap(const Vector3& v1, const Vector3& v2, float t);
