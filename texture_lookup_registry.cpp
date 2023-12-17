#include "texture_lookup_registry.h"
#include "texture_loader.h"

texture* texture_lookup_registry::get_texture(const std::string& texture_name)
{
	if (!texture_lookup_.contains(texture_name))
	{
		texture_lookup_[texture_name] = std::shared_ptr<texture>(texture_loader::load_texture(texture_name, GL_RGBA));
	}

	return texture_lookup_[texture_name].get();
}
