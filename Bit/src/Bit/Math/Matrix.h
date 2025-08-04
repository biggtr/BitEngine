#pragma once

#include "Bit/Renderer/RendererAPI.h"
#include <complex>
#include <math.h>
namespace BMath 
{
class Matrix4x4
{
public:
    float Data[16];

public:
    Matrix4x4()
        : Data{
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        }
    {
    }



    static Vector4 Multiply(const Matrix4x4& mat, const Vector4& vec)
    {
        Vector4 result(0.0f, 0.0f, 0.0f, 0.0f);
        result.x = mat.Data[0] * vec.x + mat.Data[4] * vec.y + mat.Data[8]  * vec.z + mat.Data[12] * vec.w;
        result.y = mat.Data[1] * vec.x + mat.Data[5] * vec.y + mat.Data[9]  * vec.z + mat.Data[13] * vec.w;
        result.z = mat.Data[2] * vec.x + mat.Data[6] * vec.y + mat.Data[10] * vec.z + mat.Data[14] * vec.w;
        result.w = mat.Data[3] * vec.x + mat.Data[7] * vec.y + mat.Data[11] * vec.z + mat.Data[15] * vec.w;
        return result;
    }

    static Matrix4x4 Multiply(const Matrix4x4& mat1, const Matrix4x4& mat2)
    {
        Matrix4x4 result;

        for(size_t row = 0; row < 4; row++)
        {
            for(size_t col = 0; col < 4; col++)
            {
                float sum = 0.0f;
                for(size_t k = 0; k < 4; k++)
                {
                    sum += mat1.Data[k * 4 + row] * mat2.Data[col * 4 + k];
                }
                result.Data[col * 4 + row] = sum;
            }
        }
        return result;
    }

    Matrix4x4 operator*(const Matrix4x4& mat)
    {
        return Multiply(*this, mat);
    }

    static Matrix4x4 Identity()
    {
        return Matrix4x4();
    }

    static Matrix4x4 Rotate(float xAngle = 0.0f, float yAngle = 0.0f, float zAngle = 0.0f) 
    {
        Matrix4x4 xRotationMatrix; 
        xRotationMatrix.Data[5] =  cos(xAngle);
        xRotationMatrix.Data[6] = -sin(xAngle);
        xRotationMatrix.Data[9]  = sin(xAngle);
        xRotationMatrix.Data[10] = cos(xAngle);

        Matrix4x4 yRotationMatrix; 
        yRotationMatrix.Data[0] = cos(yAngle);
        yRotationMatrix.Data[2] = -sin(yAngle);
        yRotationMatrix.Data[8] = sin(yAngle);
        yRotationMatrix.Data[10] = cos(yAngle);


        Matrix4x4 zRotationMatrix;
        zRotationMatrix.Data[0] = cos(zAngle);
        zRotationMatrix.Data[1] = sin(zAngle);
        zRotationMatrix.Data[4] = -sin(zAngle);
        zRotationMatrix.Data[5] = cos(zAngle);

        Matrix4x4 rotationMatrix = Multiply(zRotationMatrix, yRotationMatrix);
        rotationMatrix = Multiply(rotationMatrix, xRotationMatrix);
        return rotationMatrix;
    }
    static Matrix4x4 Scale(float sx, float sy, float sz) 
    {
        Matrix4x4 scaleMatrix;
        scaleMatrix.Data[0] = sx;
        scaleMatrix.Data[5] = sy;
        scaleMatrix.Data[10] = sz;
        return scaleMatrix;
    }
    static Matrix4x4 Translate(float tx, float ty, float tz)
    {
        Matrix4x4 translationMatrix; 
        translationMatrix.Data[12] = tx;
        translationMatrix.Data[13] = ty;
        translationMatrix.Data[14] = tz;
        return translationMatrix;
    }
    static Matrix4x4 CreateTransform(const Vector3& position, 
                                     const Vector3& scale, 
                                     const Vector3& rotation = Vector3(0.0f, 0.0f, 0.0f))
    {
        Matrix4x4 t = Translate(position.x, position.y, position.z);
        Matrix4x4 r = Rotate(rotation.x, rotation.y, rotation.z);
        Matrix4x4 s = Scale(scale.x, scale.y, scale.z);
        Matrix4x4 translationMatrix = t * r * s;
        return translationMatrix;
    }
    static Matrix4x4 Ortho(float left, float right, float bot, float top, float zNear, float zFar) 
    {
       Matrix4x4 orthoMatrix; 

       orthoMatrix.Data[0] = 2 / (right -left);
       orthoMatrix.Data[5] = 2 / (top - bot);
       orthoMatrix.Data[10] = -2 / (zFar - zNear);
       orthoMatrix.Data[12] = -(right + left) / (right - left);
       orthoMatrix.Data[13] = -(top + bot) / (top - bot);
       orthoMatrix.Data[14] = -(zFar + zNear) / (zFar - zNear);
       orthoMatrix.Data[15] = 1;
       return orthoMatrix;

    }
};
}
