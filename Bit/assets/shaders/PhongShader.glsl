#Vertex
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec4 a_Color;
layout (location = 3) in vec2 a_TexCoords;

out vec4 v_Color;
out vec2 v_TexCoords;
out float v_TexIndex; 
uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
void main()
{
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0f);
    v_Color = a_Color;
    v_TexCoords = a_TexCoords;
}

#Fragment
in vec4 v_Color;
in vec2 v_TexCoords;
uniform sampler2D u_texture;
out vec4 color;
void main()
{
    color = texture(u_texture, v_TexCoords);

}
