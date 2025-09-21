#pragma once
#include "BMath.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Vector.h"
#include <cmath>
#include <complex>
#include <cstring>
namespace BMath 
{
struct Mat4
{
    float Data[16];
};


Vec4 Multiply(const Mat4& mat, const Vec4& vec);
Mat4 Multiply(const Mat4& mat1, const Mat4& mat2);

Mat4 operator*(const Mat4& mat1, const Mat4& mat2);

Vec4 operator*(const Mat4& mat, const Vec4& vec);


Mat4 Identity();
Mat4 Transpose(Mat4 mat);
Mat4 Inverse(Mat4 matrix); 
Mat4 EulerX(f32 angleRadians);
Mat4 EulerY(f32 angleRadians);
Mat4 EulerZ(f32 angleRadians);
Mat4 Rotate(float xAngleRadians = 0.0f, float yAngleRadians = 0.0f, float zAngleRadians = 0.0f); 
Mat4 Scale(float sx, float sy, float sz); 
Mat4 Translate(float tx, float ty, float tz);
Mat4 CreateTransform(const Vec3& position, 
                                 const Vec3& scale, 
                                 const Vec3& rotation = Vec3(0.0f, 0.0f, 0.0f));
Mat4 Ortho(float left, float right, float bot, float top, float zNear, float zFar); 
Mat4 Perspective(f32 fovRadians, f32 aspectRatio, f32 nearClip, f32 farClip);

Vec3 Forward(const Mat4& mat);

Vec3 Backward(const Mat4& mat);
Vec3 Right(const Mat4& mat);
Vec3 Left(const Mat4& mat);;
Vec3 Up(const Mat4& mat);;
Vec3 Down(const Mat4& mat);;
}
