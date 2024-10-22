//@project CORE

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <set>
#include <numeric>
#include <algorithm>
#include <stdexcept>

namespace CORE
{
    /*!
    * @brief Tube class: Represents a tube with specific properties such as diameter, curvature, etc.
    */
    class Tube{
    public:
        /*!
        *@brief Construct a Tube object with the given parameters.
        */
       Tube(const std::string& name, double length, double outer_diameter, double inner_diameter,
       double wall_thickness, const glm::vec3& curvature)
       : _name(name), _length(length), _outer_diameter(outer_diameter), _inner_diameter(inner_diameter),
       _wall_thickness(wall_thickness), _curvature(curvature) 
       
       {
            if(length <= 0.0 || outer_diameter <= 0.0 || inner_diameter <= 0.0 || wall_thickness <= 0.0) {
                throw std::invalid_argument("All parameters must be positive.");
            }

            if(inner_diameter >= outer_diameter) {
                throw std::invalid_argument("Inner diameter must be smaller than outer diameter.");
            }
       }


       //Getters for the parameters
       std::string get_name() const{ return _name;}
       double get_length() const{ return _length;}
       double get_outer_diameter() const{ return _outer_diameter;}
       double get_inner_diameter() const{ return _inner_diameter;}
       double get_wall_thickness() const{ return _wall_thickness;}
       glm::vec3 get_curvature() const{ return _curvature;}

       /*!
       * @brief Compares two Tubes.
       * @return True if the tubes are identical.
       */
       bool operator==(const Tube& other) const{
                return _name ==other._name && _length == other._length && _outer_diameter == other._outer_diameter && _inner_diameter == other._inner_diameter && _wall_thickness == other._wall_thickness 
                && _curvature == other._curvature;
       }

    private:
        const std::string _name;
        const double _length;
        const double _outer_diameter;
        const double _inner_diameter;
        const double _wall_thickness;
        glm::vec3 _curvature;
    };
}