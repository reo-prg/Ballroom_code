#pragma once
#include "VectorInterface.h"

struct Matrix {
	float m[3][3];
};

Matrix IdentityMat();

Matrix TranslateMat(float x, float y);

Matrix RotateMat(float angle);

Matrix ScaleMat(float x, float y);

Matrix MultipleMat(const Matrix& lmat, const Matrix& rmat);

template<typename T>
Vector2<T> MultipleVec(const Matrix& mat, const Vector2<T>& vec)
{
	Vector2<T> ret = {};
	ret.x = mat.m[0][0] * static_cast<float>(vec.x) + mat.m[0][1] * static_cast<float>(vec.y) + mat.m[0][2];
	ret.y = mat.m[1][0] * static_cast<float>(vec.x) + mat.m[1][1] * static_cast<float>(vec.y) + mat.m[1][2];
	return ret;
}
