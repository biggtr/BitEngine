#include "Bit/Math/Vector.h"
namespace BPhysics
{
struct CParticle
{
    BMath::Vec3 SumForces;
    BMath::Vec3 Acceleration;
    BMath::Vec3 Velocity; 
    BMath::Vec3 Position;
    f32 Mass;
    f32 InverseMass;
    f32 Gravity;

    CParticle(const BMath::Vec3& position, f32 mass)
        :  SumForces(0.0f), Acceleration(0.0f), Velocity(0.0f),
           Position(position), Mass(mass), Gravity(9.8f)
    {
        InverseMass = Mass == 0.0f ? 0.0f : 1.0f / Mass;
    }
};
}
