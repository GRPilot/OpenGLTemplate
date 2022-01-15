#include "incs.hpp"

#include "vertex.hpp"

//================================ OFFSETS ================================//

size_t Vertex::getPosOffset() {
    return 0;
}

size_t Vertex::getClrOffset() {
    return getPosSize();
}

size_t Vertex::getTexOffset() {
    return getPosSize() + getClrSize();
}

//================================= SIZES ================================//

size_t Vertex::getPosSize() {
    return getPosCount() * sizeof(glm::vec3::value_type);
}

size_t Vertex::getClrSize() {
    return getClrCount() * sizeof(glm::vec4::value_type);
}

size_t Vertex::getTexSize() {
    return getTexCount() * sizeof(glm::vec2::value_type);
}

//================================ COUNTS ================================//

size_t Vertex::getPosCount() {
    return glm::vec3::length();
}

size_t Vertex::getClrCount() {
    return glm::vec4::length();
}

size_t Vertex::getTexCount() {
    return glm::vec2::length();
}

//================================ STRIDES ===============================//

size_t Vertex::getStride() {
    return getPosSize() + getClrSize() + getTexSize();
}
