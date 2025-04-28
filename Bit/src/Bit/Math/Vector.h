
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

};
    
class Vector4
{

};

}
