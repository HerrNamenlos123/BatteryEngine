
#include "Battery/Engine.h"
#include "Battery/Core.h"
#include "Battery/Exception.h"
#include "Battery/FileUtils.h"
#include "Battery/TimeUtils.h"
#include "Battery/StringUtils.h"

// ImGui library
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui.h"
#include "imgui_impl_allegro5.h"

#include <iostream>



namespace Battery {

	Engine::Engine() {

	}



	bool Engine::Setup() {
		return true;
	}

	bool Engine::Update() {
		return true;
	}

	void Engine::Shutdown() {

	}

	bool Engine::Close() {
		return true;
	}

	void Engine::PreApplicationFunction() {

	}

	void Engine::PostApplicationFunction() {

	}






	void Engine::Run(const std::string& name, int width, int height, enum WINDOW_FLAGS flags) {

		try {
			// Function is run immediately, before anything was done
			PreApplicationFunction();

			title = name;
			this->width = width;
			this->height = height;
			Core::SetApplicationName(title);

			// Initialize the Allegro framework
			Core::Initialize();

			// Set some flags
			if (flags & WINDOW_RESIZABLE)
				al_set_new_display_flags(ALLEGRO_RESIZABLE);

			if (flags & WINDOW_MINIMIZED)
				al_set_new_display_flags(ALLEGRO_MINIMIZED);

			if (flags & WINDOW_MAXIMIZED)
				al_set_new_display_flags(ALLEGRO_MAXIMIZED);

			display = al_create_display(width, height);
			if (display == nullptr)
				throw Exception("Failed to create Allegro window");

			defaultFont = al_load_ttf_font(defaultFontFile.c_str(), 64, 0);
			if (defaultFont == nullptr) {
				al_destroy_display(display);
				throw Exception("Failed to load Allegro font " + defaultFontFile);
			}

			events = al_create_event_queue();
			if (events == nullptr) {
				al_destroy_display(display);
				al_destroy_font(defaultFont);
				throw Exception("Failed to create Allegro event queue");
			}

			// Finally run the main loop until it finishes
			RunMainloop(flags);

			// Destroy everything
			al_destroy_display(display);
			al_destroy_font(defaultFont);
			al_destroy_event_queue(events);

			// Shut the Allegro framework down
			Core::Shutdown();

			// Last function call before Engine::Run returns, can be used after application has closed
			PostApplicationFunction();
		}
		catch (const Exception& e) {
			std::cout << std::string("[BatteryEngine::Run]: -> Battery::Exception: \r\n\"") + e.what() + "\"" << std::endl;
			Core::ShowErrorMessageBox(std::string("[BatteryEngine::Run]: -> Battery::Exception: \r\n\"") + e.what() + "\"");
		}
		catch (const std::exception& e) {
			std::cout << std::string("[BatteryEngine::Run]: -> std::exception: \r\n\"") + e.what() + "\"" << std::endl;
			Core::ShowErrorMessageBox(std::string("[BatteryEngine::Run]: -> std::exception: \r\n\"") + e.what() + "\"");
		}
		catch (...) {
			std::cout << "[BatteryEngine::Run]: -> Unknown exception, no further information" << std::endl;
			Core::ShowErrorMessageBox("[BatteryEngine::Run]: -> Unknown exception, no further information");
		}

		// Allegro shutdown after exception is not necessary, it automatically cleans when main returns
	}










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









// Private

	void Engine::RunMainloop(enum WINDOW_FLAGS flags) {

		ALLEGRO_MONITOR_INFO monitor;
		al_get_monitor_info(0, &monitor);
		screenWidth = monitor.x2 - monitor.x1;
		screenHeight = monitor.y2 - monitor.y1;

		// Register Allegro events
		al_register_event_source(events, al_get_keyboard_event_source());
		al_register_event_source(events, al_get_mouse_event_source());
		al_register_event_source(events, al_get_display_event_source(display));

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer backends
		ImGui_ImplAllegro5_Init(display);

		// Call user overridden setup function
		if (!Setup()) {	// User application failed to load, exit
			return;
		}

		uint32_t nextRefresh = TimeUtils::GetMicroseconds();
		while (running) {

			// Handle all events
			HandleEvents();

			// Wait for the next frame refresh
			while (TimeUtils::GetMicroseconds() < nextRefresh);
			nextRefresh = TimeUtils::GetMicroseconds() + static_cast<uint32_t>(1000000.f / desiredFramerate);

			{
				// Do something before rendering
				PreUpdate();

				// Call user overridden update function
				if (!Update())			// If false is returned, close immediately
					running = false;	

				// Do something after user update
				PostUpdate();

				// Finally flip the backbuffer/display the rendered result
				al_flip_display();
			}
			
		}

		ImGui_ImplAllegro5_Shutdown();

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
					//keyPressed(event.keyboard.keycode, event.keyboard.modifiers);
					break;

				case ALLEGRO_EVENT_KEY_UP:
					//keyReleased(event.keyboard.keycode, event.keyboard.modifiers);
					break;

				case ALLEGRO_EVENT_KEY_CHAR:
					//keyPressed(event.keyboard.keycode, event.keyboard.unichar, event.keyboard.modifiers, event.keyboard.repeat);
					break;

				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					if (Close()) {
						running = false;
					}
					break;

				case ALLEGRO_EVENT_DISPLAY_RESIZE:
					ImGui_ImplAllegro5_InvalidateDeviceObjects();
					al_acknowledge_resize(display);
					ImGui_ImplAllegro5_CreateDeviceObjects();
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

		/*ALLEGRO_MOUSE_STATE state;
		al_get_mouse_state(&state);
		pmouse = mouse;
		mouse = { state.x, state.y };
		glm::ivec2 scrolled = mouseScroll - glm::ivec2(state.w, state.z);
		mouseScroll = glm::ivec2(state.w, state.z);
		if (scrolled.x != 0 || scrolled.y != 0)
			mouseScrolled(scrolled.x, scrolled.y);*/

		// Start ImGui Frame
		ImGui_ImplAllegro5_NewFrame();
		ImGui::NewFrame();
	}

	void Engine::PostUpdate() {

		ImGui::Render();
		ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
	}

}
