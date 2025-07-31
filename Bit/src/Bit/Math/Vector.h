#pragma once
#include <cmath>
namespace BitMath 
{


class Vector2
{
public:
    Vector2(float x = 1, float y = 1)
        :  x(x), Y(y)
    {
    }

    static float Dot(Vector2 vec1, Vector2 vec2)
    {
        return vec1.x * vec2.x + vec1.Y * vec2.Y;
    }
    float Magnitude()
    {
        return std::sqrt(x * x + Y * Y);
    }

    Vector2 Normalize()
    {
        float magnitude = Magnitude();
        float invMagnitude = 1 / magnitude;
        return (magnitude != 0) ? Vector2(x * invMagnitude, Y * invMagnitude) : Vector2(0, 0);
    }

    

public:
    float x, Y;
};

class Vector3
{

public:
    Vector3(float x, float y, float z)
        : x(x), y(y), z(z)
    {
    }
    Vector3() = default;                              // Default constructor
    Vector3(const Vector3& other) = default;         // Copy constructor
    Vector3(Vector3&& other) = default;              // Move constructor
    Vector3& operator=(const Vector3& other) = default;  // Copy assignment
    Vector3& operator=(Vector3&& other) = default;       // Move assignment
    static float Dot(Vector3 vec1, Vector3 vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
    }
    float Magnitude()
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3 Normalize()
    {
        float magnitude = Magnitude();
        float invMagnitude = 1 / magnitude;
        return (magnitude != 0) ? Vector3(x * invMagnitude, y * invMagnitude, z * invMagnitude) : Vector3(0, 0, 0);
    }
public:
    float x, y, z;
};
    
class Vector4
{

public:
    Vector4(float x, float y, float z, float w)
        : x(x), y(y), z(z), w(w)
    {
    }

    static float Dot(Vector4 vec1, Vector4 vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z + vec1.w * vec2.w;
    }
    float Magnitude()
    {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    Vector4 Normalize()
    {
        float magnitude = Magnitude();
        float invMagnitude = 1 / magnitude;
        return (magnitude != 0) ? Vector4(x * invMagnitude, y * invMagnitude, z * invMagnitude, w * invMagnitude) : Vector4(0, 0, 0, 0);
    }
public:
    float x, y, z, w;
};

}
