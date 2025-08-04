#Vertex
layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoords;

out vec2 v_TexCoords;
void main()
{ 
    gl_Position = vec4(a_Position, 0.0, 1.0);
    v_TexCoords = a_TexCoords;
}

#Fragment 
out vec4 color;  
in vec2 v_TexCoords;
uniform vec4 u_Color;
uniform sampler2D u_TexImage;
void main()
{
    color = texture(u_TexImage, v_TexCoords);
}

