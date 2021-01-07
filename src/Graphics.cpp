
#include "Battery/Graphics.h"

// ImGui library
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui.h"
#include "imgui_impl_allegro5.h"

namespace Battery {
	namespace Graphics {

		void DrawBackground(glm::vec3 color) {
			al_clear_to_color(ConvertAllegroColor(color));
		}

		void DrawBackground(glm::vec4 color) {
			al_clear_to_color(ConvertAllegroColor(color));
		}



		ALLEGRO_COLOR ConvertAllegroColor(glm::vec3 color) {
			return al_map_rgb(color.r, color.g, color.b);
		}

		ALLEGRO_COLOR ConvertAllegroColor(glm::vec4 color) {
			return al_map_rgba(color.r, color.g, color.b, color.a);
		}
	}
}
