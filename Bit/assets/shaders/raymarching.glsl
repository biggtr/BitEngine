#Vertex
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec4 a_Color;
layout (location = 3) in vec2 a_TexCoords;

out vec4 v_Color;
out vec2 v_TexCoords;
void main()
{ 
    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
    v_TexCoords = a_TexCoords;
    v_Color = a_Color;
}

#Fragment
const float MAX_DIST = 64.0;
const float MIN_DIST = 0.001;

out vec4 FragColor;  
in vec4 v_Color;
in vec2 v_TexCoords;
uniform float screenx;
uniform float screeny;

float sdfSphere(vec3 p, vec3 c, float r)
{
    return length(p - c) - r;
}

float map(vec3 p)
{
    return sdfSphere(p, vec3(0.0), 0.5);
}

float rayMarch(vec3 ro, vec3 rd)
{
    float dist = 0.0;
    for(int i = 0; i < 128; i++)
    {
        float d = map(ro + rd * dist);
        if(d < MIN_DIST) break;
        dist += d;
        if(dist > MAX_DIST) break;
    }
    return dist;
}

void main()
{
    vec2 uv = 2.0 * v_TexCoords - 1.0;
    uv.x *= screenx /screeny;

    vec3 ro = vec3(0.0, 0.0, -2.0);
    vec3 rd = normalize(vec3(uv, 1.0));

    float dist = rayMarch(ro, rd);
    vec3 color = dist < MAX_DIST ? vec3(1.0) : vec3(0.0);

    FragColor = vec4(color, 1.0);
}


