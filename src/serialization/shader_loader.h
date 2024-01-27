#pragma once

#include <string>
#include <memory>

class shader;

/**
 * @brief Static only class assisting in loading shaders.
 *
 */
class shader_loader
{
public:
    /**
     * @brief Load a shader from a file.
     *
     * @param vertex_shader_path The path to the vertex shader
     * @param fragment_shader_path The path to the fragment shader
     * @return std::unique_ptr<shader> A pointer to the shader
     */
	static std::unique_ptr<shader> load_shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path);

private:
	static std::string read_shader_file(const std::string& shader_path);
};

