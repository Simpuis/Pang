#include "renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "material.h"
#include "material_component.h"
#include "sprite.h"
#include "transform.h"
#include "mesh.h"

void renderer::render_scene(const entt::registry& registry,
	GLFWwindow* window) const
{
	glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	auto view = registry.view<transform, mesh>();
	for(auto entity : view)
	{
		auto [transform_comp, mesh_comp] = view.get<transform, mesh>(entity);

        for(auto& primitive : mesh_comp.primitives) {
            const material *mat = primitive.mat.get();

            for (auto &[key, value]: mat->get_texture()) {
                value->bind(key);
            }
            mat->material_shader->use();
            mat->material_shader->set_matrix("model", transform_comp.transform_matrix);

            glm::mat4 view_matrix = glm::mat4x4(1.0f);
            view_matrix = glm::translate(view_matrix, glm::vec3(0.0f, 0.0f, -5.0f));
            mat->material_shader->set_matrix("view", view_matrix);

            glm::mat4 projection_matrix;
            projection_matrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
            mat->material_shader->set_matrix("projection", projection_matrix);

            glBindVertexArray(primitive.VAO);
            //glDrawElements(GL_TRIANGLES, primitive.count, GL_UNSIGNED_INT, (char*)NULL + primitive.byte_offset);
            glDrawElements(GL_TRIANGLES, primitive.count, GL_UNSIGNED_SHORT, nullptr);

            glBindVertexArray(0);
            glUseProgram(0);
        }
	}
}
