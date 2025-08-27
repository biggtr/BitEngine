#Vertex
layout (location = 0) in vec4 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoords;
layout (location = 3) in float a_TexIndex;

out vec2 v_TexCoords;
out vec4 v_Color;
out float v_TexIndex;
uniform mat4 u_ViewProjection;
void main()
{ 
    gl_Position = u_ViewProjection * a_Position;
    v_Color = a_Color;
    v_TexCoords = a_TexCoords;
    v_TexIndex = a_TexIndex;
}

#Fragment 
out vec4 color;  
in vec2 v_TexCoords;
in float v_TexIndex;
in vec4 v_Color;
uniform sampler2D u_Textures[16];
void main()
{

    int texIndex = int(v_TexIndex);
    vec4 texColor = vec4(1.0);
    switch(texIndex)
    {
        case 0:  texColor = texture(u_Textures[0], v_TexCoords); break;
        case 1:  texColor = texture(u_Textures[1], v_TexCoords); break;
        case 2:  texColor = texture(u_Textures[2], v_TexCoords); break;
        case 3:  texColor = texture(u_Textures[3], v_TexCoords); break;
        case 4:  texColor = texture(u_Textures[4], v_TexCoords); break;
        case 5:  texColor = texture(u_Textures[5], v_TexCoords); break;
        case 6:  texColor = texture(u_Textures[6], v_TexCoords); break;
        case 7:  texColor = texture(u_Textures[7], v_TexCoords); break;
        case 8:  texColor = texture(u_Textures[8], v_TexCoords); break;
        case 9:  texColor = texture(u_Textures[9], v_TexCoords); break;
        case 10: texColor = texture(u_Textures[10], v_TexCoords); break;
        case 11: texColor = texture(u_Textures[11], v_TexCoords); break;
        case 12: texColor = texture(u_Textures[12], v_TexCoords); break;
        case 13: texColor = texture(u_Textures[13], v_TexCoords); break;
        case 14: texColor = texture(u_Textures[14], v_TexCoords); break;
        case 15: texColor = texture(u_Textures[15], v_TexCoords); break;
        // case 16: texColor = texture(u_Textures[16], v_TexCoords); break;
        // case 17: texColor = texture(u_Textures[17], v_TexCoords); break;
        // case 18: texColor = texture(u_Textures[18], v_TexCoords); break;
        // case 19: texColor = texture(u_Textures[19], v_TexCoords); break;
        // case 20: texColor = texture(u_Textures[20], v_TexCoords); break;
        // case 21: texColor = texture(u_Textures[21], v_TexCoords); break;
        // case 22: texColor = texture(u_Textures[22], v_TexCoords); break;
        // case 23: texColor = texture(u_Textures[23], v_TexCoords); break;
        // case 24: texColor = texture(u_Textures[24], v_TexCoords); break;
        // case 25: texColor = texture(u_Textures[25], v_TexCoords); break;
        // case 26: texColor = texture(u_Textures[26], v_TexCoords); break;
        // case 27: texColor = texture(u_Textures[27], v_TexCoords); break;
        // case 28: texColor = texture(u_Textures[28], v_TexCoords); break;
        // case 29: texColor = texture(u_Textures[29], v_TexCoords); break;
        // case 30: texColor = texture(u_Textures[30], v_TexCoords); break;
        // case 31: texColor = texture(u_Textures[31], v_TexCoords); break;
        default: texColor = texture(u_Textures[0], v_TexCoords); break;
    }   

    if(texColor.a < 0.1)
        discard;

    color =  texColor * v_Color;
}

