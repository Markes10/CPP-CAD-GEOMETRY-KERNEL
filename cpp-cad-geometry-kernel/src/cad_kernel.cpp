#include "cad_kernel.hpp"
#include <sstream>
#include <iostream>

namespace CadKernel {

void MeshSolid::computeProperties() {
    surfaceArea = 0.0;
    volume = 0.0;

    for (const auto& tri : triangles) {
        Vec3 edge1 = tri.v1 - tri.v0;
        Vec3 edge2 = tri.v2 - tri.v0;
        Vec3 crossP = edge1.cross(edge2);
        surfaceArea += 0.5 * crossP.length();

        // Signed volume of tetrahedron formed with origin
        volume += (1.0 / 6.0) * (tri.v0.x * (tri.v1.y * tri.v2.z - tri.v1.z * tri.v2.y) -
                                 tri.v0.y * (tri.v1.x * tri.v2.z - tri.v1.z * tri.v2.x) +
                                 tri.v0.z * (tri.v1.x * tri.v2.y - tri.v1.y * tri.v2.x));
    }
    volume = std::abs(volume);
}

std::string MeshSolid::exportObj() const {
    std::stringstream ss;
    ss << "# Parametric CAD Kernel Export: " << name << "\n";
    for (const auto& v : vertices) {
        ss << "v " << v.x << " " << v.y << " " << v.z << "\n";
    }
    for (size_t i = 0; i < triangles.size(); i++) {
        size_t idx = i * 3 + 1;
        ss << "f " << idx << " " << idx + 1 << " " << idx + 2 << "\n";
    }
    return ss.str();
}

MeshSolid ParametricSolidBuilder::makeBox(double width, double height, double depth) {
    MeshSolid solid;
    solid.name = "Box_Solid";
    double hx = width / 2.0, hy = height / 2.0, hz = depth / 2.0;

    Vec3 p0(-hx, -hy, -hz), p1(hx, -hy, -hz), p2(hx, hy, -hz), p3(-hx, hy, -hz);
    Vec3 p4(-hx, -hy, hz),  p5(hx, -hy, hz),  p6(hx, hy, hz),  p7(-hx, hy, hz);

    auto addQuad = [&](Vec3 a, Vec3 b, Vec3 c, Vec3 d) {
        solid.vertices.push_back(a); solid.vertices.push_back(b); solid.vertices.push_back(c);
        solid.vertices.push_back(a); solid.vertices.push_back(c); solid.vertices.push_back(d);
        solid.triangles.emplace_back(a, b, c);
        solid.triangles.emplace_back(a, c, d);
    };

    // 6 Faces
    addQuad(p0, p1, p2, p3); // Bottom (-Z)
    addQuad(p4, p7, p6, p5); // Top (+Z)
    addQuad(p0, p4, p5, p1); // Front (-Y)
    addQuad(p3, p2, p6, p7); // Back (+Y)
    addQuad(p0, p3, p7, p4); // Left (-X)
    addQuad(p1, p5, p6, p2); // Right (+X)

    solid.computeProperties();
    return solid;
}

MeshSolid ParametricSolidBuilder::makeCylinder(double radius, double height, int segments) {
    MeshSolid solid;
    solid.name = "Cylinder_Solid";
    double hz = height / 2.0;
    Vec3 topCenter(0, 0, hz);
    Vec3 bottomCenter(0, 0, -hz);

    std::vector<Vec3> topRing, bottomRing;
    for (int i = 0; i < segments; i++) {
        double theta = (2.0 * M_PI * i) / segments;
        double x = radius * std::cos(theta);
        double y = radius * std::sin(theta);
        topRing.emplace_back(x, y, hz);
        bottomRing.emplace_back(x, y, -hz);
    }

    for (int i = 0; i < segments; i++) {
        int next = (i + 1) % segments;
        // Top cap
        solid.triangles.emplace_back(topCenter, topRing[i], topRing[next]);
        // Bottom cap
        solid.triangles.emplace_back(bottomCenter, bottomRing[next], bottomRing[i]);
        // Side quad
        solid.triangles.emplace_back(bottomRing[i], topRing[i], topRing[next]);
        solid.triangles.emplace_back(bottomRing[i], topRing[next], bottomRing[next]);
    }

    solid.computeProperties();
    return solid;
}

} // namespace CadKernel
