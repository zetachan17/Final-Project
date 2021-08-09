#include "PointLightSource.h"

PointLightSource::PointLightSource(glm::vec3 position, glm::vec3 angles, glm::vec3 color) :
    position(position),
    angles(angles),
    color(color) {
    // some value defined by some smart guys
    constant = 1.0f;
    linear = 0.14f;
    quadratic = 0.07f;
}