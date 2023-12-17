#include "scene_serializer.h"

#include <fstream>

#include "material.h"
#include "sprite.h"
#include "transform.h"

/*
void scene_serializer::serialize(const entt::registry& registry, const std::string& filename)
{
    std::stringstream storage;

    {
	    // output finishes flushing when it goes out of scope
		cereal::JSONOutputArchive output(storage);

        entt::snapshot{registry}
            .get<transform>(output)
            .get<material>(output)
			.get<sprite>(output);
    }

    std::ofstream out_file;
    out_file.open(filename);
    out_file << storage.rdbuf();
}
*/
