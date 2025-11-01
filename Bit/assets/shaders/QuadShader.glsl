#Vertex
layout (location = 0) in vec4 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoords;
layout (location = 3) in float a_TexIndex;

out vec2 v_TexCoords;
out vec4 v_Color;
flat out float v_TexIndex;
uniform mat4 u_ViewProjection;
void main()
{ 
    gl_Position = u_ViewProjection * a_Position;
    v_Color = a_Color;
    v_TexCoords = a_TexCoords;
    v_TexIndex = a_TexIndex;
}

#Fragment 
out vec4 color;  
in vec2 v_TexCoords;
flat in float v_TexIndex;
in vec4 v_Color;
uniform sampler2D u_Textures[16];
void main()
{

    int texIndex = clamp(int(v_TexIndex), 0, 15);
    vec4 texColor = texture(u_Textures[texIndex], v_TexCoords);

    if(texColor.a < 0.1)
        discard;

    color =  texColor * v_Color;
}

