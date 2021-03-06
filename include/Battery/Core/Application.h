#pragma once

#include "Battery/pch.h"
#include "Battery/Core/Layer.h"
#include "Battery/Core/LayerStack.h"
#include "Battery/Core/ApplicationEvents.h"
#include "Battery/Core/AllegroContext.h"
#include "Battery/Core/AllegroWindow.h"
#include "Battery/Core/Config.h"
#include "Battery/Core/Event.h"
#include "Battery/Utils/TimeUtils.h"
#include "Battery/Platform/Dialog.h"
#include "Battery/Log/Log.h"

// Forward declaring, so that main can be a friend function
// and gets access to the private Run() function
int main(int argc, const char** argv);

namespace Battery {

	class Application;
	extern Application* CreateApplication();	// This will be defined in the client project

	class Application {
	public:

		Application(int width, int height, const std::string applicationFolderName = BATTERY_DEFAULT_FOLDER_NAME);
		virtual ~Application();

		virtual bool OnStartup() { return true; }
		virtual void OnUpdate() {}
		virtual void OnRender() {}
		virtual void OnShutdown() {}
		virtual void OnEvent(Battery::Event* e) {}

		bool GetKey(int allegroKeycode);
		std::string GetKeyName(int allegroKeycode);
		// TODO: Add keyboard leds cuz why not

		void SetFramerate(double f);
		void SetWindowFlags(int flags);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void ClearLayerStack();
		void CloseApplication();

		static Application* GetApplicationPointer();

		glm::ivec2 GetPrimaryMonitorSize();

	private:
		void Run(int argc, const char** argv);
		static void SetApplicationPointer(Application* app);
		friend int ::main(int argc, const char** argv);

		void _preUpdate();
		void _postUpdate();
		void _preRender();
		void _postRender();
		void _mainLoop();
		void _updateApp();
		void _renderApp();
		void _onEvent(Battery::Event* e);

	// All layers should have access through the application pointer
	public:
		LayerStack layers;
		bool shouldClose = false;

		double frametime = 0;
		double framerate = 0;
		uint32_t framecount = 0;

		AllegroWindow window;
		std::vector<std::string> args;
		double desiredFramerate = 60;
		double oldPreUpdateTime = 0;

	private:
		static Application* applicationPointer;
		std::string applicationFolderName;
		int windowFlags = 0;
	};

}
