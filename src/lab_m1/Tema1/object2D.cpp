#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    return object2D::CreateRectangle(name, leftBottomCorner, length, length, color, fill);
}

Mesh* object2D::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    std::vector<glm::vec3> color,
    bool fill)
{
    return object2D::CreateRectangle(name, leftBottomCorner, length, length, color, fill);
}

Mesh* object2D::CreateStar(
    const std::string& name,
    glm::vec3 center,
    float interiorRadius,
    float exetriorRadius,
    int spikes,
    glm::vec3 color,
    bool fill) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Define angle between 2 points
    float angle = RADIANS(360) / ((float)(2 * spikes));
    float radius;

    // place n = segments vertexes + center
    vertices.push_back(VertexFormat(center, color));
    for (int i = 0; i < 2 * spikes; i++) {
        if (i % 2 == 1) {
            radius = interiorRadius;
        }
        else {
            radius = exetriorRadius;
        }

        float x = radius * cos(angle * (float)i);
        float y = radius * sin(angle * (float)i);

        vertices.push_back(VertexFormat(center + glm::vec3(x, y, 0), color));
    }

    Mesh* polygon = new Mesh(name);

    if (!fill) {
        // Stroke and no fill case. Use LINE_LOOP
        for (int i = 1; i <= 2 * spikes; i++) {
            indices.push_back(i);
        }
        polygon->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // No stroke and fill case. Make triangles with format
        // 0, i, i + 1, where 0 is center and i is a vertex
        for (int i = 1; i < 2 * spikes; i++) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back((i + 1));
        }
        // Close loop
        indices.push_back(0);
        indices.push_back(2 * spikes);
        indices.push_back(1);
    }

    polygon->InitFromData(vertices, indices);
    return polygon;
}

Mesh* object2D::CreatePolygon(
    const std::string& name, 
    glm::vec3 center, 
    float radius, 
    int segments, 
    glm::vec3 color,
    bool fill) {

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Define angle between 2 points
    float angle = RADIANS(360) / ((float)segments);

    // place n = segments vertexes + center
    vertices.push_back(VertexFormat(center, color));
    for (int i = 0; i < segments; i++) {
        float x = radius * cos(angle * (float)i);
        float y = radius * sin(angle * (float)i);
        
        vertices.push_back(VertexFormat(center + glm::vec3(x, y, 0), color));
    }

    Mesh* polygon = new Mesh(name);
    
    if (!fill) {
        // Stroke and no fill case. Use LINE_LOOP
        for (int i = 1; i <= segments; i++) {
            indices.push_back(i);
        }
        polygon->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // No stroke and fill case. Make triangles with format
        // 0, i, i + 1, where 0 is center and i is a vertex
        for (int i = 1; i < segments; i++) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back((i + 1));
        }
        // Close loop
        indices.push_back(0);
        indices.push_back(segments);
        indices.push_back(1);
    }

    polygon->InitFromData(vertices, indices);
    return polygon;
}


Mesh* object2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float width, float height,
    glm::vec3 color,
    bool fill) 
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color)
    };

    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    Mesh* rectangle = new Mesh(name);

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* object2D::CreateIsosTriangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float base,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(base, 0, 0), color),
        VertexFormat(corner + glm::vec3(base / 2, height, 0), color)
    };

    std::vector<unsigned int> indices = { 0, 1, 2};

    Mesh* triangle = new Mesh(name);

    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    }

    triangle->InitFromData(vertices, indices);
    return triangle;
}


Mesh* object2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float width, float height,
    std::vector<glm::vec3> color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color[0]),
        VertexFormat(corner + glm::vec3(width, 0, 0), color[1]),
        VertexFormat(corner + glm::vec3(width, height, 0), color[2]),
        VertexFormat(corner + glm::vec3(0, height, 0), color[3])
    };

    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    Mesh* rectangle = new Mesh(name);

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

