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
    return Vec3Normalize(v);
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
}

