#version 330 core

#Vertex
layout (location = 0) a_Position;
layout (location = 1) a_TextureCoords;

out vec2 v_TextureCoords;
void main()
{ 
    gl_Position = vec4(vec3(a_Position), 1.0);
    o_TextureCoords = a_TextureCoords;
}

//-----------------------------------------------------

#Fragment 
in vec2 v_TextureCoords;
out vec4 f_FinalColor; 
uniform sampler2D u_TextureImage;
void main()
{
    f_FinalColor = texture(u_TextureImage, v_TextureCoords);
}
