
#include "Battery/pch.h"
#include "Battery/Core/AllegroWindow.h"
#include "Battery/Core/AllegroContext.h"

#undef CreateEvent

#define CHECK_ALLEGRO_INIT() \
	if (!Battery::AllegroContext::GetInstance()->IsInitialized()) {	\
		throw Battery::Exception(__FUNCTION__ "(): Allegro Context is not initialized!");	\
	}

namespace Battery {

	AllegroWindow::AllegroWindow(int w, int h) {
		width = max(w, BATTERY_MIN_WINDOW_WIDTH);
		height = max(h, BATTERY_MIN_WINDOW_HEIGHT);
	}

	AllegroWindow::~AllegroWindow() {
		if (valid) {
			Destroy();
		}
	}

	void AllegroWindow::Create(int windowFlags) {
		CHECK_ALLEGRO_INIT();

		if (valid) {
			LOG_CORE_WARN("Can't create window: Already exists!");
			return;
		}

		if (!al_is_system_installed())
			throw Battery::Exception("Failed to create Allegro window: Allegro is not initialized!");

		// Windows flags
		al_set_new_display_flags(ALLEGRO_PROGRAMMABLE_PIPELINE | ALLEGRO_OPENGL | windowFlags);
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

		allegroDisplayPointer = al_create_display(width, height);
		if (allegroDisplayPointer == nullptr)
			throw Battery::Exception("Failed to create Allegro window");

		allegroDefaultFont = al_load_ttf_font(defaultFontFile.c_str(), 64, 0);
		if (allegroDefaultFont == nullptr) {
			al_destroy_display(allegroDisplayPointer);
			allegroDisplayPointer = nullptr;
			throw Battery::Exception("Failed to load Allegro font " + defaultFontFile);
		}

		allegroEventQueue = al_create_event_queue();
		if (allegroEventQueue == nullptr) {
			al_destroy_display(allegroDisplayPointer);
			al_destroy_font(allegroDefaultFont);
			allegroDisplayPointer = nullptr;
			allegroDefaultFont = nullptr;
			throw Battery::Exception("Failed to create Allegro event queue");
		}

		// Attach event sources
		al_register_event_source(allegroEventQueue, al_get_mouse_event_source());
		al_register_event_source(allegroEventQueue, al_get_keyboard_event_source());
		al_register_event_source(allegroEventQueue, al_get_display_event_source(allegroDisplayPointer));

		// Limit window size
		if (!al_set_window_constraints(allegroDisplayPointer, BATTERY_MIN_WINDOW_WIDTH, BATTERY_MIN_WINDOW_HEIGHT, 0, 0)) {
			LOG_CORE_WARN(__FUNCTION__ "(): Window constraints could not be set!");
		}
		al_apply_window_constraints(allegroDisplayPointer, true);

		LOG_CORE_TRACE("Created Allegro window");

		SetTitle(BATTERY_DEFAULT_TITLE);

		valid = true;
	}

	void AllegroWindow::Destroy() {
		// Destroy everything
		LOG_CORE_TRACE("Destroying Allegro window");
		al_destroy_display(allegroDisplayPointer);
		al_destroy_font(allegroDefaultFont);
		al_destroy_event_queue(allegroEventQueue);
		allegroDisplayPointer = nullptr;
		allegroDefaultFont = nullptr;
		allegroEventQueue = nullptr;
		valid = false;
	}

	void AllegroWindow::SetEventCallback(std::function<void(Battery::Event* event)> eventCallback) {
		if (eventCallback != nullptr) {
			this->eventCallback = eventCallback;
		}
		else {
			LOG_CORE_ERROR("AllegroWindow: Event Callback was nullptr!");
		}
	}

	void AllegroWindow::HandleEvents() {
		CHECK_ALLEGRO_INIT();

		if (eventCallback != nullptr && allegroEventQueue != nullptr) {

			while (!al_is_event_queue_empty(allegroEventQueue)) {
				ALLEGRO_EVENT event;

				if (al_get_next_event(allegroEventQueue, &event)) {
					for (Event* e : Battery::CreateEvent(&event)) {
						if (e != nullptr) {
							LOG_CORE_INFO(std::string("[AllegroWindow]: ") + e->GetInfoString());

							if (e->GetType() == Battery::EventType::WindowResize) {
								al_acknowledge_resize(allegroDisplayPointer);
							}

							eventCallback(e);
						}
						delete e;
					}
				}
			}

		}
	}

	glm::ivec2 AllegroWindow::GetScreenPosition() {
		CHECK_ALLEGRO_INIT();
		glm::ivec2 position;
		al_get_window_position(allegroDisplayPointer, &position.x, &position.y);
		return position;
	}

	void AllegroWindow::SetScreenPosition(const glm::ivec2& position) {
		CHECK_ALLEGRO_INIT();
		al_set_window_position(allegroDisplayPointer, position.x, position.y);
	}

	int AllegroWindow::GetWidth() {
		CHECK_ALLEGRO_INIT();
		return al_get_display_width(allegroDisplayPointer);
	}

	int AllegroWindow::GetHeight() {
		CHECK_ALLEGRO_INIT();
		return al_get_display_height(allegroDisplayPointer);
	}

	void AllegroWindow::SetSize(const glm::vec2 size) {
		CHECK_ALLEGRO_INIT();
		al_resize_display(allegroDisplayPointer, 
			max(size.x, BATTERY_MIN_WINDOW_WIDTH), max(size.y, BATTERY_MIN_WINDOW_HEIGHT));
	}

	void AllegroWindow::SetTitle(const std::string title) {
		CHECK_ALLEGRO_INIT();
		al_set_window_title(allegroDisplayPointer, title.c_str());
	}

	void AllegroWindow::Maximize() {
		CHECK_ALLEGRO_INIT();
		al_set_display_flag(allegroDisplayPointer, ALLEGRO_MAXIMIZED, true);
	}

	void AllegroWindow::Minimize() {
		CHECK_ALLEGRO_INIT();
		al_set_display_flag(allegroDisplayPointer, ALLEGRO_MINIMIZED, true);
	}

	void AllegroWindow::Restore() {
		CHECK_ALLEGRO_INIT();
		al_set_display_flag(allegroDisplayPointer, ALLEGRO_MAXIMIZED, false);
		al_set_display_flag(allegroDisplayPointer, ALLEGRO_MINIMIZED, false);
	}



// Pseudo-Private: accessible, but not encouraged to use

	bool AllegroWindow::__setWindowsIconID(int iconID) {
		CHECK_ALLEGRO_INIT();

		// Load the embedded icon to the Allegro window so no external 
		// icon resource is needed
		HICON icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(iconID));

		if (!icon)
			return false;

		HWND winhandle = al_get_win_window_handle(allegroDisplayPointer);
		SetClassLongPtr(winhandle, GCLP_HICON, (LONG_PTR)icon);
		SetClassLongPtr(winhandle, GCLP_HICONSM, (LONG_PTR)icon);

		return true;
	}

}
