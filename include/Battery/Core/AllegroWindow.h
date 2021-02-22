#pragma once

#include "Battery/AllegroDeps.h"
#include "Battery/Log/Log.h"
#include "Battery/Core/Exception.h"
#include "Battery/Core/ApplicationEvents.h"

namespace Battery {

	class AllegroWindow {
	public:
		AllegroWindow(int w, int h);
		~AllegroWindow();

		void Create(int windowFlags = 0);
		void Destroy();
		void SetEventCallback(std::function<void(Battery::Event* event)> eventCallback);
		void HandleEvents();

		ALLEGRO_DISPLAY* allegroDisplayPointer = nullptr;
		ALLEGRO_EVENT_QUEUE* allegroEventQueue = nullptr;
		ALLEGRO_FONT* allegroDefaultFont = nullptr;

	private:
		std::string defaultFontFile = "C:\\Windows\\Fonts\\consola.ttf";

		// These are only for remembering until the window is created
		int width = 0;
		int height = 0;
		bool valid = false;

		std::function<void(Battery::Event* event)> eventCallback = nullptr;
	};

}
