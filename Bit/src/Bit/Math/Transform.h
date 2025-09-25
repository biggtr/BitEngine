#pragma once 
#include "Bit/Core/Defines.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
namespace BMath
{

struct Transform
{
    Vec3 Position;
    Vec3 Rotation;
    Vec3 Scale;

    b8 IsDirty;
    Mat4 Local;

    Transform* Parent;
};

Transform TransformCreate();
Transform TransformCreate(const Vec3& position,  const Vec3& scale, const Vec3& rotation);
Transform TransformFromPosition(const Vec3& position);
Transform TransformFromRotation(const Vec3& rotation);
Transform TransformFromPositionRotation(const Vec3& position, const Vec3& rotation);
Transform* TransformGetParent();
void TransformSetParent(Transform* t, Transform* parent);

Vec3 TransformGetPosition(const Transform* t);
void TransformSetPosition(Transform* t, const Vec3& position);
void TransformTranslate(Transform* t, const Vec3& position);

Vec3 TransformGetRotation(const Transform* t);
void TransformSetRotation(Transform* t, const Vec3& rotation);
void TransformRotate(Transform* t, const Vec3& rotation);

Vec3 TransformGetScale(const Transform* t);
void TransformSetScale(Transform* t, const Vec3& scale);
void TransformScale(Transform* t, const Vec3& scale);

Mat4 TransformGetLocal(Transform* t);
Mat4 TransformGetWorld(Transform* t);
}
