#pragma once

#include "Battery/AllegroDeps.h"
#include "Battery/Log/Log.h"
#include "Battery/Core/Exception.h"
#include "Battery/Core/ApplicationEvents.h"

/// <summary>
/// Usage: allegroWindow.SetWindowExecutableIcon();
/// Set the window icon to the embedded icon of the executable. If 'IDI_ICON1' is undefined, make sure to include the (correct!!!)
/// 'resource.h', which is located in the inner project folder of your Visual Studio project. Also make sure that a .ico file is added
/// as a resource in Visual Studio.
/// </summary>
#define SetWindowExecutableIcon() __setWindowsIconID(IDI_ICON1)

namespace Battery {

	class AllegroWindow {
	public:
		AllegroWindow(int w, int h);
		~AllegroWindow();

		void Create(int windowFlags = 0);
		void Destroy();
		void SetEventCallback(std::function<void(Battery::Event* event)> eventCallback);
		void HandleEvents();

		glm::ivec2 GetScreenPosition();
		void SetScreenPosition(const glm::ivec2& position);
		int GetWidth();
		int GetHeight();
		void SetSize(const glm::vec2 size);
		void SetTitle(const std::string title);
		void Maximize();
		void Minimize();
		void Restore();
		// TODO: Clipboard support

		/// <summary>
		/// It is encouraged to use the macro SetWindowExecutableIcon() instead of this function.
		/// The supplied id should be IDI_ICON1, but must be evaluated at the compile stage
		/// in your final project. IDI_ICON1 is defined in "resource.h" in the project directory
		/// of your final project.
		/// </summary>
		/// <param name="iconID">- the id of the embedded icon in the executable.</param>
		/// <returns>bool - if loading the icon was successful</returns>
		bool __setWindowsIconID(int iconID);

		ALLEGRO_DISPLAY* allegroDisplayPointer = nullptr;
		ALLEGRO_EVENT_QUEUE* allegroEventQueue = nullptr;
		ALLEGRO_FONT* allegroDefaultFont = nullptr;

	private:
		std::string defaultFontFile = "C:\\Windows\\Fonts\\consola.ttf";

		// These are only for remembering the size until the window is created
		int width = 0;
		int height = 0;
		bool valid = false;

		std::function<void(Battery::Event* event)> eventCallback = nullptr;
	};

}
