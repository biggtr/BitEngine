#version 430 core

layout(binding = 0, rgba32f) uniform image2D imageOutput;

layout(local_size_x = 16, local_size_y = 16) in;
void main(void)
{
    vec4 value = vec4(0,0,0,1);
    ivec2 texCoords = ivec2(gl_GlobalInvocationID.xy);
    value.x = float(texelCoord.x)/(gl_NumWorkGroups.x);
    value.y = float(texelCoord.y)/(gl_NumWorkGroups.y);
    imageStore(imageOutput, texCoords, value);
}
