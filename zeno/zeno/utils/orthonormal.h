#pragma once

#include <zeno/utils/vec.h>
#include <tuple>

namespace zeno {

struct orthonormal {
    vec3f normal, tangent, bitangent;

    orthonormal() = default;

    explicit orthonormal(vec3f const &normal_)
        : normal(normal_) {
        normal = normalize(normal);

        // https://backend.orbit.dtu.dk/ws/portalfiles/portal/126824972/onb_frisvad_jgt2012_v2.pdf
        if (normal[2] < -0.9999999f) {
            tangent = vec3f(0.0f, -1.0f, 0.0f);
            bitangent = vec3f(-1.0f, 0.0f, 0.0f);
            return;
        }
        float a = 1.0f / (1.0f + normal[2]);
        float b = -normal[0]*normal[1]*a;
        tangent = vec3f(1.0f - normal[0]*normal[0]*a, b, -normal[0]);
        bitangent = vec3f(b, 1.0f - normal[1]*normal[1]*a, -normal[1]);
    }

    orthonormal(vec3f const &normal_, vec3f const &tangent_)
        : normal(normal_), tangent(tangent_) {
        bitangent = cross(normal, tangent);
        bitangent = normalize(bitangent);
        tangent = cross(bitangent, normal);
        //tangent = normalize(tangent);
    }
};

// Get orthonormal basis from surface normal
// https://graphics.pixar.com/library/OrthonormalB/paper.pdf
static std::tuple<vec3f, vec3f> pixarONB(vec3f const &n) {
	vec3f up = std::abs(n[2]) < 0.999f ? vec3f(0.0f, 0.0f, 1.0f) : vec3f(1.0f, 0.0f, 0.0f);
    vec3f b1 = normalize(cross(up, n));
    vec3f b2 = cross(n, b1);
    return {b1, b2};
}

}
