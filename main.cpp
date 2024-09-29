#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <string>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <omp.h>

// Segment based class
class Segment {
public:
    double length;
    glm::vec3 curvature;

    Segment(double len, const glm::vec3& curv) : length(len), curvature(curv) {}
    virtual ~Segment() = default;
};

// Linear segment subclass
class LinearSegment : public Segment {
public:
    LinearSegment(double len) : Segment(len, glm::vec3(0.0f)) {}
};

// Curved segment subclass
class CurvedSegment : public Segment {
public:
    CurvedSegment(double len, const glm::vec3& curv)
        : Segment(len * 2 * M_PI, curv) {}
};

// Tube class
class Tube {
public:
    std::string name;
    double length;
    double outer_diameter;
    double inner_diameter;
    double wall_thickness;
    glm::vec3 curvature;
    glm::vec3 start_point;
    glm::vec3 start_direction;

    Tube(const std::string& name, double length, double outer_diameter, double inner_diameter, double wall_thickness,
         const glm::vec3& curvature, const glm::vec3& start_point, const glm::vec3& start_direction)
        : name(name), length(length), outer_diameter(outer_diameter), inner_diameter(inner_diameter),
          wall_thickness(wall_thickness), curvature(curvature), start_point(start_point), start_direction(start_direction) {}

    bool operator==(const Tube& other) const {
        return std::tie(name, length, outer_diameter, inner_diameter, wall_thickness, curvature) ==
               std::tie(other.name, other.length, other.outer_diameter, other.inner_diameter, other.wall_thickness, other.curvature);
    }

            std::string to_string() const {
                std::ostringstream oss;
                oss << name << " (Length: " << length << ", Outer Diameter: " << outer_diameter
                    << ", Inner Diameter: " << inner_diameter << ", Wall Thickness: " << wall_thickness
                    << ", Curvature: (" << curvature.x << ", " << curvature.y << ", " << curvature.z << "))";
                return oss.str();
            }

    void compute_curve(std::vector<glm::vec4>& points, const std::vector<std::shared_ptr<Segment>>& segment_data, int base_interpolation_points) {
        glm::vec3 position = start_point;
        glm::vec3 tangent = glm::normalize(start_direction);

        glm::vec3 normal, binormal;
        initialize_frame(tangent, normal, binormal);

        int total_points = base_interpolation_points * segment_data.size();
        int point_index = 0;

        //compute segment of Linear
        #pragma omp parallel for
        for (int s_index = 0; s_index < segment_data.size(); ++s_index) {
            const auto& segment = segment_data[s_index];

            // 动态计算当前段的插值点数
            double curvature_magnitude = glm::length(segment->curvature);
            int dynamic_interpolation_points = std::max(base_interpolation_points, int(base_interpolation_points * curvature_magnitude));
            double ds = segment->length / dynamic_interpolation_points;

            //vec3 settings
            glm::vec3 local_position = position;
            glm::vec3 local_tangent = tangent;
            glm::vec3 local_normal = normal;
            glm::vec3 local_binormal = binormal;

            for (int i = 0; i < dynamic_interpolation_points; ++i) {
                int local_point_index = point_index + i;

                
                points[local_point_index] = glm::vec4(local_position, 1.0f);

                //Location updating
                local_position += ds * local_tangent;

                //Frenet-Serret
                update_frenet_frame(local_tangent, local_normal, local_binormal, segment->curvature, ds);
            }

            #pragma omp critical
            point_index += dynamic_interpolation_points;
        }
    }

private:
    void initialize_frame(const glm::vec3& tangent, glm::vec3& normal, glm::vec3& binormal) const {
        if (glm::dot(tangent, glm::vec3(0, 0, 1)) >= 0.99) {
            normal = glm::normalize(glm::cross(tangent, glm::vec3(1, 0, 0)));
        } else {
            normal = glm::normalize(glm::cross(tangent, glm::vec3(0, 0, 1)));
        }
        binormal = glm::normalize(glm::cross(normal, tangent));
    }

    void update_frenet_frame(glm::vec3& tangent, glm::vec3& normal, glm::vec3& binormal, 
                         const glm::vec3& curvature, double ds) const {
    // 预先缓存一些中间结果
    glm::vec3 curvature_rotated = (curvature.x * tangent) + (curvature.y * normal) + (curvature.z * binormal);

    // 显式转换 ds 为 float 以匹配 glm::vec3
    tangent += static_cast<float>(ds) * curvature_rotated;
    tangent = glm::normalize(tangent);

    // 直接计算法线和副法线，减少重复交叉乘法操作
    normal = glm::cross(binormal, tangent);
    normal = glm::normalize(normal);

    binormal = glm::cross(tangent, normal);
    binormal = glm::normalize(binormal);
}

};

// 检查内外管的组合是否有效
bool valid_combination(const Tube& inner, const Tube& outer) {
    return (outer.inner_diameter > inner.outer_diameter) && (outer.length >= inner.length);
}

int main() {
    std::cout << "Tube Parameter!" << std::endl;

    // 示例数据
    std::vector<std::shared_ptr<Segment>> segment_data = {
        std::make_shared<LinearSegment>(1.0),
        std::make_shared<CurvedSegment>(0.5, glm::vec3(0.1, 0.2, 0.3))
    };

    Tube tube("ExampleTube", 2.0, 1.0, 0.8, 0.1, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0));

    std::vector<glm::vec4> points(100); // 100 插值点
    tube.compute_curve(points, segment_data, 100);

    // 输出计算出的点
    for (const auto& point : points) {
        std::cout << "Point: (" << point.x << ", " << point.y << ", " << point.z << ")" << std::endl;
    }

    return 0;
}

