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
const float MAX_DIST = 128.0;
const float MIN_DIST = 0.001;

out vec4 FragColor;  
in vec4 v_Color;
in vec2 v_TexCoords;
uniform float screenx;
uniform float screeny;
uniform float time;
uniform vec2 mouseInput;
uniform vec3 cameraPos;
uniform vec3 cameraForward;
uniform vec3 cameraRight;
uniform vec3 cameraUp;

float sdfPlane(vec3 p, vec3 n, float h)
{
    return dot(p,n) * h;
}
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

float sdfTorus(vec3 p, vec2 t)
{
    float x = length(p.xz) - t.x;
    return length(vec2(x, p.y)) - t.y;
}
float sdfBox(vec3 p, vec3 s)
{
    return length(max(abs(p) - s,0));
}
float opSmoothUnion( float a, float b, float k )
{
    k *= 4.0;
    float h = max(k-abs(a-b),0.0);
    return min(a, b) - h*h*0.25/k;
}

float opSmoothSubtraction( float a, float b, float k )
{
    return -opSmoothUnion(a,-b,k);

    // k *= 4.0;
    // float h = max(k-abs(-a-b),0.0);
    // return max(-a, b) + h*h*0.25/k;
}

float opSmoothIntersection( float a, float b, float k )
{
    return -opSmoothUnion(-a,-b,k);

    // k *= 4.0;
    // float h = max(k-abs(a-b),0.0);
    // return max(a, b) + h*h*0.25/k;
}
vec3 repeat(vec3 p, float s)
{
    vec3 r = p - s*round(p / s);
    return r;
}
float scene(vec3 p)
{
    vec3 center = vec3(0.0);
    float r = 0.5;
    float plane = sdfPlane(p, vec3(0,1,0), 0.5);
    vec3 q = repeat(p, 15.0);
    float sphere = sdfSphere(q, vec3(0, sin(time * 2), 0.0),2, center, r);
    float capsule = sdfCapsule(vec3(p.x - 2.0,p.y - sin(time * 1.5),p.z - 3), center, vec3(0.0, 3.0f, 0.0), r);
    float torus = sdfTorus(q, vec2(5.5, 1.0));
    float box = sdfBox(q, vec3(1.8));
    
    // d = opSmoothUnion(d, plane, 0.2);
    // d = opSmoothUnion(d, torus, 0.1);

    float d = opSmoothUnion(sphere, box, 0.4);
    d = opSmoothUnion(d, torus, 0.8);
    return d;
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

    float ambientStrength = 0.2;

    vec3 ambient = ambientStrength * lightColor;

    vec3 directionToLight = normalize(lightPosition - currentPos);
    float diffuseIntensity = max(0.0, dot(normal, directionToLight));
    vec3 diffuse = diffuseIntensity * lightColor;

    float specularIntensity = 0.6;
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

    // vec3 cameraPos = vec3(0.0, 2.0, -6.0);
    vec3 ro = cameraPos;
    vec3 rd = normalize(uv.x * cameraRight + uv.y * cameraUp + cameraForward);
    vec3 color = vec3(0.0, 0.0, 0.0);
    float d = rayMarch(ro, rd);
    if(d >= MAX_DIST)
    {
        FragColor = vec4(color, 1.0);
        return;
    }
    vec3 currentPos = ro + rd * d;
    vec3 lightPosition = vec3(mouse.x, mouse.y, -3.0);
    vec3 lightColor = vec3(1.0, 1.0, 0.0);
    color += lighting(currentPos, cameraPos, lightPosition, lightColor);

    FragColor = vec4(color, 1.0);
}




















