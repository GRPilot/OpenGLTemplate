#include "incs.hpp"
#include "templates.hpp"

#include "vertex.hpp"

namespace {

inline glm::vec4 red() {
    return glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f };
}
inline glm::vec4 green() {
    return glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f };
}
inline glm::vec4 blue() {
    return glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f };
}
inline glm::vec4 black() {
    return glm::vec4{ .0f, .0f, .0f, 1.0f };
}

TemplateGenerator::Template triangle(float scale) {
    glm::vec3 positions[] {
        glm::vec3{ -0.1f * scale, -0.1f * scale, .0f },
        glm::vec3{  0.0f,          0.1f * scale, .0f },
        glm::vec3{  0.1f * scale, -0.1f * scale, .0f }
    };
    std::vector<Vertex> vertices{
        Vertex{ positions[0], red(),   glm::vec2{ 0.0f, 0.0f } },
        Vertex{ positions[1], green(), glm::vec2{ 0.5f, 1.0f } },
        Vertex{ positions[2], blue(),  glm::vec2{ 1.0f, 0.0f } }
    };
    std::vector<GLuint> indices {
        0, 1, 2
    };

    return std::make_pair(vertices, indices);
}

TemplateGenerator::Template square(float scale) {
    glm::vec3 positions[] {
        glm::vec3{ -0.1f * scale, -0.1f * scale, .0f },
        glm::vec3{  0.1f * scale, -0.1f * scale, .0f },
        glm::vec3{  0.1f * scale,  0.1f * scale, .0f },
        glm::vec3{ -0.1f * scale,  0.1f * scale, .0f }
    };
    std::vector<Vertex> vertices{
        Vertex{ positions[0], red(),   glm::vec2{ 0.0f, 0.0f } },
        Vertex{ positions[1], green(), glm::vec2{ 1.0f, 0.0f } },
        Vertex{ positions[2], blue(),  glm::vec2{ 1.0f, 1.0f } },
        Vertex{ positions[3], black(), glm::vec2{ 0.0f, 1.0f } }
    };
    std::vector<GLuint> indices {
        0, 1, 2,
        0, 2, 3
    };

    return std::make_pair(vertices, indices);
}

} // anon namespace
//=============================== GENERATOR ===============================//

TemplateGenerator::Template TemplateGenerator::Generate(TemplateType type, float scale) {
    switch(type) {
        case TemplateType::TRIANGLE:
            return triangle(scale);
        case TemplateType::SQUARE:
            return square(scale);
    }
    return triangle(scale); ///< as default
}
