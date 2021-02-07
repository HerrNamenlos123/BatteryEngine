
#include "Battery/pch.h"
#include "Battery/Engine.h"
#include "Battery/Core.h"
#include "Battery/Core/Exception.h"
#include "Battery/Utils/TimeUtils.h"
#include "Battery/FileUtils.h"
#include "Battery/StringUtils.h"

// ImGui library
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui.h"
#include "imgui_impl_allegro5.h"

/*


	bool Engine::SetWindowIconWindowsID(int iconID) {
	
		// Load the embedded icon to the Allegro window so no external 
		// icon resource is needed
		HICON icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(iconID));

		if (!icon)
			return false;

		HWND winhandle = al_get_win_window_handle(display);
		SetClassLongPtr(winhandle, GCLP_HICON, (LONG_PTR)icon);
		SetClassLongPtr(winhandle, GCLP_HICONSM, (LONG_PTR)icon);

		return true;
	}

	bool Engine::SetWindowIcon(const std::string& file) {

		std::string e = StringUtils::ToLowerCase(FileUtils::GetExtensionFromPath(file));

		if (e == ".bmp" ||
			e == ".dds" ||
			e == ".pcx" ||
			e == ".tga" ||
			e == ".jpg" ||
			e == ".jpeg" ||
			e == ".png")
		{
			if (!FileUtils::FileExists(file))
				return false;

			ALLEGRO_BITMAP* bitmap = al_load_bitmap(file.c_str());

			if (bitmap == nullptr)
				return false;

			al_set_display_icon(display, bitmap);
			al_destroy_bitmap(bitmap);

			return true;
		}

		throw Battery::Exception("Engine::SetWindowIcon(): The specified icon file has an unsupported file format!");
	}

	void Engine::SetWindowTitle(const std::string& title) {
		al_set_window_title(display, title.c_str());
	}

	void Engine::SetFramerate(double framerate) {
		desiredFramerate = framerate;
	}










// Private

	void Engine::RunMainloop(enum WINDOW_FLAGS flags, std::vector<std::string> args) {

		ALLEGRO_MONITOR_INFO monitor;
		al_get_monitor_info(0, &monitor);
		screenWidth = monitor.x2 - monitor.x1;
		screenHeight = monitor.y2 - monitor.y1;

		std::time_t nextRefresh = TimeUtils::GetMicroseconds();
		while (running) {

			// Handle all events
			HandleEvents();

			// Bound the time to not sleep too long and prevent freezing
			std::time_t frametime = static_cast<std::time_t>(1000000.f / desiredFramerate);
			std::time_t now = TimeUtils::GetMicroseconds();
			TimeUtils::SleepMicroseconds(std::min(std::max(nextRefresh - now, std::time_t(0)), frametime));
			while (TimeUtils::GetMicroseconds() < nextRefresh);

			// Set timeflag for next frame, will be repeated several times if the host can't keep up with the framerate
			while (nextRefresh <= TimeUtils::GetMicroseconds()) {
				nextRefresh += frametime;
			}

			
		}

		primitiveRenderer.Unload();

		// Call user overridden shutdown function
		Shutdown();

	}

	void Engine::HandleEvents() {

		// Handle all pending events

		ALLEGRO_EVENT event;
		while (!al_is_event_queue_empty(events)) {

			// Retrieve event and check if it's valid
			if (al_get_next_event(events, &event)) {

				// Let ImGui know about the event
				ImGui_ImplAllegro5_ProcessEvent(&event);

				switch (event.type) {

				case ALLEGRO_EVENT_KEY_DOWN:
					KeyPressed(event.keyboard.keycode, event.keyboard.modifiers);
					break;

				case ALLEGRO_EVENT_KEY_UP:
					KeyReleased(event.keyboard.keycode, event.keyboard.modifiers);
					break;

				case ALLEGRO_EVENT_KEY_CHAR:
					KeyPressed(event.keyboard.keycode, event.keyboard.unichar, event.keyboard.modifiers, event.keyboard.repeat);
					break;

				default:
					break;
				}
			}
		}
	}

	void Engine::PreUpdate() {
		width = al_get_display_width(display);
		height = al_get_display_height(display);

		framecount++;
		frametime = (TimeUtils::GetMicroseconds() - previousUpdate) / 1000000.f;
		previousUpdate = TimeUtils::GetMicroseconds();
		framerate = 1.f / frametime;

		ALLEGRO_MOUSE_STATE state;
		al_get_mouse_state(&state);
		pmouse = mouse;
		mouse = { state.x, state.y };
		glm::ivec2 scrolled = mouseScroll - glm::ivec2(state.w, state.z);
		mouseScroll = glm::ivec2(state.w, state.z);
		//if (scrolled.x != 0 || scrolled.y != 0)
		//	mouseScrolled(scrolled.x, scrolled.y);


		Graphics::DrawBackground(BATTERY_DEFAULT_BACKGROUND_COLOR);
	}

}
*/