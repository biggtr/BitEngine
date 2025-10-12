#pragma once
#include "Bit/Core/Defines.h"
#include <math.h>

namespace BMath 
{
struct Vec2
{
    union 
    {
        struct { f32 x, y; };  
        struct { f32 r, g; };
        struct { f32 s, t; };  
        f32 elements[2];
    };

    constexpr Vec2() = default;
    constexpr Vec2(f32 value) : x(value), y(value) {}
    constexpr Vec2(f32 x_, f32 y_) : x(x_), y(y_) {}
    constexpr Vec2(const f32 arr[2]) : x(arr[0]), y(arr[1]){};

};

// Vec2& operator=(const Vec2& vec)
// {
//     x = vec.x;
//     y = vec.y;
//     return *this;
// }
//
// Vec2& operator=(Vec2&& vec)
// {
//     x = vec.x;
//     y = vec.y;
//     return *this;
// }
// Vec2 operator+(Vec2& other)
// {
//     return Vec2(x + other.x, y + other.y);
// }
// Vec2 operator-(Vec2& other)
// {
//     return Vec2(x - other.x, y - other.y);
// }
// static float Dot(const Vec2& a, const Vec2& b)
// {
//     return a.x * b.x + a.y * b.y;
// }
//
//
//
// static f32 LengthSquared(const Vec2& vec)
// {
//     return vec.x * vec.x + vec.y * vec.y;
// }
// static f32 Length(const Vec2& vec) 
// {
//     return std::sqrt(vec.x * vec.x + vec.y * vec.y);
// }
//
// static void Normalize(Vec2* vec) 
// {
//     float vecLength = Vec2::Length(*vec);
//     vec->x /= vecLength;
//     vec->y /= vecLength;
// }
// static Vec2 Normalize(Vec2 vector) 
// {
//     Vec2::Normalize(&vector);
//     return vector;
// }
// static f32 Distance(const Vec2& vector1, const Vec2& vector2)
// {
//     Vec2 d = {vector2.x - vector1.x, vector2.y - vector1.y};
//     return Vec2::Length(d);
// }
// static f32 DistanceSquared(const Vec2& vector1, const Vec2& vector2)
// {
//     Vec2 d = {vector2.x - vector1.x, vector2.y - vector1.y};
//     return Vec2::LengthSquared(d);
// }
struct Vec3
{
    union 
    {
        struct { f32 x, y, z; };  
        struct { f32 r, g, b; };
        struct { f32 s, t, p; };  
        f32 elements[3];
    };

    constexpr Vec3() = default;
    constexpr Vec3(f32 value) : x(value), y(value), z(value) {}
    constexpr Vec3(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    constexpr Vec3(const f32 arr[3]) : x(arr[0]), y(arr[1]), z(arr[2]) {}
};

Vec3& operator*=(Vec3& v, f32 scalar); 
Vec3 operator*(const Vec3& v, f32 scalar); 
Vec3& operator/=(Vec3& v, f32 scalar); 
Vec3 operator/(Vec3& v, f32 scalar);
Vec3& operator+=(Vec3& v1, const Vec3& v2);
Vec3 operator+(const Vec3& v1, const Vec3& v2);
Vec3& operator-=(Vec3& v1, const Vec3& v2);
Vec3 operator-(const Vec3& v1, const Vec3& v2);
float Vec3Dot(const Vec3& a, const Vec3& b);
Vec3 Vec3Zero();
Vec3 Vec3One();
f32 Vec3Length(const Vec3& vec); 
void Vec3Normalize(Vec3* vec); 
Vec3 Vec3Normalize(Vec3 vector); 
Vec3 Vec3Normal2D(const Vec3& v);
const Vec3 Vec3Cross(const Vec3& vec1, const Vec3& vec2);
f32 Vec3LengthSquared(const Vec3& vec);
f32 Vec3Distance(const Vec3& vec1, const Vec3& vec2);
f32 Vec3DistanceSquared(const Vec3& vec1, const Vec3& vec2);

struct Vec4
{
    union {
        struct {
            union { f32 x, r, s; };
            union { f32 y, g, t; };
            union { f32 z, b, p; };
            union { f32 w, a, q; };
        };
        f32 elements[4];
    };

    constexpr Vec4() = default;
    constexpr Vec4(f32 value) : x(value), y(value), z(value) {}
    constexpr Vec4(f32 x_, f32 y_, f32 z_, f32 w_) : x(x_), y(y_), z(z_), w(w_) {}
    constexpr Vec4(const f32 arr[4]) : x(arr[0]), y(arr[1]), z(arr[2]), w(arr[3]) {}
    
};

Vec4& operator*=(Vec4& v, f32 scalar); 
Vec4 operator*(const Vec4& v, f32 scalar); 
Vec4& operator/=(Vec4& v, f32 scalar); 
Vec4 operator/(Vec4& v, f32 scalar);
Vec4& operator+=(Vec4& v1, const Vec4& v2);
Vec4 operator+(const Vec4& v1, const Vec4& v2);
Vec4& operator-=(Vec4& v1, const Vec4& v2);
Vec4 operator-(const Vec4& v1, const Vec4& v2);
float Vec4Dot(const Vec4& a, const Vec4& b);
f32 Vec4Length(const Vec4& vec); 
void Vec4Normalize(Vec4* vec); 
Vec4 Vec4Normalize(Vec4 vector); 
Vec3 Vec4ToVec3(const Vec4& v);
const Vec4 Vec4Cross(const Vec4& vec1, const Vec4& vec2);
f32 Vec4LengthSquared(const Vec4& vec);
f32 Vec4Distance(const Vec4& vec1, const Vec4& vec2);
f32 Vec4DistanceSquared(const Vec4& vec1, const Vec4& vec2);


}
