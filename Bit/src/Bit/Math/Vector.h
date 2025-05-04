#include <cmath>
namespace BitMath 
{


class Vector2
{
public:
    Vector2(float x, float y)
        : X(x), Y(y)
    {
    }

    static float Dot(Vector2 vec1, Vector2 vec2)
    {
        return vec1.X * vec2.X + vec1.Y * vec2.Y;
    }
    float Magnitude()
    {
        return std::sqrt(X * X + Y * Y);
    }

    Vector2 Normalize()
    {
        float magnitude = Magnitude();
        float invMagnitude = 1 / magnitude;
        return (magnitude != 0) ? Vector2(X * invMagnitude, Y * invMagnitude) : Vector2(0, 0);
    }

    

public:
    float X, Y;
};

class Vector3
{

public:
    Vector3(float x, float y, float z)
        : X(x), Y(y), Z(z)
    {
    }

    static float Dot(Vector3 vec1, Vector3 vec2)
    {
        return vec1.X * vec2.X + vec1.Y * vec2.Y + vec1.Z * vec2.Z;
    }
    float Magnitude()
    {
        return std::sqrt(X * X + Y * Y + Z * Z);
    }

    Vector3 Normalize()
    {
        float magnitude = Magnitude();
        float invMagnitude = 1 / magnitude;
        return (magnitude != 0) ? Vector3(X * invMagnitude, Y * invMagnitude, Z * invMagnitude) : Vector3(0, 0, 0);
    }
public:
    float X, Y, Z;
};
    
class Vector4
{

public:
    Vector4(float x, float y, float z, float w)
        : X(x), Y(y), Z(z), W(w)
    {
    }

    static float Dot(Vector4 vec1, Vector4 vec2)
    {
        return vec1.X * vec2.X + vec1.Y * vec2.Y + vec1.Z * vec2.Z + vec1.W * vec2.W;
    }
    float Magnitude()
    {
        return std::sqrt(X * X + Y * Y + Z * Z + W * W);
    }

    Vector4 Normalize()
    {
        float magnitude = Magnitude();
        float invMagnitude = 1 / magnitude;
        return (magnitude != 0) ? Vector4(X * invMagnitude, Y * invMagnitude, Z * invMagnitude, W * invMagnitude) : Vector4(0, 0, 0, 0);
    }
public:
    float X, Y, Z, W;
};

}
