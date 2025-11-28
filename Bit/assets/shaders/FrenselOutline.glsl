#Vertex
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec4 a_Color;
layout (location = 3) in vec2 a_TexCoords;

out vec4 v_Color;
out vec2 v_TexCoords;
out vec3 v_WorldPos;
out vec3 v_Normal;
uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
void main()
{
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0f);
    v_WorldPos = vec3(u_Model * vec4(a_Position, 1.0f));
    v_Color = a_Color;
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
}

#Fragment
in vec4 v_Color;
in vec2 v_TexCoords;
in vec3 v_WorldPos;
in vec3 v_Normal;
out vec4 color;
uniform vec3 u_CameraPos;

void main()
{
    vec3 viewVec = normalize(u_CameraPos - v_WorldPos);
    float fren = max(dot(normalize(v_Normal), viewVec), 0.0);
    vec3 baseColor = vec3(0.0, 1.0, 0.0);
    vec3 rimColor = v_Color.xyz;
    fren = pow((1.0 - fren), 3);
    vec3 result = mix(baseColor, rimColor, fren);
    color = vec4(result, fren);
}
