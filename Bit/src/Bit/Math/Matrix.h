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

Mat4 Mat4Identity();
Vec4 Mat4Multiply(const Mat4& mat, const Vec4& vec);
Mat4 Mat4Multiply(const Mat4& mat1, const Mat4& mat2);

Mat4 operator*(const Mat4& mat1, const Mat4& mat2);

Vec4 operator*(const Mat4& mat, const Vec4& vec);


Mat4 Mat4Identity();
Mat4 Mat4Transpose(Mat4 mat);
Mat4 Mat4Inverse(Mat4 matrix); 
Mat4 Mat4EulerX(f32 angleRadians);
Mat4 Mat4EulerY(f32 angleRadians);
Mat4 Mat4EulerZ(f32 angleRadians);
Mat4 Mat4Rotate(float xAngleRadians = 0.0f, float yAngleRadians = 0.0f, float zAngleRadians = 0.0f); 
Mat4 Mat4Scale(float sx, float sy, float sz); 
Mat4 Mat4Translate(float tx, float ty, float tz);
Mat4 Mat4CreateTransform(const Vec3& position, 
                                 const Vec3& scale, 
                                 const Vec3& rotation = Vec3(0.0f, 0.0f, 0.0f));
Mat4 Mat4Ortho(float left, float right, float bot, float top, float zNear, float zFar); 
Mat4 Mat4Perspective(f32 fovRadians, f32 aspectRatio, f32 nearClip, f32 farClip);

Vec3 Mat4Forward(const Mat4& mat);

Vec3 Mat4Backward(const Mat4& mat);
Vec3 Mat4Right(const Mat4& mat);
Vec3 Mat4Left(const Mat4& mat);;
Vec3 Mat4Up(const Mat4& mat);;
Vec3 Mat4Down(const Mat4& mat);;
}
