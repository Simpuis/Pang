#pragma once

#include <map>
#include <memory>
#include <string>

class texture;

class texture_lookup_registry
{
public:
	texture_lookup_registry() = default;
	~texture_lookup_registry() = default;

	texture* get_texture(const std::string& texture_name);

private:

	std::map<std::string, std::shared_ptr<texture>> texture_lookup_;
};

