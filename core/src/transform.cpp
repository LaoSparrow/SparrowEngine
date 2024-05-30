#include "sparrow_engine/transform.hpp"

using namespace SparrowEngine;

void Transform::set_euler_angles(float x, float y, float z) {
    rotation = glm::vec3(glm::radians(x), glm::radians(y), glm::radians(z));
}