#include "Transform.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"

namespace BMath
{

Transform TransformCreate()
{
    Transform t;
    t = TransformCreate(Vec3Zero(), Vec3One(), Vec3One());
    t.Local = Mat4Identity();
    t.Parent = 0;
    return t;
}
Transform TransformCreate(const Vec3& position, const Vec3& rotation, const Vec3& scale)
{
    Transform t;
    t.Position = position;
    t.Rotation = rotation;
    t.Scale= scale;
    t.Local = Mat4Identity();
    t.Parent = 0;
    return t;
}
Transform TransformFromPosition(const Vec3& position)
{
    Transform t = TransformCreate(position, Vec3One(), Vec3One());
    return t;
}
Transform TransformFromRotation(const Vec3& rotation)
{
    Transform t = TransformCreate(Vec3Zero(), rotation, Vec3One());
    return t;
}
Transform TransformFromPositionRotation(const Vec3& position, const Vec3& rotation)
{
    Transform t = TransformCreate(position, rotation, Vec3One());
    return t;
}
Transform* TransformGetParent(Transform* t)
{
    if(!t)
    {
        return 0;
    }
    return t->Parent;
}
void TransformSetParent(Transform* t, Transform* parent)
{
    if(t)
    {
        t->Parent = parent;
    }
}

Vec3 TransformGetPosition(const Transform* t)
{
    return t->Position;
}
void TransformSetPosition(Transform* t, const Vec3& position)
{
    t->Position = position;
    t->IsDirty = true;
}
void TransformTranslate(Transform* t, const Vec3& position)
{
    t->Position += position;
    t->IsDirty = true;
}

Vec3 TransformGetRotation(const Transform* t)
{
    return t->Rotation;
}
void TransformSetRotation(Transform* t, const Vec3& rotation)
{
    t->Rotation = rotation;
    t->IsDirty = true;
}
void TransformRotate(Transform* t, const Vec3& rotation)
{
    t->Rotation += rotation;
    t->IsDirty = true;
}

Vec3 TransformGetScale(const Transform* t)
{
    return t->Scale;
}
void TransformSetScale(Transform* t, const Vec3& scale)
{
    t->Scale = scale;
    t->IsDirty = true;
}
void TransformScale(Transform* t, const Vec3& scale)
{
    t->Scale += scale;
    t->IsDirty = true;
}

Mat4 TransformGetLocal(Transform* t)
{
    if(t)
    {
        if(t->IsDirty)
        {
            Mat4 tr = Mat4CreateTransform(t->Position, t->Scale, t->Rotation);
            return tr;
        }
        return Mat4Identity();
    }
    return Mat4Identity();
}
Mat4 TransformGetWorld(Transform* t)
{
    if(t)
    {
        Mat4 l = TransformGetLocal(t);
        if(t->Parent)
        {
            Mat4 p = TransformGetWorld(t);
            return p * l;
        }
        return l;
    }
    return Mat4Identity();
}

}

