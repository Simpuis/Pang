#pragma once

#include <string>
#include <iostream>
#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * @brief A class that represents a shader program.
 *
 * This class is responsible for compiling and linking a shader program.
 * It also provides methods for setting uniforms.
 */
class shader
{
public:
    /**
     * @brief Construct a new shader object
     *
     * @param vertex_shader_source The source code of the vertex shader. Not the filepath, use shader_loader
     * @param fragment_shader_source The source code of the fragment shader. Not the filepath, use shader_loader
     */
    shader(const char* vertex_shader_source, const char* fragment_shader_source);

    /**
     * @brief Bind the shader program in the OpenGL context.
     *
     */
    void use() const;

    void set_bool(const std::string&, bool) const;
    void set_int(const std::string& name, int value) const;
    void set_float(const std::string& name, int value) const;
    void set_vector(const std::string& name, glm::vec2 vector) const;
    void set_vector(const std::string& name, glm::vec3 vector) const;
    void set_vector(const std::string& name, glm::vec4 vector) const;
    void set_matrix(const std::string& name, glm::mat4 matrix) const;

private:
    static unsigned int compile_shader(const char* shader_source, GLenum shader_type);
    void link(unsigned int vertex_shader, unsigned int fragment_shader);

    static bool check_shader_compile_success(unsigned int shader);
    static bool check_program_compile_success(unsigned int program);

    unsigned int id_;
};
