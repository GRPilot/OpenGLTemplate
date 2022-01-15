#include "incs.hpp"

#include "vertex.hpp"

//================================ OFFSETS ================================//

size_t Vertex::GetPosOffset() {
    return 0;
}

size_t Vertex::GetClrOffset() {
    return GetPosSize();
}

size_t Vertex::GetTexOffset() {
    return GetPosSize() + GetClrSize();
}

//================================= SIZES ================================//

size_t Vertex::GetPosSize() {
    return GetPosCount() * sizeof(glm::vec3::value_type);
}

size_t Vertex::GetClrSize() {
    return GetClrCount() * sizeof(glm::vec4::value_type);
}

size_t Vertex::GetTexSize() {
    return GetTexCount() * sizeof(glm::vec2::value_type);
}

//================================ COUNTS ================================//

size_t Vertex::GetPosCount() {
    return glm::vec3::length();
}

size_t Vertex::GetClrCount() {
    return glm::vec4::length();
}

size_t Vertex::GetTexCount() {
    return glm::vec2::length();
}

//================================ STRIDES ===============================//

size_t Vertex::GetStride() {
    return GetPosSize() + GetClrSize() + GetTexSize();
}
