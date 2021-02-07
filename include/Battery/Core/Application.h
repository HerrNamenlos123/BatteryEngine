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
	extern Application* CreateApplication();

	class Application {
	public:

		Application(int width, int height, const std::string name = BATTERY_DEFAULT_TITLE);
		virtual ~Application();

		virtual bool OnStartup() { return true; }
		virtual void OnUpdate() {}
		virtual void OnRender() {}
		virtual void OnShutdown() {}
		virtual void OnEvent(Battery::Event* e) {}

		void SetFramerate(double f);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void ClearLayerStack();
		void CloseApplication();

	private:
		void Run(int argc, const char** argv);
		friend int ::main(int argc, const char** argv);

		void p_PreUpdate();
		void p_PreRender();
		void p_PostRender();
		void p_MainLoop();
		void p_UpdateApp();
		void p_RenderApp();
		void p_OnEvent(Battery::Event* e);

	// All layers should have access through the application pointer
	public:
		LayerStack layers;
		bool shouldClose = false;

		glm::ivec2 windowSize;
		double frametime = 0;
		double framerate = 0;
		uint32_t framecount = 0;

		AllegroWindow window;
		std::vector<std::string> args;
		double desiredFramerate = 60;
		double oldPreUpdateTime = 0;
	};

}
