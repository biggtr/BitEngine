#Vertex
layout (location = 0) in vec4 a_Position;
layout (location = 1) in vec3 a_NormalizedPosition;
layout (location = 2) in vec4 a_Color;
layout (location = 3) in float a_Thickness;
layout (location = 4) in float a_Fade;

out vec4 v_Color;
out vec3 v_NormalizedPosition;
out float v_Thickness;
out float v_Fade;

uniform mat4 u_ViewProjection;
void main()
{ 
    v_Color = a_Color;
    v_NormalizedPosition = a_NormalizedPosition;
    v_Thickness = a_Thickness;
    v_Fade = a_Fade;
    gl_Position = u_ViewProjection * a_Position;
}

#Fragment 
out vec4 o_Color;  
in vec4 v_Color;
in vec3 v_NormalizedPosition;
in float v_Thickness;
in float v_Fade;
void main()
{
    float d = 1.0 - length(v_NormalizedPosition);
    float circle = smoothstep(0.0, v_Fade, d);
    circle *= smoothstep(v_Thickness + v_Fade, v_Thickness, d);

    if(circle  == 0.0)
        discard;

    o_Color = v_Color;
    o_Color.a = circle;
}

