#Vertex
layout (location = 0) a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoords;

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
}

#Fragment
in vec4 v_Color;
in vec2 v_TexCoords;
uniform sampler2D u_DiffuseMap;
uniform sampler2D u_SpecularMap;
uniform sampler2D u_AmbientMap;

void main()
{
    vec4 result = vec4(1.0);

    color =  result * v_Color;
}
