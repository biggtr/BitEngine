#include "Vector.h"
#include "BMath.h"

namespace BMath
{

Vec2& operator*=(Vec2& v, f32 scalar) 
{
    v.x *= scalar;
    v.y *= scalar;
    return v;
}
Vec2 operator*(const Vec2& v, f32 scalar) 
{
    Vec2 tmp = v;
    tmp *= scalar;
    return tmp;
}

Vec2& operator/=(Vec2& v, f32 scalar) 
{
    v.x /= scalar;
    v.y /= scalar;
    return v;
}
Vec2 operator/(Vec2& v, f32 scalar)
{
    Vec2 tmp = v;
    tmp /= scalar;
    return tmp;
}
Vec2& operator+=(Vec2& v1, const Vec2& v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    return v1;
}
Vec2 operator+(const Vec2& v1, const Vec2& v2)
{
    Vec2 tmp = v1;
    tmp += v2;
    return tmp;
}

Vec2& operator-=(Vec2& v1, const Vec2& v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    return v1;
}
Vec2 operator-(const Vec2& v1, const Vec2& v2)
{
    Vec2 tmp = v1;
    tmp -= v2;
    return tmp;
}

Vec2 Vec2Zero()
{
    return Vec2(0, 0);
}
Vec2 Vec2One()
{
    return Vec2(1, 1);
}
float Vec2Dot(const Vec2& a, const Vec2& b)
{
    return a.x * b.x + a.y * b.y; 
}

f32 Vec2Length(const Vec2& vec) 
{
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}
void Vec2Normalize(Vec2* vec) 
{
    float vecLength = Vec2Length(*vec);
    vec->x /= vecLength;
    vec->y /= vecLength;
}
Vec2 Vec2Normalize(Vec2 vector) 
{
    Vec2Normalize(&vector);
    return vector;
}
Vec2 Vec2Normal2D(const Vec2& v)
{
    return {v.y, -v.x};
}
f32 Vec2LengthSquared(const Vec2& vec)
{
    return vec.x * vec.x + vec.y * vec.y; 
}

f32 Vec2Distance(const Vec2& vec1, const Vec2& vec2)
{
    Vec2 d = {vec2.x - vec1.x, vec2.y - vec1.y};
    return Vec2Length(d);
}
f32 Vec2DistanceSquared(const Vec2& vec1, const Vec2& vec2)
{
    Vec2 d = {vec2.x - vec1.x, vec2.y - vec1.y};
    return Vec2LengthSquared(d);
}

BMath::Vec2 Lerp(BMath::Vec2 a, BMath::Vec2 b, f32 t)
{
    return a + (b - a) * t;
}


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

Vec3 Vec3Zero()
{
    return Vec3(0, 0, 0);
}
Vec3 Vec3One()
{
    return Vec3(1, 1, 1);
}
float Vec3Dot(const Vec3& a, const Vec3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

f32 Vec3Length(const Vec3& vec) 
{
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
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

BMath::Vec3 Lerp(BMath::Vec3 a, BMath::Vec3 b, f32 t)
{
    return a + (b - a) * t;
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
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
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

BMath::Vec4 Lerp(BMath::Vec4 a, BMath::Vec4 b, f32 t)
{
    return a + (b - a) * t;
}
}

