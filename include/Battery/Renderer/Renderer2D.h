#pragma once

#include "Battery/pch.h"
#include "Battery/Core/AllegroContext.h"
#include "Battery/Core/AllegroWindow.h"

namespace Battery {

	class Renderer2D {
	public:

		struct Scene {
			AllegroWindow* window = nullptr;
		};

		static void Setup();
		static void Shutdown();

		static void BeginScene(Renderer2D::Scene* scene, const glm::vec4& color = BATTERY_DEFAULT_BACKGROUND_COLOR);
		static void EndScene();
		static void EndUnfinishedScene();

		static void DrawBackground(const glm::vec4& color);

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static ALLEGRO_COLOR ConvertAllegroColor(const glm::vec4& color);
	};

}
