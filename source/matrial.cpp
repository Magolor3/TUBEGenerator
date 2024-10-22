#include <iostream>
#include <vector>
#include <string>
#include <cmath> 


struct Material {
    std::string name;               // Material name
    double elastic_modulus;         // Elastic modulus (MPa)
    double yield_strength;          // Yield strength (MPa)
    double hardening_exponent;      // Hardening exponent
};

std::vector<Material> filterMaterials(const std::vector<Material>& materials, double stress, double strain) {
    std::vector<Material> suitableMaterials;

    for (const auto& material : materials) {
        
        double strain_calc = std::pow(stress / material.elastic_modulus, 1.0 / material.hardening_exponent);

        
        if (stress < material.yield_strength && strain <= strain_calc) {
            suitableMaterials.push_back(material);
        }
    }

    return suitableMaterials;
}

int main() {
    
    std::vector<Material> materials = {
        {"Steel", 210000, 250, 0.2},     // Steel
        {"Aluminum", 70000, 200, 0.1},   // Aluminum
    };

    double inputStress, inputStrain;

    // Input stress and strain values
    std::cout << "Enter stress (MPa): ";
    std::cin >> inputStress;
    std::cout << "Enter strain: ";
    std::cin >> inputStrain;

    // Filter suitable materials
    std::vector<Material> suitableMaterials = filterMaterials(materials, inputStress, inputStrain);

    // Output suitable materials
    std::cout << "Suitable materials are:" << std::endl;
    for (const auto& material : suitableMaterials) {
        std::cout << "- " << material.name << std::endl;
    }

    return 0;
}