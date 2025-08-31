#pragma once
#include "Bit/Core/Defines.h"
#include <cmath>

namespace BMath 
{

class Vec2
{
public:
    union {
        struct {
            union { float x, r, s, u; };
            union { float y, g, t, v; };
        };
        float elements[2];
    };

public:
    constexpr Vec2() : x(1.0f), y(1.0f) {}
    constexpr Vec2(float x, float y) : x(x), y(y) {}
    Vec2(const Vec2&) = default;
    ~Vec2() = default;


    Vec2& operator=(const Vec2& vec)
    {
        x = vec.x;
        y = vec.y;
        return *this;
    }

    Vec2& operator=(Vec2&& vec)
    {
        x = vec.x;
        y = vec.y;
        return *this;
    }
    Vec2 operator+(Vec2& other)
    {
        return Vec2(x + other.x, y + other.y);
    }
    Vec2 operator-(Vec2& other)
    {
        return Vec2(x - other.x, y - other.y);
    }
    static float Dot(const Vec2& a, const Vec2& b)
    {
        return a.x * b.x + a.y * b.y;
    }

    
    
    static f32 LengthSquared(const Vec2& vec)
    {
        return vec.x * vec.x + vec.y * vec.y;
    }
    static f32 Length(const Vec2& vec) 
    {
        return std::sqrt(vec.x * vec.x + vec.y * vec.y);
    }

    static void Normalize(Vec2* vec) 
    {
        float vecLength = Vec2::Length(*vec);
        vec->x /= vecLength;
        vec->y /= vecLength;
    }
    static Vec2 Normalize(Vec2 vector) 
    {
        Vec2::Normalize(&vector);
        return vector;
    }
    static f32 Distance(const Vec2& vector1, const Vec2& vector2)
    {
        Vec2 d = {vector2.x - vector1.x, vector2.y - vector1.y};
        return Vec2::Length(d);
    }
    static f32 DistanceSquared(const Vec2& vector1, const Vec2& vector2)
    {
        Vec2 d = {vector2.x - vector1.x, vector2.y - vector1.y};
        return Vec2::LengthSquared(d);
    }
};

struct Vec3
{
    union 
    {
        struct { float x, y, z; };  
        struct { float r, g, b; };
        struct { float s, t, p; };  
        float elements[3];
    };

    constexpr Vec3() = default;
    constexpr Vec3(float value) : x(value), y(value), z(value) {}
    constexpr Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    constexpr Vec3(const float arr[3]) : x(arr[0]), y(arr[1]), z(arr[2]) {}
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
f32 Vec3Length(const Vec3& vec); 
void Vec3Normalize(Vec3* vec); 
Vec3 Vec3Normalize(Vec3 vector); 
Vec3 Vec3Normal2D(const Vec3& v);
const Vec3 Vec3Cross(const Vec3& vec1, const Vec3& vec2);
f32 Vec3LengthSquared(const Vec3& vec);
f32 Vec3Distance(const Vec3& vec1, const Vec3& vec2);
f32 Vec3DistanceSquared(const Vec3& vec1, const Vec3& vec2);

class Vec4
{
public:
    union {
        struct {
            union { float x, r, s; };
            union { float y, g, t; };
            union { float z, b, p; };
            union { float w, a, q; };
        };
        float elements[4];
    };

public:
    constexpr Vec4(float value) : x(value), y(value), z(value), w(value) {}
    constexpr Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    constexpr Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Vec4(const Vec4&) = default;
    ~Vec4() = default;

    Vec4& operator=(const Vec4& vec)
    {
        x = vec.x;
        y = vec.y;
        z = vec.z;
        w = vec.w;
        return *this;
    }

    Vec4& operator=(Vec4&& vec)
    {
        x = vec.x;
        y = vec.y;
        z = vec.z;
        w = vec.w;
        return *this;
    }

    Vec4 operator+(Vec4& other)
    {
        return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    Vec4 operator-(Vec4& other)
    {
        return Vec4(x - other.x, y - other.y, z - other.z, w + other.w);
    }
    static float Dot(const Vec4& a, const Vec4& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    static const Vec4 Cross(const Vec4& vec1, const Vec4& vec2)
    {
        return {
            vec1.y * vec2.z - vec1.z * vec2.y,
            vec1.z * vec2.x - vec1.x * vec2.z,
            vec1.x * vec2.y - vec1.y * vec2.x,
            0.0f 
        };
    }
    
    static f32 LengthSquared(const Vec4& vec)
    {
        return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w;
    }
    static f32 Length(const Vec4& vec) 
    {
        return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
    }

    static void Normalize(Vec4* vec) 
    {
        float vecLength = Vec4::Length(*vec);
        vec->x /= vecLength;
        vec->y /= vecLength;
        vec->z /= vecLength;
        vec->w /= vecLength;
    }
    static Vec4 Normalize(Vec4 vector) 
    {
        Vec4::Normalize(&vector);
        return vector;
    }
    Vec3 ToVec3()
    {
        return Vec3(x, y, z);
    }

    static f32 Distance(const Vec4& vec1, const Vec4& vec2)
    {
        Vec4 d = {vec2.x - vec1.x, vec2.y - vec1.y, vec2.z - vec1.z, vec2.w - vec1.w};
        return Vec4::Length(d);
    }
    static f32 DistanceSquared(const Vec4& vec1, const Vec4& vec2)
    {
        Vec4 d = {vec2.x - vec1.x, vec2.y - vec1.y, vec2.z - vec1.z, vec2.w - vec1.w};
        return Vec4::LengthSquared(d);
    }
};

}
