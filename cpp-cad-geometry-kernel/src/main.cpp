#include <iostream>
#include <fstream>
#include <iomanip>
#include "cad_kernel.hpp"

int main() {
    std::cout << "====================================================================" << std::endl;
    std::cout << "  Parametric 3D CAD B-Rep & CSG Solid Geometry Kernel (C++20)" << std::endl;
    std::cout << "====================================================================" << std::endl << std::endl;

    // 1. Instantiate Parametric Primitives
    std::cout << "[1/4] Generating Box Solid (100.0 x 50.0 x 30.0 mm)..." << std::endl;
    CadKernel::MeshSolid box = CadKernel::ParametricSolidBuilder::makeBox(100.0, 50.0, 30.0);
    box.computeProperties();
    std::cout << "      Vertices: " << box.vertices.size()
              << " | Triangles: " << box.triangles.size() << std::endl;
    std::cout << "      Analytical Volume: 150000.00 mm^3" << std::endl;
    std::cout << "      Calculated Signed Volume: " << std::fixed << std::setprecision(2)
              << box.volume << " mm^3" << std::endl;
    std::cout << "      Surface Area: " << box.surfaceArea << " mm^2" << std::endl << std::endl;

    // 2. Instantiate Cylinder Primitive
    std::cout << "[2/4] Generating Cylinder Tool (Radius = 15.0 mm, Height = 40.0 mm)..." << std::endl;
    CadKernel::MeshSolid cylinder = CadKernel::ParametricSolidBuilder::makeCylinder(15.0, 40.0, 32);
    cylinder.computeProperties();
    std::cout << "      Vertices: " << cylinder.vertices.size()
              << " | Triangles: " << cylinder.triangles.size() << std::endl;
    std::cout << "      Calculated Signed Volume: " << cylinder.volume << " mm^3" << std::endl;
    std::cout << "      Surface Area: " << cylinder.surfaceArea << " mm^2" << std::endl << std::endl;

    // 3. Perform CSG Boolean Difference: Part = Box - Cylinder
    std::cout << "[3/4] Executing CSG Boolean Difference (Part = Box - Cylinder)..." << std::endl;
    CadKernel::MeshSolid hollowPart = CadKernel::ParametricSolidBuilder::booleanDifference(box, cylinder);
    hollowPart.computeProperties();
    std::cout << "      Resulting Solid: " << hollowPart.name << std::endl;
    std::cout << "      Remaining Mesh Volume: " << hollowPart.volume << " mm^3" << std::endl;
    std::cout << "      Volume Reduction: " << (box.volume - hollowPart.volume) << " mm^3" << std::endl << std::endl;

    // 4. Export Tessellated Wavefront OBJ
    std::cout << "[4/4] Tessellating and exporting Wavefront OBJ..." << std::endl;
    std::string objData = box.exportObj();
    std::cout << "      Exported OBJ data size: " << objData.size() << " bytes." << std::endl;

    std::cout << std::endl << "[SUCCESS] C++20 CAD Geometry Kernel operational." << std::endl;
    return 0;
}
