#pragma once

#include "Battery/AllegroDeps.h"

// GLM vector library
#include <glm/common.hpp>
#include <glm/glm.hpp>

namespace Battery {
	namespace Graphics {

		enum class LINECAP {
			NONE,
			SQUARE,
			ROUND,
			TRIANGLE
		};

		enum class LINEJOIN {
			NONE,
			BEVEL,
			ROUND,
			MITER
		};

		void DrawBackground(glm::vec3 color);
		void DrawBackground(glm::vec4 color);

		void Fill(glm::vec3 color);
		void Stroke(glm::vec3 color, double thickness);
		void NoFill();
		void NoStroke();
		void UseLineCap(enum class LINECAP linecap);
		void UseLineJoin(enum class LINEJOIN linejoin);

		void DrawLine(glm::vec2 p1, glm::vec2 p2);

		ALLEGRO_COLOR ConvertAllegroColor(glm::vec3 color);
		ALLEGRO_COLOR ConvertAllegroColor(glm::vec4 color);
		glm::vec4 ConvertAllegroColor(ALLEGRO_COLOR color);

	}
}
