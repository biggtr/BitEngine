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
uniform float time;
uniform vec2 mouseInput;
float sdfSphere(vec3 p, vec3 translation, float scale, vec3 c, float r)
{
    p -= translation;
    p /= scale;
    return (length(p - c) - r) * scale;
}
float sdfCapsule(vec3 p, vec3 a, vec3 b, float r)
{
    vec3 ap = p - a;
    vec3 ab = b - a;
    float t = dot(ap, ab) / dot(ab, ab);
    t = clamp(t, 0, 1);
    vec3 c = a + t * ab;
    return length(p - c) - r;
}
float scene(vec3 p)
{
    vec3 center = vec3(0.0);
    float r = 0.5;
    float sphere = sdfSphere(p, vec3(sin(time * 2.25) * 2 , 0.0, 0.0),2, center, r);
    float capsule = sdfCapsule(p, center, vec3(0.0, 3.0f, 0.0), r);
    return min(sphere, capsule); 
}
vec3 calculateNormals(in vec3 p)
{
    vec3 smallStep = vec3(0.001, 0.0, 0.0);
    float gradientX = scene(p + smallStep.xyy) - scene(p - smallStep.xyy); //gradient in x p - (0.001,0,0);
    float gradientY = scene(p + smallStep.yxy) - scene(p - smallStep.yxy); //gradient in y p - (0,0.001,0);
    float gradientZ = scene(p + smallStep.yyx) - scene(p - smallStep.yyx); //gradient in z p - (0,0, 0.001);
    vec3 normal = normalize(vec3(gradientX, gradientY, gradientZ));
    return normal;
}
vec3 lighting(vec3 currentPos, vec3 cameraPos, vec3 lightPosition, vec3 lightColor)
{
    vec3 normal = calculateNormals(currentPos);

    float ambientStrength = 0.1;

    vec3 ambient = ambientStrength * lightColor;

    vec3 directionToLight = normalize(lightPosition - currentPos);
    float diffuseIntensity = max(0.0, dot(normal, directionToLight));
    vec3 diffuse = diffuseIntensity * lightColor;

    float specularIntensity = 0.2;
    vec3 viewDir = normalize(cameraPos - currentPos);
    vec3 R = reflect(-directionToLight, normal);
    float shineness = 32;
    float spec = pow(max(dot(viewDir, R), 0.0), shineness);
    vec3 specular = specularIntensity * spec * lightColor;

    vec3 light = specular + diffuse + ambient;
    return light;
}
float rayMarch(vec3 ro, vec3 rd)
{

    float dist = 0.0;
    for(int i = 0; i < 128; i++)
    {
        vec3 currentPos = ro + rd * dist;
        float d = scene(currentPos);
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
    vec2 mouse = (2.0 * mouseInput / vec2(screenx, screeny)) - 1.0;
    mouse.x *= screenx / screeny;

    vec3 cameraPos = vec3(0.0, 0.0, -5.0);
    vec3 ro = cameraPos;
    vec3 rd = normalize(vec3(uv, 1.0));
    vec3 color = vec3(0.0, 0.0, 0.0);
    float d = rayMarch(ro, rd);
    if(d >= MAX_DIST)
    {
        FragColor = vec4(color, 1.0);
        return;
    }
    vec3 currentPos = ro + rd * d;
    vec3 lightPosition = vec3(sin(mouse.x), mouse.y, -3.0);
    vec3 lightColor = vec3(1.0, 1.0, 0.0);
    color += lighting(currentPos, cameraPos, lightPosition, lightColor);

    FragColor = vec4(color, 1.0);
}


















