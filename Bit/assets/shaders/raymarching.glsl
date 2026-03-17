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
    return dot(p,n) + h;
}
float sdfSphere(vec3 p, vec3 c, float r)
{
    return (length(p - c) - r);
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
float sdfTerrian(vec3 p)
{
    return p.y - sin(p.x) * sin(p.z);
}
float menger(vec3 p)
{
    float d = sdfBox(p, vec3(1));
    float s = 1.0;

    for(int i = 0; i < 5; ++i)
    {
        vec3 a = mod(p*s, 2.0) - 1.0;
        s *= 3.0;
        vec3 r = abs(1.0 - 3.0*abs(a));
        float da = max(r.x, r.y);
        float db = max(r.y, r.z);
        float dc = max(r.z, r.x);
        float c = (min(da,min(db,dc)) - 1.0) / s;
        d = max(d,c);
    }
    return d;
}

float scene(vec3 p)
{
    vec3 center = vec3(0.0);
    float r = 0.5;
    float plane = sdfPlane(p, vec3(0,1,0), 1.5);
    // float sphere = sdfSphere(p, vec3(0, sin(time * 2), 0.0),2, center, r);
    float capsule = sdfCapsule(vec3(p.x - 2.0,p.y - sin(time * 1.5),p.z - 3), center, vec3(0.0, 3.0f, 0.0), r);
    float torus = sdfTorus(p, vec2(3.5, 1.0));
    float box = sdfBox(p, vec3(1.8));
    float player = sdfSphere(p - (cameraPos + cameraForward * 10.0), vec3(0.5), 1.5);
    float terrian = sdfTerrian(p);
    vec3 q = repeat(p, 1.5);
    float menger = menger(q);
    float d = min(player, menger);
    return menger;
}
float rayMarch(vec3 ro, vec3 rd, float maxDist)
{
    float dist = 0.0;
    for(int i = 0; i < 128; i++)
    {
        vec3 currentPos = ro + rd * dist;
        float d = scene(currentPos);
        
        if(d < MIN_DIST) break;
        dist += d;
        if(dist > maxDist) break;
    }
    return dist;
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

    // //shadow
    // float distToLight = length(lightPosition - currentPos);
    // vec3 ro = currentPos + normal * 0.01;
    // float dist = rayMarch(ro, directionToLight, distToLight);
    // if(dist < distToLight)
    // {
    //     light = light * vec3(0.25);
    // }
    return light;
}

void main()
{
    vec2 uv = 2.0 * v_TexCoords - 1.0;
    uv.x *= screenx /screeny;
    vec2 mouse = (2.0 * mouseInput / vec2(screenx, screeny)) - 1.0; mouse.x *= screenx / screeny;
    // vec3 cameraPos = vec3(0.0, 2.0, -6.0);
    vec3 ro = cameraPos;
    vec3 rd = normalize(uv.x * cameraRight + uv.y * cameraUp + cameraForward);
    vec3 color = vec3(1.0, 1.0, 1.0) * 0.3;
    float d = rayMarch(ro, rd, MAX_DIST);
    if(d >= MAX_DIST)
    {
        FragColor = vec4(color, 1.0);
        return;
    }
    vec3 currentPos = ro + rd * d;
    vec3 lightPosition = vec3(0,10, -3.0);
    vec3 lightColor = vec3(1.0, 1.0, 0.0);
    color += lighting(currentPos, cameraPos, lightPosition, lightColor);

    FragColor = vec4(color, 1.0);
}

























