#pragma once
#include "Bit/Core/Defines.h"
#include <cmath>

namespace BMath 
{

class vec2
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
    constexpr vec2() : x(1.0f), y(1.0f) {}
    constexpr vec2(float x, float y) : x(x), y(y) {}
    vec2(const vec2&) = default;
    ~vec2() = default;


    vec2& operator=(const vec2& vec)
    {
        x = vec.x;
        y = vec.y;
        return *this;
    }

    vec2& operator=(vec2&& vec)
    {
        x = vec.x;
        y = vec.y;
        return *this;
    }

    static float Dot(const vec2& a, const vec2& b)
    {
        return a.x * b.x + a.y * b.y;
    }

    
    
    static f32 LengthSquared(const vec2& vec)
    {
        return vec.x * vec.x + vec.y * vec.y;
    }
    static f32 Length(const vec2& vec) 
    {
        return std::sqrt(vec.x * vec.x + vec.y * vec.y);
    }

    static void Normalize(vec2* vec) 
    {
        float vecLength = vec2::Length(*vec);
        vec->x /= vecLength;
        vec->y /= vecLength;
    }
    static vec2 Normalize(vec2 vector) 
    {
        vec2::Normalize(&vector);
        return vector;
    }
    static f32 Distance(const vec2& vector1, const vec2& vector2)
    {
        vec2 d = {vector2.x - vector1.x, vector2.y - vector1.y};
        return vec2::Length(d);
    }
    static f32 DistanceSquared(const vec2& vector1, const vec2& vector2)
    {
        vec2 d = {vector2.x - vector1.x, vector2.y - vector1.y};
        return vec2::LengthSquared(d);
    }
};

class vec3
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
    constexpr vec3() : x(1.0f), y(1.0f), z(1.0f) {}
    constexpr vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    vec3(const vec3&) = default;
    ~vec3() = default;

    vec3& operator=(const vec3& vec)
    {
        x = vec.x;
        y = vec.y;
        z = vec.z;
        return *this;
    }

    vec3& operator=(vec3&& vec)
    {
        x = vec.x;
        y = vec.y;
        z = vec.z;
        return *this;
    }

    static float Dot(const vec3& a, const vec3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static f32 LengthSquared(const vec3& vec)
    {
        return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
    }
    static f32 Length(const vec3& vec) 
    {
        return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    }

    static void Normalize(vec3* vec) 
    {
        float vecLength = vec3::Length(*vec);
        vec->x /= vecLength;
        vec->y /= vecLength;
        vec->z /= vecLength;
    }
    static vec3 Normalize(vec3 vector) 
    {
        vec3::Normalize(&vector);
        return vector;
    }
    static f32 Distance(const vec3& vec1, const vec3& vec2)
    {
        vec3 d = {vec2.x - vec1.x, vec2.y - vec1.y, vec2.z - vec1.z};
        return vec3::Length(d);
    }
    static f32 DistanceSquared(const vec3& vec1, const vec3& vec2)
    {
        vec3 d = {vec2.x - vec1.x, vec2.y - vec1.y, vec2.z - vec1.z};
        return vec3::LengthSquared(d);
    }
};

class vec4
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
    constexpr vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    constexpr vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    vec4(const vec4&) = default;
    ~vec4() = default;

    vec4& operator=(const vec4& vec)
    {
        x = vec.x;
        y = vec.y;
        z = vec.z;
        w = vec.w;
        return *this;
    }

    vec4& operator=(vec4&& vec)
    {
        x = vec.x;
        y = vec.y;
        z = vec.z;
        w = vec.w;
        return *this;
    }

    static float Dot(const vec4& a, const vec4& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    static f32 LengthSquared(const vec4& vec)
    {
        return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w;
    }
    static f32 Length(const vec4& vec) 
    {
        return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
    }

    static void Normalize(vec4* vec) 
    {
        float vecLength = vec4::Length(*vec);
        vec->x /= vecLength;
        vec->y /= vecLength;
        vec->z /= vecLength;
        vec->w /= vecLength;
    }
    static vec4 Normalize(vec4 vector) 
    {
        vec4::Normalize(&vector);
        return vector;
    }

    static f32 Distance(const vec4& vec1, const vec4& vec2)
    {
        vec4 d = {vec2.x - vec1.x, vec2.y - vec1.y, vec2.z - vec1.z, vec2.w - vec1.w};
        return vec4::Length(d);
    }
    static f32 DistanceSquared(const vec4& vec1, const vec4& vec2)
    {
        vec4 d = {vec2.x - vec1.x, vec2.y - vec1.y, vec2.z - vec1.z, vec2.w - vec1.w};
        return vec4::LengthSquared(d);
    }
};

}
