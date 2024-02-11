#pragma once

#include <refl.hpp>
#include <glm/vec3.hpp>

#include "serialization/serializable.h"

REFL_TYPE(glm::vec3, bases<>)
        REFL_FIELD(x, serializable())
        REFL_FIELD(y, serializable())
        REFL_FIELD(z, serializable())
REFL_END
