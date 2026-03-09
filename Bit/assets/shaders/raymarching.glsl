#Vertex
layout (location = 0) in vec4 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoords;
layout (location = 3) in float a_TexIndex;

out vec4 v_Color;
void main()
{ 
    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
    v_Color = a_Color;
}

#Fragment 
out vec4 color;  
in vec4 v_Color;
void main()
{
    color =  v_Color;
}

