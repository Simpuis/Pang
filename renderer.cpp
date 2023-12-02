#include "renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "material.h"
#include "sprite.h"
#include "transform.h"

void renderer::render_scene(const entt::registry& registry, GLFWwindow* window) const
{
	glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	auto view = registry.view<transform, sprite, material>();
	for(auto entity : view)
	{
		auto [transform_comp, sprite_comp, material_comp] = view.get<transform, sprite, material>(entity);


		for(auto& [key, value] : material_comp.get_texture())
		{
						value->bind(key);
		}
		material_comp.material_shader->use();
		material_comp.material_shader->set_matrix("transform", transform_comp.transform_matrix);

		glBindVertexArray(sprite_comp.VAO_);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glUseProgram(0);
	}


	glfwSwapBuffers(window);
}

