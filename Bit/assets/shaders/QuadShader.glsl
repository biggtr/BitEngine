#Vertex
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoords;

out vec2 v_TexCoords;
out vec4 v_Color;
uniform mat4 u_ViewProjection;
void main()
{ 
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
    v_Color = a_Color;
    v_TexCoords = a_TexCoords;
}

#Fragment 
out vec4 color;  
in vec2 v_TexCoords;
in vec4 v_Color;
uniform vec4 u_Color;
uniform sampler2D u_TexImage;
void main()
{
    color = vec4(1.0, 0.0,0.0, 1.0 );
}

