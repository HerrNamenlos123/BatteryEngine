#pragma once

#include <string>
#include "Battery/_AllegroDependencies.h"

enum WINDOW_FLAGS {
	WINDOW_NOFLAGS = 0,
	WINDOW_MAXIMIZED = 1,
	WINDOW_MINIMIZED = 2,
	WINDOW_RESIZABLE = 4
};

namespace Battery {

	class Engine {

		std::string defaultFontFile = "C:/Windows/Fonts/consola.ttf";
		bool running = true;
		float desiredFramerate = 60;

	public:
		std::string title;
		uint64_t framecount;

		int width = 0;
		int height = 0;
		int screenWidth = 0;
		int screenHeight = 0;
		ALLEGRO_DISPLAY* display = nullptr;
		ALLEGRO_FONT* defaultFont = nullptr;
		ALLEGRO_EVENT_QUEUE* events = nullptr;

		Engine();







		// Functions to be overridden by the application

		/// <summary>
		/// Engine setup function, called once at startup. Override this from a derived class. When loading fails
		/// and false is returned, the engine terminates immediately.
		/// </summary>
		/// <returns>bool - Return false to stop the application</returns>
		virtual bool Setup();

		/// <summary>
		/// Engine update function, called at the desired framerate. Override this from a derived class. When false is
		/// returned, the engine terminates.
		/// </summary>
		/// <returns>bool - Return false to stop the application</returns>
		virtual bool Update();

		/// <summary>
		/// Engine shutdown function, called once when the engine is terminated
		/// </summary>
		virtual void Shutdown();

		/// <summary>
		/// Called, when the engine wants to close. Override this function to intercept, engine will only close when true
		/// is returned.
		/// </summary>
		/// <returns>bool - Return false to intercept closing</returns>
		virtual bool Close();









		/// <summary>
		/// Setup everything, run the engine (view virtual functions for reference) and then shutdown everything.
		/// When this function returns, the application has closed and the process should terminate.
		/// </summary>
		/// <param name="name">- The global name of the application, influences the window title, the process name
		/// and the application folder in %appdata%</param>
		/// <param name="width">- The width of the window at startup</param>
		/// <param name="height">- The height of the window at startup</param>
		void Run(const std::string& name, int width, int height, enum WINDOW_FLAGS flags = WINDOW_NOFLAGS);

	private:

		void RunMainloop(enum WINDOW_FLAGS flags);

		void HandleEvents();
		void PreUpdate();
		void PostUpdate();
	};

}
