#include "Vector.h"

namespace BMath
{


Vec3& operator*=(Vec3& v, f32 scalar) 
{
    v.x *= scalar;
    v.y *= scalar;
    v.z *= scalar;
    return v;
}
Vec3 operator*(const Vec3& v, f32 scalar) 
{
    Vec3 tmp = v;
    tmp *= scalar;
    return tmp;
}

Vec3& operator/=(Vec3& v, f32 scalar) 
{
    v.x /= scalar;
    v.y /= scalar;
    v.z /= scalar;
    return v;
}
Vec3 operator/(Vec3& v, f32 scalar)
{
    Vec3 tmp = v;
    tmp /= scalar;
    return tmp;
}
Vec3& operator+=(Vec3& v1, const Vec3& v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    return v1;
}
Vec3 operator+(const Vec3& v1, const Vec3& v2)
{
    Vec3 tmp = v1;
    tmp += v2;
    return tmp;
}

Vec3& operator-=(Vec3& v1, const Vec3& v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
    return v1;
}
Vec3 operator-(const Vec3& v1, const Vec3& v2)
{
    Vec3 tmp = v1;
    tmp -= v2;
    return tmp;
}

float Vec3Dot(const Vec3& a, const Vec3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

f32 Vec3Length(const Vec3& vec) 
{
    return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}
void Vec3Normalize(Vec3* vec) 
{
    float vecLength = Vec3Length(*vec);
    vec->x /= vecLength;
    vec->y /= vecLength;
    vec->z /= vecLength;
}
Vec3 Vec3Normalize(Vec3 vector) 
{
    Vec3Normalize(&vector);
    return vector;
}
Vec3 Vec3Normal2D(const Vec3& v)
{
    return {v.y, -v.x, v.z};
}
const Vec3 Vec3Cross(const Vec3& vec1, const Vec3& vec2)
{
    return {
        vec1.y * vec2.z - vec1.z * vec2.y,
        vec1.z * vec2.x - vec1.x * vec2.z,
        vec1.x * vec2.y - vec1.y * vec2.x,
    };
}
f32 Vec3LengthSquared(const Vec3& vec)
{
    return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

f32 Vec3Distance(const Vec3& vec1, const Vec3& vec2)
{
    Vec3 d = {vec2.x - vec1.x, vec2.y - vec1.y, vec2.z - vec1.z};
    return Vec3Length(d);
}
f32 Vec3DistanceSquared(const Vec3& vec1, const Vec3& vec2)
{
    Vec3 d = {vec2.x - vec1.x, vec2.y - vec1.y, vec2.z - vec1.z};
    return Vec3LengthSquared(d);
}


Vec4& operator*=(Vec4& v, f32 scalar) 
{
   v.x *= scalar;
   v.y *= scalar;
   v.z *= scalar;
   v.w *= scalar;
   return v;
}
Vec4 operator*(const Vec4& v, f32 scalar) 
{
    Vec4 tmp = v;
    tmp *= scalar;
    return tmp;
}

Vec4& operator/=(Vec4& v, f32 scalar) 
{
   v.x /= scalar;
   v.y /= scalar;
   v.z /= scalar;
   v.w /= scalar;
   return v;
}
Vec4 operator/(Vec4& v, f32 scalar)
{
   Vec4 tmp = v;
   tmp /= scalar;
   return tmp;
}
Vec4& operator+=(Vec4& v1, const Vec4& v2)
{
   v1.x += v2.x;
   v1.y += v2.y;
   v1.z += v2.z;
   v1.w += v2.w;
   return v1;
}
Vec4 operator+(const Vec4& v1, const Vec4& v2)
{
   Vec4 tmp = v1;
   tmp += v2;
   return tmp;
}

Vec4& operator-=(Vec4& v1, const Vec4& v2)
{
   v1.x -= v2.x;
   v1.y -= v2.y;
   v1.z -= v2.z;
   v1.w -= v2.w;
   return v1;
}
Vec4 operator-(const Vec4& v1, const Vec4& v2)
{
    Vec4 tmp = v1;
    tmp -= v2;
    return tmp;
}
float Vec4Dot(const Vec4& a, const Vec4& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

const Vec4 Vec4Cross(const Vec4& vec1, const Vec4& vec2)
{
    return {
        vec1.y * vec2.z - vec1.z * vec2.y,
        vec1.z * vec2.x - vec1.x * vec2.z,
        vec1.x * vec2.y - vec1.y * vec2.x,
        0.0f 
    };
}

f32 Vec4LengthSquared(const Vec4& vec)
{
    return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w;
}
f32 Vec4Length(const Vec4& vec) 
{
    return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
}

void Vec4Normalize(Vec4* vec) 
{
    float vecLength = Vec4Length(*vec);
    vec->x /= vecLength;
    vec->y /= vecLength;
    vec->z /= vecLength;
    vec->w /= vecLength;
}
Vec4 Vec4Normalize(Vec4 vector) 
{
    Vec4Normalize(&vector);
    return vector;
}
Vec3 Vec4ToVec3(const Vec4& v)
{
    return Vec3(v.x, v.y, v.z);
}

f32 Vec4Distance(const Vec4& vec1, const Vec4& vec2)
{
    Vec4 d = {vec2.x - vec1.x, vec2.y - vec1.y, vec2.z - vec1.z, vec2.w - vec1.w};
    return Vec4Length(d);
}
f32 Vec4DistanceSquared(const Vec4& vec1, const Vec4& vec2)
{
    Vec4 d = {vec2.x - vec1.x, vec2.y - vec1.y, vec2.z - vec1.z, vec2.w - vec1.w};
    return Vec4LengthSquared(d);
}
}

