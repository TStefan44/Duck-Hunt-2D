#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{
    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, std::vector<glm::vec3> color, bool fill = false);

    // Create circle with given center, radius, number of segments and color
    Mesh* CreatePolygon(const std::string& name, glm::vec3 center, float radius, int segments, glm::vec3 color, bool fill = false);

    // Create star with given center, interior radius, exterior radius, number of spike and color
    Mesh* CreateStar(const std::string& name, glm::vec3 center, float interiorRadius, float exteriorRadius, int spikes, glm::vec3 color, bool fill = false);

    // Create rectangle with given bottom left corner, height, width and color
    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill = false);
    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float width, float height, std::vector<glm::vec3> color, bool fill = false);

    // Create isocel triangle with given bottom left corner, base width, height and color
    Mesh* CreateIsosTriangle(const std::string& name, glm::vec3 leftBottomCorner, float base, float height, glm::vec3 color, bool fill = false);

}
