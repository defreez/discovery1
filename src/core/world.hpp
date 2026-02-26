#pragma once
#include "math.hpp"
#include <vector>
#include <optional>

namespace core {

struct Solid {
    AABB bounds;
};

struct RayHit {
    float distance;
    Vec3 point;
    Vec3 normal;
};

struct World {
    std::vector<Solid> solids;
    AABB bounds;

    bool is_solid(Vec3 point) const {
        for (const auto& s : solids) {
            if (s.bounds.contains(point)) return true;
        }
        return false;
    }

    bool collides(AABB box) const {
        for (const auto& s : solids) {
            if (s.bounds.intersects(box)) return true;
        }
        return false;
    }

    // Raycast against all solids, returns closest hit
    std::optional<RayHit> raycast(Vec3 origin, Vec3 direction, float max_dist = 1000.0f) const {
        direction = direction.normalized();
        std::optional<RayHit> closest;

        for (const auto& solid : solids) {
            auto hit = ray_vs_aabb(origin, direction, solid.bounds, max_dist);
            if (hit && (!closest || hit->distance < closest->distance)) {
                closest = hit;
            }
        }
        return closest;
    }

    // What can be seen from a point looking in a direction?
    // Returns list of visible surfaces within FOV
    std::vector<RayHit> visible_surfaces(Vec3 origin, Vec3 forward, float fov_h, float fov_v,
                                          int rays_h = 16, int rays_v = 9) const {
        std::vector<RayHit> hits;
        Vec3 right = cross({0, 1, 0}, forward).normalized();
        Vec3 up = cross(forward, right);

        for (int y = 0; y < rays_v; y++) {
            for (int x = 0; x < rays_h; x++) {
                float yaw = (float(x) / (rays_h - 1) - 0.5f) * fov_h;
                float pitch = (float(y) / (rays_v - 1) - 0.5f) * fov_v;

                Vec3 dir = forward + right * std::tan(yaw) + up * std::tan(pitch);
                dir = dir.normalized();

                if (auto hit = raycast(origin, dir)) {
                    hits.push_back(*hit);
                }
            }
        }
        return hits;
    }

private:
    static std::optional<RayHit> ray_vs_aabb(Vec3 origin, Vec3 dir, const AABB& box, float max_dist) {
        float tmin = 0, tmax = max_dist;

        for (int i = 0; i < 3; i++) {
            float o = (i == 0) ? origin.x : (i == 1) ? origin.y : origin.z;
            float d = (i == 0) ? dir.x : (i == 1) ? dir.y : dir.z;
            float bmin = (i == 0) ? box.min.x : (i == 1) ? box.min.y : box.min.z;
            float bmax = (i == 0) ? box.max.x : (i == 1) ? box.max.y : box.max.z;

            if (std::abs(d) < 1e-6f) {
                if (o < bmin || o > bmax) return std::nullopt;
            } else {
                float t1 = (bmin - o) / d;
                float t2 = (bmax - o) / d;
                if (t1 > t2) std::swap(t1, t2);
                tmin = std::max(tmin, t1);
                tmax = std::min(tmax, t2);
                if (tmin > tmax) return std::nullopt;
            }
        }

        if (tmin < 0) return std::nullopt;

        Vec3 point = origin + dir * tmin;

        // Compute normal (which face was hit)
        Vec3 normal{0, 0, 0};
        float eps = 0.001f;
        if (std::abs(point.x - box.min.x) < eps) normal = {-1, 0, 0};
        else if (std::abs(point.x - box.max.x) < eps) normal = {1, 0, 0};
        else if (std::abs(point.y - box.min.y) < eps) normal = {0, -1, 0};
        else if (std::abs(point.y - box.max.y) < eps) normal = {0, 1, 0};
        else if (std::abs(point.z - box.min.z) < eps) normal = {0, 0, -1};
        else if (std::abs(point.z - box.max.z) < eps) normal = {0, 0, 1};

        return RayHit{tmin, point, normal};
    }
};

} // namespace core
