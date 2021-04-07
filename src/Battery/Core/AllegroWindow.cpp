
#include "Battery/pch.h"
#include "Battery/Core/AllegroWindow.h"
#include "Battery/Core/AllegroContext.h"
#include "Battery/Core/Config.h"
#include "Battery/Utils/TimeUtils.h"

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
		PROFILE_CORE_SCOPE(__FUNCTION__"()");

		if (eventCallback != nullptr && allegroEventQueue != nullptr) {

			while (!al_is_event_queue_empty(allegroEventQueue)) {
				ALLEGRO_EVENT allegroEvent;

				if (al_get_next_event(allegroEventQueue, &allegroEvent)) {

					eventContainer.Load(&allegroEvent);

					if (eventContainer.primaryEventType != Battery::EventType::None) {
						HandleEvent(eventContainer.primaryEvent);
					}
					
					if (eventContainer.secondaryEventType != Battery::EventType::None) {
						HandleEvent(eventContainer.secondaryEvent);
					}
				}
			}

		}
	}

	void AllegroWindow::HandleEvent(Battery::Event* event) {

#ifdef BATTERY_DEBUG
		static const size_t length = 1024;
		char info[length];
		event->GetInfoString(info, length);
		LOG_CORE_INFO("[AllegroWindow]: {}", info);
#endif
		
		if (event->GetType() == Battery::EventType::WindowResize) {
			al_acknowledge_resize(allegroDisplayPointer);
		}

		// Clear keyboard buffer when window loses focus
		if (event->GetType() == Battery::EventType::WindowLostFocus) {
			LOG_CORE_TRACE(__FUNCTION__"(): Window lost focus: Clearing keyboard state");
			al_clear_keyboard_state(allegroDisplayPointer);
		}

		if (eventCallback != nullptr) {
			eventCallback(event);
		}
		else {
			LOG_CORE_TRACE(__FUNCTION__"(): OnEvent callback can't be called: Function pointer is nullptr!");
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

	glm::ivec2 AllegroWindow::GetSize() {
		CHECK_ALLEGRO_INIT();
		return glm::ivec2(GetWidth(), GetHeight());
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

	glm::vec2 AllegroWindow::GetMousePosition() {
		CHECK_ALLEGRO_INIT();
		ALLEGRO_MOUSE_STATE mouse;
		al_get_mouse_state(&mouse);
		return glm::vec2(mouse.x, mouse.y);
	}

	bool AllegroWindow::GetLeftMouseButton() {
		CHECK_ALLEGRO_INIT();
		ALLEGRO_MOUSE_STATE mouse;
		al_get_mouse_state(&mouse);
		return mouse.buttons & 0x01;
	}

	bool AllegroWindow::GetRightMouseButton() {
		CHECK_ALLEGRO_INIT();
		ALLEGRO_MOUSE_STATE mouse;
		al_get_mouse_state(&mouse);
		return mouse.buttons & 0x02;
	}

	bool AllegroWindow::GetMouseWheel() {
		CHECK_ALLEGRO_INIT();
		ALLEGRO_MOUSE_STATE mouse;
		al_get_mouse_state(&mouse);
		return mouse.buttons & 0x03;
	}

	HWND AllegroWindow::GetWinHandle() {
		CHECK_ALLEGRO_INIT();
		return al_get_win_window_handle(allegroDisplayPointer);
	}

	bool AllegroWindow::IsFocused() {
		CHECK_ALLEGRO_INIT();
		return GetForegroundWindow() == GetWinHandle();
	}

	bool AllegroWindow::Focus() {
		CHECK_ALLEGRO_INIT();
		return SetForegroundWindow(GetWinHandle());
	}

	std::string AllegroWindow::GetClipboardContent() {
		char* content = al_get_clipboard_text(allegroDisplayPointer);

		if (content) {
			std::string str = content;
			al_free(content);
			return str;
		}

		return "";
	}

	bool AllegroWindow::SetClipboardContent(const std::string& content) {
		return al_set_clipboard_text(allegroDisplayPointer, content.c_str());
	}

	bool AllegroWindow::HasClipboardContent() {
		return al_clipboard_has_text(allegroDisplayPointer);
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
