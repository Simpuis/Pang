#include "name.h"

#include <tiny_gltf.h>

void name::serialize(tinygltf::Model &model, tinygltf::Node *node) const {
    node->name = name_string;
}
