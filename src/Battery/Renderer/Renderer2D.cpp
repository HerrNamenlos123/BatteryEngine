
#include "Battery/pch.h"
#include "Battery/Renderer/Renderer2D.h"
#include "Battery/Core/Exception.h"
#include "Battery/Core/AllegroContext.h"
#include "Battery/Log/Log.h"
#include "Battery/Core/Config.h"

#ifdef BATTERY_DEBUG
#define CHECK_INIT() \
	if (data == nullptr) { \
		LOG_CORE_ERROR(__FUNCTION__ "(): Renderer is not initialized!"); \
		return;	\
	}
#else
#define CHECK_INIT()
#endif

namespace Battery {

	struct Renderer2DData {
		Renderer2D::Scene* currentScene = nullptr;
	};

	static Renderer2DData* data = nullptr;







	void Renderer2D::Setup() {
		if (data == nullptr) {
			data = new Renderer2DData();
		}
		else {
			LOG_CORE_CRITICAL("Can't setup Renderer2D: Already initialized!");
		}
	}

	void Renderer2D::Shutdown() {
		if (data != nullptr) {
			delete data;
			data = nullptr;
		}
		else {
			LOG_CORE_CRITICAL("Can't shutdown Renderer2D: Not initialized!");
		}
	}

	void Renderer2D::BeginScene(Renderer2D::Scene* scene, const glm::vec4& color) {
		CHECK_INIT();
		LOG_CORE_TRACE(__FUNCTION__ "()");

		// Some sanity checks
		if (data->currentScene != nullptr) {
			LOG_CORE_ERROR(__FUNCTION__ "(): Can't begin scene, another scene is still active!");
			return;
		}

		if (scene == nullptr) {
			LOG_CORE_ERROR(__FUNCTION__ "(): Can't load scene: Supplied scene pointer is null!");
			return;
		}

		if (scene->window == nullptr) {
			LOG_CORE_ERROR(__FUNCTION__ "(): Can't load scene: Supplied AllegroWindow pointer is null!");
			return;
		}

		// Change pointer to the new scene
		data->currentScene = scene;

		// Initialize the canvas for the scene
		al_set_target_backbuffer(data->currentScene->window->allegroDisplayPointer);
		DrawBackground(color);
	}

	void Renderer2D::EndScene() {
		CHECK_INIT();
		LOG_CORE_TRACE(__FUNCTION__ "()");

		if (data->currentScene == nullptr) {
			LOG_CORE_ERROR(__FUNCTION__ "(): Can't end scene: No scene is currently active!");
			return;
		}

		data->currentScene = nullptr;
	}

	void Renderer2D::EndUnfinishedScene() {
		CHECK_INIT();

		if (data->currentScene != nullptr) {
			LOG_CORE_WARN(__FUNCTION__ "(): There is an unfinished scene, make sure to call Renderer2D::EndScene()!");
			EndScene();
		}
	}

	void Renderer2D::DrawBackground(const glm::vec4& color) {
		CHECK_INIT();

		al_clear_to_color(ConvertAllegroColor(color));
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		CHECK_INIT();
		DrawQuad(glm::vec3(position.x, position.y, 0), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		CHECK_INIT();
	}

	ALLEGRO_COLOR Renderer2D::ConvertAllegroColor(const glm::vec4& color) {
		return al_map_rgba(color.r, color.g, color.b, color.a);
	}

}
