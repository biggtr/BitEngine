#pragma once 
#include "Bit/Core/Defines.h"

enum PhysicsCategories 
{
    STATIC  = 0x00000000,
    DYNAMIC = 0x00000001,
    PLAYER  = 0x00000002,
    ENEMY   = 0x00000004,
    FLOOR   = 0x00000008,
    NONE    = 0xFFFFFFFF,
};

enum PhysicsColliderType
{
    BOX,
    CIRCLE,
    CAPSULE,
};
