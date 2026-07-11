#Vertex
layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoords;
out vec2 TexCoords;

uniform mat4 u_ViewProjection;

void main()
{
    gl_Position = u_ViewProjection * vec4(a_Position, 0.0, 1.0);
    TexCoords = a_TexCoords;
}  

#Fragment
in vec2 TexCoords;
out vec4 color;

uniform sampler2D tex;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}  
