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

class Vec3
{
public:
    union {
        struct {
            union { float x, r, s, u; };
            union { float y, g, t, v; };
            union { float z, b, p, w; };
        };
        float elements[3];
    };

public:
    constexpr Vec3() : x(1.0f), y(1.0f), z(1.0f) {}
    constexpr Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3(const Vec3&) = default;
    ~Vec3() = default;

    Vec3& operator=(const Vec3& vec)
    {
        x = vec.x;
        y = vec.y;
        z = vec.z;
        return *this;
    }

    Vec3& operator=(Vec3&& vec)
    {
        x = vec.x;
        y = vec.y;
        z = vec.z;
        return *this;
    }

    static float Dot(const Vec3& a, const Vec3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static const Vec3 Cross(const Vec3& vec1, const Vec3& vec2)
    {
        return {
            vec1.y * vec2.z - vec1.z * vec2.y,
            vec1.z * vec2.x - vec1.x * vec2.z,
            vec1.x * vec2.y - vec1.y * vec2.x,
        };
    }
    static f32 LengthSquared(const Vec3& vec)
    {
        return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
    }
    static f32 Length(const Vec3& vec) 
    {
        return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    }

    static void Normalize(Vec3* vec) 
    {
        float vecLength = Vec3::Length(*vec);
        vec->x /= vecLength;
        vec->y /= vecLength;
        vec->z /= vecLength;
    }
    static Vec3 Normalize(Vec3 vector) 
    {
        Vec3::Normalize(&vector);
        return vector;
    }
    static f32 Distance(const Vec3& vec1, const Vec3& vec2)
    {
        Vec3 d = {vec2.x - vec1.x, vec2.y - vec1.y, vec2.z - vec1.z};
        return Vec3::Length(d);
    }
    static f32 DistanceSquared(const Vec3& vec1, const Vec3& vec2)
    {
        Vec3 d = {vec2.x - vec1.x, vec2.y - vec1.y, vec2.z - vec1.z};
        return Vec3::LengthSquared(d);
    }
};

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
