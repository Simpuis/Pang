#include "renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "material.h"
#include "material_component.h"
#include "sprite.h"
#include "transform.h"

void renderer::render_scene(const material_lookup_registry& material_registry, const entt::registry& registry, GLFWwindow* window) const
{
	glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	auto view = registry.view<transform, sprite, material_component>();
	for(auto entity : view)
	{
		auto [transform_comp, sprite_comp, material_comp] = view.get<transform, sprite, material_component>(entity);

		//const material* mat = material_registry.get_material(material_comp.id);
		const material* mat = material_comp.material.get();

		for(auto& [key, value] : mat->get_texture())
		{
			value->bind(key);
		}
		mat->material_shader->use();
		mat->material_shader->set_matrix("transform", transform_comp.transform_matrix);

		glBindVertexArray(sprite_comp.VAO_);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glUseProgram(0);
	}
}

