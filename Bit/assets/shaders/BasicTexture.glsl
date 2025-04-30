#Vertex
layout (location = 0) in vec2 a_Position;

void main()
{ 
    gl_Position = vec4(a_Position, 0.0, 1.0);
}

#Fragment 
out vec4 f_FinalColor; 
void main()
{
    f_FinalColor = vec4(1.0, 0.0, 0.0, 1.0);
}
