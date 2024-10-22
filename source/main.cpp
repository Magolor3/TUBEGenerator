#include <iostream> 
#include <string>
#include <random>
#include <glm/glm.hpp>
#include <vector>
#include "C:/TubeCPP/headfile/tube.h"

int main() {
    // Random parameters generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> length_dist(10.0, 40.0);
    std::uniform_real_distribution<> outer_diameter_dist(2.0, 10.0);
    std::uniform_real_distribution<> wall_thickness_dist(0.1, 1.0);
    std::uniform_real_distribution<> curvature_dist(-1.0, 1.0);

    double length = length_dist(gen);
    double outer_diameter = outer_diameter_dist(gen);
    double wall_thickness = wall_thickness_dist(gen);
    double inner_diameter = outer_diameter - wall_thickness;
    glm::vec3 curvature(curvature_dist(gen), curvature_dist(gen), curvature_dist(gen));

    std::string name = "Tube_" + std::to_string(rd() % 100);

    try {
        // Create Tube object with the correct parameters
        CORE::Tube tube(name, length, outer_diameter, inner_diameter, wall_thickness, curvature);

        // Output Tube's properties
        std::cout << "Tube Name: " << tube.get_name() << std::endl;
        std::cout << "Length: " << tube.get_length() << " units" << std::endl;
        std::cout << "Outer Diameter: " << tube.get_outer_diameter() << " units" << std::endl;
        std::cout << "Inner Diameter: " << tube.get_inner_diameter() << " units" << std::endl;
        std::cout << "Wall Thickness: " << tube.get_wall_thickness() << " units" << std::endl;
        std::cout << "Curvature: (" << tube.get_curvature().x << ", " << tube.get_curvature().y << ", " << tube.get_curvature().z << ")" << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Error creating Tube: " << e.what() << std::endl;
    }

    return 0;
}
