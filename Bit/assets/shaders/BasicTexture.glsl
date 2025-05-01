#Vertex
layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TextureCoords;

out vec2 o_TextureCoords;
void main()
{ 
    gl_Position = vec4(a_Position, 0.0, 1.0);
    o_TextureCoords = a_TextureCoords;
}

#Fragment 
in vec2 o_TextureCoords;
out vec4 f_FinalColor; 
uniform sampler2D u_TextureImage;
void main()
{
    f_FinalColor = texture(u_TextureImage, o_TextureCoords);
}
