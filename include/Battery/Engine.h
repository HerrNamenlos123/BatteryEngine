#pragma once

#include "Battery/pch.h"

#include "Battery/Core/Exception.h"
#include "Battery/AllegroDeps.h"
#include "Battery/PrimitiveRenderer.h"
#include "glm/glm.hpp"

/// <summary>
/// Set the window icon to the embedded icon of the executable. If 'IDI_ICON1' is undefined, make sure to include the (correct!!!)
/// 'resource.h', which is located in the inner project folder of your Visual Studio project. Also make sure that a .ico file is added
/// as a resource in Visual Studio.
/// </summary>
#define SetWindowExecutableIcon() SetWindowIconWindowsID(IDI_ICON1)


enum WINDOW_FLAGS {
	WINDOW_NOFLAGS = 0,
	WINDOW_MAXIMIZED = 1,
	WINDOW_MINIMIZED = 2,
	WINDOW_RESIZABLE = 4
};

namespace Battery {

	class Engine {

		Battery::PrimitiveRenderer primitiveRenderer;

		std::string defaultFontFile = "C:/Windows/Fonts/consola.ttf";
		bool running = true;
		double desiredFramerate = 60;
		bool virtualFunctionCalled = false;
		std::time_t previousUpdate = 0;

	public:
		std::string applicationName;
		uint64_t framecount;
		double frametime;
		double framerate;

		glm::ivec2 mouse;
		glm::ivec2 pmouse;
		glm::ivec2 mouseScroll;

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
		/// and you return false, the engine terminates immediately. This function will only be called when 
		/// Setup(std::vector&lt;std::string&gt; args) was NOT overridden.
		/// </summary>
		/// <returns>bool - Return false to stop the application</returns>
		virtual bool Setup();

		/// <summary>
		/// Engine setup function, called once at startup. Override this from a derived class. When loading fails
		/// and you return false, the engine terminates immediately. Parameter contains command line arguments.
		/// If both Setup() and Setup(std::vector&lt;std::string&gt; args) functions are overridden, only this one
		/// will be called.
		/// </summary>
		/// <param name="args">- The array of command line arguments</param>
		/// <returns>bool - Return false to stop the application</returns>
		virtual bool Setup(std::vector<std::string> args);

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
		/// Called when the engine wants to close. Override this function to intercept, engine will only close when true
		/// is returned.
		/// </summary>
		/// <returns>bool - Return false to intercept closing</returns>
		virtual bool Close();

		/// <summary>
		/// Called when any key is pressed, meant to be used for key events like pressing 'F3' or 'ESC'.
		/// </summary>
		/// <param name="keycode">- The Allegro Keycode, use it with the Allegro defines like ALLEGRO_KEY_A</param>
		/// <param name="modifiers">- The Allegro key modifier, use it with the Allegro defines like ALLEGRO_KEYMOD_SHIFT</param>
		virtual void KeyPressed(int keycode, int modifiers);

		/// <summary>
		/// Called when a key is pressed, meant to be used for typing unicode text.
		/// </summary>
		/// <param name="keycode">- The Allegro Keycode, use it with the Allegro defines like ALLEGRO_KEY_A</param>
		/// <param name="unicode">- The unicode character for typing text</param>
		/// <param name="modifiers">- The Allegro key modifier, use it with the Allegro defines like ALLEGRO_KEYMOD_SHIFT</param>
		/// <param name="repeat">- If the key is held down</param>
		virtual void KeyPressed(int keycode, char unicode, int modifiers, bool repeat);

		/// <summary>
		/// Called when any key is released, meant to be used for key events like pressing 'F3' or 'ESC'.
		/// </summary>
		/// <param name="keycode">- The Allegro Keycode, use it with the Allegro defines like ALLEGRO_KEY_A</param>
		/// <param name="modifiers">- The Allegro key modifier, use it with the Allegro defines like ALLEGRO_KEYMOD_SHIFT</param>
		virtual void KeyReleased(int keycode, int modifiers);
		
		/// <summary>
		/// Called as soon as Engine::Run is called, before anything was done
		/// </summary>
		virtual void PreApplicationFunction();

		/// <summary>
		/// The very last funciton call before Engine::Run exits, in this function the
		/// application is already closed
		/// </summary>
		virtual void PostApplicationFunction();




		/// <summary>
		/// Setup everything, run the engine (view virtual functions for reference) and then shutdown everything.
		/// When this function returns, the application has closed and the process should terminate.
		/// </summary>
		/// <param name="name">- The global name of the application, influences the window title, the process name
		/// and the application folder in %appdata%</param>
		/// <param name="width">- The width of the window at startup</param>
		/// <param name="height">- The height of the window at startup</param>
		/// <param name="argc">- The number of command line arguments</param>
		/// <param name="argv">- The array of command line arguments</param>
		void Run(const std::string& name, int width, int height, int argc, const char** argv, enum WINDOW_FLAGS flags = WINDOW_NOFLAGS);





		/// <summary>
		/// It is encouraged to use the macro SetWindowExecutableIcon() instead of this function.
		/// The supplied id should be IDI_ICON1, but must be evaluated at the compile stage
		/// in your final project. IDI_ICON1 is defined in "resource.h" in the project directory
		/// of your final project.
		/// </summary>
		/// <param name="iconID">- the id of the embedded icon in the executable.</param>
		/// <returns>bool - if loading the icon was successful</returns>
		bool SetWindowIconWindowsID(int iconID);

		/// <summary>
		/// Set the icon of the created Allegro window. Supported file types: bmp, dds, pcx, tga, jpg, jpeg, png.
		/// </summary>
		/// <param name="file">- The relative or absolute path to an image file</param>
		/// <returns>bool - if loading the icon was successful</returns>
		bool SetWindowIcon(const std::string& file);

		/// <summary>
		/// Set the title of the main Allegro window.
		/// </summary>
		/// <param name="title">- The title string</param>
		void SetWindowTitle(const std::string& title);

		/// <summary>
		/// Set the desired framerate in frames per second or fps or Hz.
		/// Default is 60.
		/// </summary>
		/// <param name="framerate">- The framerate</param>
		void SetFramerate(double framerate);


	private:

		void RunMainloop(enum WINDOW_FLAGS flags, std::vector<std::string> args);

		void HandleEvents();
		void PreUpdate();
		void PostUpdate();
	};

}
