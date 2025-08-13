#pragma once
#include "BMath.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Vector.h"
#include <cmath>
#include <complex>
#include <cstring>
namespace BMath 
{
class Mat4
{
public:
    float Data[16];

public:
    Mat4()
        : Data{
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        }
    {
    }



    static Vec4 Multiply(const Mat4& mat, const Vec4& vec)
    {
        Vec4 result{0.0f, 0.0f, 0.0f, 0.0f};
        result.x = mat.Data[0] * vec.x + mat.Data[4] * vec.y + mat.Data[8]  * vec.z + mat.Data[12] * vec.w;
        result.y = mat.Data[1] * vec.x + mat.Data[5] * vec.y + mat.Data[9]  * vec.z + mat.Data[13] * vec.w;
        result.z = mat.Data[2] * vec.x + mat.Data[6] * vec.y + mat.Data[10] * vec.z + mat.Data[14] * vec.w;
        result.w = mat.Data[3] * vec.x + mat.Data[7] * vec.y + mat.Data[11] * vec.z + mat.Data[15] * vec.w;
        return result;
    }

    static Mat4 Multiply(const Mat4& mat1, const Mat4& mat2)
    {
        Mat4 result;

        const float* a = mat1.Data;
        const float* b = mat2.Data;
        float* r = result.Data;

        for (int col = 0; col < 4; ++col)
        {
            for (int row = 0; row < 4; ++row)
            {
                float sum = a[row + 0*4] * b[0 + col*4] +
                            a[row + 1*4] * b[1 + col*4] +
                            a[row + 2*4] * b[2 + col*4] +
                            a[row + 3*4] * b[3 + col*4]; 
                
                r[row + col*4] = sum;
            }
        }

        return result;
    }
    Mat4 operator*(const Mat4& mat)
    {
        return Multiply(*this, mat);
    }

    static Mat4 Identity()
    {
        return Mat4();
    }
    static Mat4 Transpose(Mat4 mat)
    {
        Mat4 transposedMat;

        transposedMat.Data[0] =  mat.Data[0];
        transposedMat.Data[1] =  mat.Data[4];
        transposedMat.Data[2] =  mat.Data[8];
        transposedMat.Data[3] =  mat.Data[12];
        transposedMat.Data[4] =  mat.Data[1];
        transposedMat.Data[5] =  mat.Data[5];
        transposedMat.Data[6] =  mat.Data[9];
        transposedMat.Data[7] =  mat.Data[13];
        transposedMat.Data[8] =  mat.Data[2];
        transposedMat.Data[9] =  mat.Data[6];
        transposedMat.Data[10] = mat.Data[10];
        transposedMat.Data[11] = mat.Data[14];
        transposedMat.Data[12] = mat.Data[3];
        transposedMat.Data[13] = mat.Data[7];
        transposedMat.Data[14] = mat.Data[11];
        transposedMat.Data[15] = mat.Data[15];
        return transposedMat;
    }
    static Mat4 Inverse(Mat4 matrix) {
        const f32* m = matrix.Data;

        //Calculate all the minors with det of 3x3 mat
        f32 t0 = m[10] * m[15];
        f32 t1 = m[14] * m[11];
        f32 t2 = m[6] * m[15];
        f32 t3 = m[14] * m[7];
        f32 t4 = m[6] * m[11];
        f32 t5 = m[10] * m[7];
        f32 t6 = m[2] * m[15];
        f32 t7 = m[14] * m[3];
        f32 t8 = m[2] * m[11];
        f32 t9 = m[10] * m[3];
        f32 t10 = m[2] * m[7];
        f32 t11 = m[6] * m[3];
        f32 t12 = m[8] * m[13];
        f32 t13 = m[12] * m[9];
        f32 t14 = m[4] * m[13];
        f32 t15 = m[12] * m[5];
        f32 t16 = m[4] * m[9];
        f32 t17 = m[8] * m[5];
        f32 t18 = m[0] * m[13];
        f32 t19 = m[12] * m[1];
        f32 t20 = m[0] * m[9];
        f32 t21 = m[8] * m[1];
        f32 t22 = m[0] * m[5];
        f32 t23 = m[4] * m[1];

        Mat4 outMatrix;
        f32* o = outMatrix.Data;

        //calculate cofactors for first row 
        o[0] = (t0 * m[5] + t3 * m[9] + t4 * m[13]) -
               (t1 * m[5] + t2 * m[9] + t5 * m[13]);
        o[1] = (t1 * m[1] + t6 * m[9] + t9 * m[13]) -
               (t0 * m[1] + t7 * m[9] + t8 * m[13]);
        o[2] = (t2 * m[1] + t7 * m[5] + t10 * m[13]) -
               (t3 * m[1] + t6 * m[5] + t11 * m[13]);
        o[3] = (t5 * m[1] + t8 * m[5] + t11 * m[9]) -
               (t4 * m[1] + t9 * m[5] + t10 * m[9]);

        // calculate the det of first row cofactors to make sure the matrix is invertable
        f32 d = 1.0f / (m[0] * o[0] + m[4] * o[1] + m[8] * o[2] + m[12] * o[3]);

        // if its singular matrix return Identity
        if (babs(d) < 1e-6f) {
            return Mat4();
        }

        // calculate inverse for all components of the matrix
        o[0] = d * o[0];
        o[1] = d * o[1];
        o[2] = d * o[2];
        o[3] = d * o[3];
        o[4] = d * ((t1 * m[4] + t2 * m[8] + t5 * m[12]) -
                    (t0 * m[4] + t3 * m[8] + t4 * m[12]));
        o[5] = d * ((t0 * m[0] + t7 * m[8] + t8 * m[12]) -
                    (t1 * m[0] + t6 * m[8] + t9 * m[12]));
        o[6] = d * ((t3 * m[0] + t6 * m[4] + t11 * m[12]) -
                    (t2 * m[0] + t7 * m[4] + t10 * m[12]));
        o[7] = d * ((t4 * m[0] + t9 * m[4] + t10 * m[8]) -
                    (t5 * m[0] + t8 * m[4] + t11 * m[8]));
        o[8] = d * ((t12 * m[7] + t15 * m[11] + t16 * m[15]) -
                    (t13 * m[7] + t14 * m[11] + t17 * m[15]));
        o[9] = d * ((t13 * m[3] + t18 * m[11] + t21 * m[15]) -
                    (t12 * m[3] + t19 * m[11] + t20 * m[15]));
        o[10] = d * ((t14 * m[3] + t19 * m[7] + t22 * m[15]) -
                     (t15 * m[3] + t18 * m[7] + t23 * m[15]));
        o[11] = d * ((t17 * m[3] + t20 * m[7] + t23 * m[11]) -
                     (t16 * m[3] + t21 * m[7] + t22 * m[11]));
        o[12] = d * ((t14 * m[10] + t17 * m[14] + t13 * m[6]) -
                     (t16 * m[14] + t12 * m[6] + t15 * m[10]));
        o[13] = d * ((t20 * m[14] + t12 * m[2] + t19 * m[10]) -
                     (t18 * m[10] + t21 * m[14] + t13 * m[2]));
        o[14] = d * ((t18 * m[6] + t23 * m[14] + t15 * m[2]) -
                     (t22 * m[14] + t14 * m[2] + t19 * m[6]));
        o[15] = d * ((t22 * m[10] + t16 * m[2] + t21 * m[6]) -
                     (t20 * m[6] + t23 * m[10] + t17 * m[2]));

        return outMatrix;
    }
    static Mat4 EulerX(f32 angleRadians)
    {
        f32 c = cosf(angleRadians);
        f32 s = sinf(angleRadians);
        Mat4 outMatrix{}; 
        outMatrix.Data[5]  =  c;
        outMatrix.Data[6]  =  s;
        outMatrix.Data[9]  = -s;
        outMatrix.Data[10] =  c; 
        return outMatrix;
    }
    static Mat4 EulerY(f32 angleRadians)
    {
        f32 c = cosf(angleRadians);
        f32 s = sinf(angleRadians);
        Mat4 outMatrix{}; 
        outMatrix.Data[0]  =  c;
        outMatrix.Data[2]  = -s;
        outMatrix.Data[8]  =  s;
        outMatrix.Data[10] =  c;
        return outMatrix;
    }
    static Mat4 EulerZ(f32 angleRadians)
    {
        f32 c = cosf(angleRadians);
        f32 s = sinf(angleRadians);
        Mat4 outMatrix{};
        outMatrix.Data[0] =  c;
        outMatrix.Data[1] =  s;
        outMatrix.Data[4] = -s;
        outMatrix.Data[5] =  c;
        return outMatrix;
    }
    static Mat4 Rotate(float xAngleRadians = 0.0f, float yAngleRadians = 0.0f, float zAngleRadians = 0.0f) 
    {
        Mat4 rotationMatrix = EulerZ(zAngleRadians) * EulerY(yAngleRadians) * EulerX(xAngleRadians);
        return rotationMatrix;
    }
    static Mat4 Scale(float sx, float sy, float sz) 
    {
        Mat4 scaleMatrix;
        scaleMatrix.Data[0] = sx;
        scaleMatrix.Data[5] = sy;
        scaleMatrix.Data[10] = sz;
        return scaleMatrix;
    }
    static Mat4 Translate(float tx, float ty, float tz)
    {
        Mat4 translationMatrix; 
        translationMatrix.Data[12] = tx;
        translationMatrix.Data[13] = ty;
        translationMatrix.Data[14] = tz;
        return translationMatrix;
    }
    static Mat4 CreateTransform(const Vec3& position, 
                                     const Vec3& scale, 
                                     const Vec3& rotation = Vec3(0.0f, 0.0f, 0.0f))
    {
        Mat4 t = Translate(position.x, position.y, position.z);
        Mat4 r = Rotate(rotation.x, rotation.y, rotation.z);
        Mat4 s = Scale(scale.x, scale.y, scale.z);
        Mat4 transformationMatrix = t * r * s;
        return transformationMatrix;
    }
    static Mat4 Ortho(float left, float right, float bot, float top, float zNear, float zFar) 
    {
       Mat4 orthoMatrix; 
       f32 lr = 1.0f / (right - left);
       f32 bt = 1.0f / (top - bot);  
       f32 nf = 1.0f / (zFar - zNear);
       orthoMatrix.Data[0] = 2.0f * lr; 
       orthoMatrix.Data[5] = 2.0f * bt; 
       orthoMatrix.Data[10] = -2.0f * nf;
       orthoMatrix.Data[12] = -(right + left) * lr;
       orthoMatrix.Data[13] = -(top + bot) * bt;
       orthoMatrix.Data[14] = -(zFar + zNear) * nf;
       orthoMatrix.Data[15] = 1.0f;
       return orthoMatrix;

    }
    static Mat4 Perspective(f32 fovRadians, f32 aspectRatio, f32 nearClip, f32 farClip)
    {
        f32 halfTanFov = tan(fovRadians * 0.5f);
        Mat4 outMatrix{};
        memset(outMatrix.Data, 0, sizeof(f32) * 16);
        outMatrix.Data[0] = 1.0f / (aspectRatio * halfTanFov);
        outMatrix.Data[5] = 1.0f / (halfTanFov);
        outMatrix.Data[10] = -(farClip + nearClip) / (farClip - nearClip);
        outMatrix.Data[11] = -1.0f;
        outMatrix.Data[14] = (-2.0f * farClip * nearClip) / (farClip - nearClip);
        return outMatrix; 
    }

    static Vec3 Forward(const Mat4& mat)
    {
        Vec3 forwardVector{};
        forwardVector.x = -mat.Data[8];
        forwardVector.y = -mat.Data[9];
        forwardVector.z = -mat.Data[10];
        Vec3::Normalize(&forwardVector);
        return forwardVector;
    }
    
    static Vec3 Backward(const Mat4& mat)
    {
        Vec3 backwardVector{};
        backwardVector.x = mat.Data[8];
        backwardVector.y = mat.Data[9];
        backwardVector.z = mat.Data[10];
        Vec3::Normalize(&backwardVector);
        return backwardVector;
    }
    static Vec3 Right(const Mat4& mat)
    {
        Vec3 rightVector{};
        rightVector.x = mat.Data[0];
        rightVector.y = mat.Data[1];
        rightVector.z = mat.Data[2];
        Vec3::Normalize(&rightVector);
        return rightVector;
    }
    static Vec3 Left(const Mat4& mat)
    {
        Vec3 leftVector{};
        leftVector.x = -mat.Data[0];
        leftVector.y = -mat.Data[1];
        leftVector.z = -mat.Data[2];
        Vec3::Normalize(&leftVector);
        return leftVector;
    }
    static Vec3 Up(const Mat4& mat)
    {
        Vec3 upVector{};
        upVector.x = mat.Data[4];
        upVector.y = mat.Data[5];
        upVector.z = mat.Data[6];
        Vec3::Normalize(&upVector);
        return upVector;
    }
    static Vec3 Down(const Mat4& mat)
    {
        Vec3 downVector{};
        downVector.x = -mat.Data[4];
        downVector.y = -mat.Data[5];
        downVector.z = -mat.Data[6];
        Vec3::Normalize(&downVector);
        return downVector;
    }
};
}
