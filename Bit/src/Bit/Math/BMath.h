#include "Bit/Core/Defines.h"
#define B_PI 3.14159265358979323846f
#define B_2PI (2.0f * B_PI)
#define B_4PI (4.0f * B_PI)
#define B_HALF_PI (0.5f * B_PI)
#define B_QUARTER_PI (0.25f * B_PI)
#define B_ONE_OVER_PI (1.0f / B_PI)
#define B_ONE_OVER_TWO_PI (1.0f / B_2PI)
#define B_SQRT_TWO 1.41421356237309504880f
#define B_SQRT_THREE 1.73205080756887729352f
#define B_SQRT_ONE_OVER_TWO 0.70710678118654752440f
#define B_SQRT_ONE_OVER_THREE 0.57735026918962576450f
#define B_DEG2RAD_MULTIPLIER (B_PI / 180.0f)
#define B_RAD2DEG_MULTIPLIER (180.0f / B_PI)
#define B_SEC_TO_US_MULTIPLIER (1000.0f * 1000.0f)
#define B_SEC_TO_MS_MULTIPLIER 1000.0f
#define B_MS_TO_SEC_MULTIPLIER 0.001f
#define B_INFINITY (1e30f * 1e30f)
#define B_FLOAT_EPSILON 1.192092896e-07f
#define B_FLOAT_MIN -3.40282e+38F
#define B_FLOAT_MAX 3.40282e+38F


inline bool IsPowerOf2(u64 value)
{
    return (value != 0) && (value & value - 1) == 0;
}



vec2
