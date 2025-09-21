#Vertex
layout (location = 0) a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoords;
layout (location = 3) in float a_TexIndex;

out vec4 v_Color;
out vec2 v_TexCoords;
out float v_TexIndex; 
uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
void main()
{
    gl_position = u_ViewProjection * u_Model * vec4(a_Position, 1.0f);
    v_Color = a_Color;
    v_TexCoords = a_TexCoords;
    v_TexIndex = a_TexIndex;
}


#Fragment
in vec4 v_Color;
in vec2 v_TexCoords;
uniform int textureCount;
uniform int blendMode;
uniform float blendStrength;
uniform sampler2D u_Textures[32];

vec3 MultiplyBlend(vec3 base, vec3 blend, flaot blendStrength)
{
    vec3 result = base * blend;
    return mix(base, result, blendStrength);
}
vec3 AdditiveBlend(vec3 base, vec3 blend, flaot blendStrength)
{
    return base + (blend * blendStrength);
}
void main()
{
    vec4 result = vec4(1.0);
    if(textureCount > 0)
    {
        result = texture(u_Textures[0], v_TexCoords);

        for(int i = 1; i < textureCount && i < 32; ++i)
        {
            result = texture(u_Textures[i], v_TexCoords);
            switch (blendMode) 
            {
                case 0:
                    result.rgb = multiplyBlend(result.rgb, texColor.rgb, blendStrength);
                    break;
                case 1: 
                    result.rgb = additiveBlend(result.rgb, texColor.rgb, blendStrength);
                    break;
                case 2: // alpha blend
                    result.rgb = mix(result.rgb, texColor.rgb, texColor.a * blendStrength);
                    break;
                default:
                    result.rgb = mix(result.rgb, texColor.rgb, 0.5);
        }
    }

    color =  result * v_Color;
}
