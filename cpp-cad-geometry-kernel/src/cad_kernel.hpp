#pragma once

#include <vector>
#include <string>
#include <cmath>
#include <memory>

namespace CadKernel {

struct Vec3 {
    double x, y, z;
    Vec3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    Vec3 operator+(const Vec3& o) const { return Vec3(x + o.x, y + o.y, z + o.z); }
    Vec3 operator-(const Vec3& o) const { return Vec3(x - o.x, y - o.y, z - o.z); }
    Vec3 operator*(double s) const { return Vec3(x * s, y * s, z * s); }
    double dot(const Vec3& o) const { return x * o.x + y * o.y + z * o.z; }
    Vec3 cross(const Vec3& o) const {
        return Vec3(y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x);
    }
    double length() const { return std::sqrt(x * x + y * y + z * z); }
    Vec3 normalized() const {
        double l = length();
        return l > 1e-9 ? (*this) * (1.0 / l) : *this;
    }
};

struct Triangle {
    Vec3 v0, v1, v2;
    Vec3 normal;
    Triangle(Vec3 a, Vec3 b, Vec3 c) : v0(a), v1(b), v2(c) {
        normal = (b - a).cross(c - a).normalized();
    }
};

struct MeshSolid {
    std::string name;
    std::vector<Vec3> vertices;
    std::vector<Triangle> triangles;
    double volume = 0.0;
    double surfaceArea = 0.0;

    void computeProperties();
    std::string exportObj() const;
};

class ParametricSolidBuilder {
public:
    static MeshSolid makeBox(double width, double height, double depth);
    static MeshSolid makeCylinder(double radius, double height, int segments = 32);
    static MeshSolid booleanDifference(const MeshSolid& target, const MeshSolid& tool);
};

} // namespace CadKernel
