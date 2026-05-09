#Vertex
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec4 a_Color;
layout (location = 3) in vec2 a_TexCoords;

out vec4 v_Color;
out vec2 v_TexCoords;
out vec3 v_Normal;
uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
void main()
{
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0f);
    v_Color = a_Color;
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
    v_TexCoords = a_TexCoords;
}

#Fragment
in vec4 v_Color;
in vec2 v_TexCoords;
uniform sampler2D u_DiffuseMap;
uniform sampler2D u_SpecularMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_HeightMap;

out vec4 color;
void main()
{
    color = texture(u_DiffuseMap, v_TexCoords);
}



