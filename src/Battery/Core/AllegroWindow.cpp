
#include "Battery/pch.h"
#include "Battery/Core/AllegroWindow.h"

#undef CreateEvent

namespace Battery {

	AllegroWindow::AllegroWindow(int w, int h) {
		width = w;
		height = h;
	}

	AllegroWindow::~AllegroWindow() {
		if (valid) {
			Destroy();
		}
	}

	void AllegroWindow::Create(int windowFlags) {

		if (valid) {
			LOG_CORE_WARN("Can't create window: Already exists!");
			return;
		}

		if (!al_is_system_installed())
			throw Battery::Exception("Failed to create Allegro window: Allegro is not initialized!");

		// Windows flags
		al_set_new_display_flags(ALLEGRO_PROGRAMMABLE_PIPELINE | ALLEGRO_OPENGL | windowFlags);
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

		display = al_create_display(width, height);
		if (display == nullptr)
			throw Battery::Exception("Failed to create Allegro window");

		defaultFont = al_load_ttf_font(defaultFontFile.c_str(), 64, 0);
		if (defaultFont == nullptr) {
			al_destroy_display(display);
			display = nullptr;
			throw Battery::Exception("Failed to load Allegro font " + defaultFontFile);
		}

		events = al_create_event_queue();
		if (events == nullptr) {
			al_destroy_display(display);
			al_destroy_font(defaultFont);
			display = nullptr;
			defaultFont = nullptr;
			throw Battery::Exception("Failed to create Allegro event queue");
		}

		// Attach event sources
		al_register_event_source(events, al_get_mouse_event_source());
		al_register_event_source(events, al_get_keyboard_event_source());
		al_register_event_source(events, al_get_display_event_source(display));

		LOG_CORE_INFO("Created Allegro window");

		valid = true;
	}

	void AllegroWindow::Destroy() {
		// Destroy everything
		LOG_CORE_INFO("Destroying Allegro window");
		al_destroy_display(display);
		al_destroy_font(defaultFont);
		al_destroy_event_queue(events);
		display = nullptr;
		defaultFont = nullptr;
		events = nullptr;
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
		if (eventCallback != nullptr && events != nullptr) {

			while (!al_is_event_queue_empty(events)) {
				ALLEGRO_EVENT event;

				if (al_get_next_event(events, &event)) {
					for (Event* e : Battery::CreateEvent(&event)) {
						if (e != nullptr) {
							LOG_CORE_INFO(e->GetInfoString());
							eventCallback(e);
						}
						delete e;
					}
				}
			}

		}
	}

}
