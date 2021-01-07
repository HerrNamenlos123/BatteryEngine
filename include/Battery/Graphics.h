#pragma once

#include "Battery/_AllegroDependencies.h"

// GLM vector library
#include <glm/common.hpp>
#include <glm/glm.hpp>

namespace Battery {
	namespace Graphics {

		void DrawBackground(glm::vec3 color);
		void DrawBackground(glm::vec4 color);

		ALLEGRO_COLOR ConvertAllegroColor(glm::vec3 color);
		ALLEGRO_COLOR ConvertAllegroColor(glm::vec4 color);

	}
}
