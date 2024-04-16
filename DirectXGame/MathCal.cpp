#include "MathCal.h"
#include <cmath>

Matrix4x4 Multiply(Matrix4x4& matrix1, Matrix4x4& matrix2) {
	Matrix4x4 result{};

	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			for (int i = 0; i < 4; i++) {
				result.m[row][column] += matrix1.m[row][i] * matrix2.m[i][column];
			}
		}
	}
	return result;
}

Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 result{};

	for (int i = 0; i < 4; i++) {
		result.m[i][i] = 1;
	}
	return result;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result = MakeIdentity4x4();

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;

	return result;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result = MakeIdentity4x4();

	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;

	return result;
}

Matrix4x4 MakeRotateXMatrix(float rad) {
	Matrix4x4 result = MakeIdentity4x4();

	result.m[1][1] = std::cosf(rad);
	result.m[1][2] = std::sinf(rad);
	result.m[2][1] = -std::sinf(rad);
	result.m[2][2] = std::cosf(rad);

	return result;
}

Matrix4x4 MakeRotateYMatrix(float rad) {
	Matrix4x4 result = MakeIdentity4x4();

	result.m[0][0] = std::cosf(rad);
	result.m[0][2] = -std::sinf(rad);
	result.m[2][0] = std::sinf(rad);
	result.m[2][2] = std::cosf(rad);

	return result;
}

Matrix4x4 MakeRotateZMatrix(float rad) {
	Matrix4x4 result = MakeIdentity4x4();

	result.m[0][0] = std::cosf(rad);
	result.m[0][1] = std::sinf(rad);
	result.m[1][0] = -std::sinf(rad);
	result.m[1][1] = std::cosf(rad);

	return result;
}

Matrix4x4 MakeRotateXYZMatrix(const Vector3& rota) {

	Matrix4x4 result = MakeIdentity4x4();
	Matrix4x4 rotaxM = MakeRotateXMatrix(rota.x);
	Matrix4x4 rotayM = MakeRotateYMatrix(rota.y);
	Matrix4x4 rotazM = MakeRotateZMatrix(rota.z);
	result = Multiply(rotayM, rotazM);
	result = Multiply(rotaxM, result);

	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate) {
	Matrix4x4 result = MakeIdentity4x4();
	Matrix4x4 S = MakeScaleMatrix(scale);
	Matrix4x4 R = MakeRotateXYZMatrix(rot);
	Matrix4x4 T = MakeTranslateMatrix(translate);

	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			result.m[row][col] = S.m[row][row] * R.m[row][col];
		}
	}
	result.m[3][0] = T.m[3][0];
	result.m[3][1] = T.m[3][1];
	result.m[3][2] = T.m[3][2];

	return result;
}

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	
	Vector3 result{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]};
		
		return result;
}
