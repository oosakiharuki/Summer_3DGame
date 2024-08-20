#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>
//struct AABB {
//	Vector3 min;
//	Vector3 max;
//};
#pragma once
class MyMath {

public:
	Vector3 Add(const Vector3& v1, const Vector3& v2);
	Vector3 Subtract(const Vector3& v1, const Vector3& v2);
	Vector3 Normalize(const Vector3& v);

	Matrix4x4 MakeAffineMatrix(Vector3 scale_, Vector3 rotate_, Vector3 translate_);
	Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2);

	Matrix4x4 Inverse(const Matrix4x4& m);

	Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

};