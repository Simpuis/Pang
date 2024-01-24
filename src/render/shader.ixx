module;

#include <string>
#include <iostream>
#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

export module shader;

/**
 * @brief A class that represents a shader program.
 *
 * This class is responsible for compiling and linking a shader program.
 * It also provides methods for setting uniforms.
 */
export class shader
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

shader::shader(const char* vertex_shader_source, const char* fragment_shader_source)
{
    id_ = 0;
    {
        const unsigned int vertex_id = compile_shader(vertex_shader_source, GL_VERTEX_SHADER);
        const unsigned int fragment_id = compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);

        link(vertex_id, fragment_id);
    }
}

void shader::use() const
{
    glUseProgram(id_);
}

void shader::set_int(const std::string& name, int value) const
{
    const int location = glGetUniformLocation(id_, name.c_str());
    glUniform1i(location, value);
}

void shader::set_vector(const std::string& name, const glm::vec2 vector) const
{
    const int location = glGetUniformLocation(id_, name.c_str());
    glUniform2f(location, vector.x, vector.y);
}

void shader::set_vector(const std::string& name, const glm::vec3 vector) const
{
    const int location = glGetUniformLocation(id_, name.c_str());
    glUniform3f(location, vector.x, vector.y, vector.z);
}

void shader::set_vector(const std::string& name, const glm::vec4 vector) const
{
    const int location = glGetUniformLocation(id_, name.c_str());
    glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}

void shader::set_matrix(const std::string& name, glm::mat4 matrix) const
{
    const int location = glGetUniformLocation(id_, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

unsigned int shader::compile_shader(const char* shader_source, const GLenum shader_type)
{
    const unsigned int shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &shader_source, nullptr);
    glCompileShader(shader_id);
    check_shader_compile_success(shader_id);

    return shader_id;
}

void shader::link(const unsigned vertex_shader, const unsigned fragment_shader)
{
    id_ = glCreateProgram();

    glAttachShader(id_, vertex_shader);
    glAttachShader(id_, fragment_shader);
    glLinkProgram(id_);

    check_program_compile_success(id_);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

bool shader::check_shader_compile_success(const unsigned shader)
{
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, nullptr, info_log);
        std::cout << "Error: Fragment shader compilation failed\n" << info_log << std::endl;
    }

    return success;
}

bool shader::check_program_compile_success(const unsigned program)
{
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char info_log[512];
        glGetProgramInfoLog(program, 512, nullptr, info_log);
        std::cout << "Error: Shader program linking error\n" << info_log << std::endl;
    }

    return success;
}

void shader::set_bool(const std::string & name, bool value) const {
    const int location = glGetUniformLocation(id_, name.c_str());
    glUniform1i(location, value);
}
