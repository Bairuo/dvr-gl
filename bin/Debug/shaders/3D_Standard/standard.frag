#version 330 core

#ifndef ITERATIONS
#define ITERATIONS 400
#endif

uniform sampler3D _Volume;
uniform sampler1D _Transfer;

uniform vec3 viewPos;

in vec3 world;
in vec3 local;

out vec4 fragColor;

struct Ray {
    vec3 origin;
    vec3 dir;
};

struct AABB {
    vec3 aabbMin;
    vec3 aabbMax;
};

float tnear;
float tfar;

bool intersect(Ray r, AABB aabb)
{
    vec3 invR = 1.0f / r.dir;
    vec3 tbot = invR * (aabb.aabbMin - r.origin);
    vec3 ttop = invR * (aabb.aabbMax - r.origin);
    vec3 tmin = min(ttop, tbot);
    vec3 tmax = max(ttop, tbot);
    vec2 t = max(tmin.xx, tmin.yz);
    tnear = max(t.x, t.y);
    t = min(tmax.xx, tmax.yz);
    tfar = min(t.x, t.y);
    return tnear <= tfar;
}

vec3 get_uv(vec3 p)
{
    return vec3(p.x + 0.5f, p.y + 0.5f, p.z + 0.5f);
}

vec4 sample_volume(vec3 uv)
{
    float v = texture(_Volume, uv).r;

    vec4 color = texture(_Transfer, v);

    return color;
}

uniform mat4 model;

void main()
{
    Ray ray;
    ray.origin = local;

    vec3 dir = (world - viewPos);

    mat4 modelTrans = transpose(model);

    ray.dir = normalize(modelTrans * vec4(dir, 0)).xyz;

    AABB aabb;
    aabb.aabbMin = vec3(-0.5f, -0.5f, -0.5f);
    aabb.aabbMax = vec3(0.5f, 0.5f, 0.5f);

    intersect(ray, aabb);

    tnear = max(0.0, tnear);

    vec3 start = ray.origin;
    vec3 end = ray.origin + ray.dir * tfar;
    float dist = abs(tfar - tnear);
    float step_size = dist / float(ITERATIONS);
    vec3 ds = normalize(end - start) * step_size;

    vec4 dst = vec4(0, 0, 0, 0);
    vec3 p = start;

    for (int iter = 0; iter < ITERATIONS; iter++)
    {
        vec3 uv = get_uv(p);
        vec4 src = sample_volume(uv);

        dst = dst + (1 - dst.a) * src;
        dst.a = dst.a + (1 - dst.a) * src.a;

        if(dst.a > 1.0)break;

        p += ds;
    }

    fragColor = dst;
}
