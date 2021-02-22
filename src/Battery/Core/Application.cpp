
#include "Battery/pch.h"
#include "Battery/Core/Application.h"
#include "Battery/Renderer/Renderer2D.h"

namespace Battery {

	Application::Application(int width, int height, const std::string name) : window(width, height) {
	}

	Application::~Application() {
	}

	void Application::Run(int argc, const char** argv) {

		// Initialize the Allegro framework
		if (!AllegroContext::GetInstance()->Initialize("TestApp")) {
			LOG_CORE_WARN("The Application constructor failed, closing application...");
			return;
		}

		// Create Allegro window
		window.Create();
		window.SetEventCallback(std::bind(&Application::_onEvent, this, std::placeholders::_1));

		// Load 2D renderer
		Renderer2D::Setup();

		// Parse command line arguments
		LOG_CORE_TRACE("Command line arguments:");
		for (int i = 0; i < argc; i++) {
			args.push_back(argv[i]);
			LOG_CORE_TRACE(std::string("[") + std::to_string(i) + "]: " + args[i]);
		}

		// Client startup
		try {
			LOG_CORE_INFO("Application created, loading client OnStartup()");
			if (!OnStartup()) {
				LOG_CORE_WARN("Application::OnStartup() returned false, "
					"skipping main loop and shutting engine down...");
				shouldClose = true;
			}
			else {
				LOG_CORE_INFO("Everything loaded, entering main loop");
			}
		}
		catch (const Battery::Exception& e) {
			LOG_CORE_CRITICAL(std::string("Application::OnStartup() threw Battery::Exception: ") + e.what());
			ShowErrorMessageBox(std::string("Application::OnStartup() threw Battery::Exception: ") + e.what());
			LOG_CORE_ERROR("Shutting engine down...");
			shouldClose = true;
		}

		// Client update
		try {
			_mainLoop();
		}
		catch (const Battery::Exception& e) {
			LOG_CORE_CRITICAL(std::string("Some Layer's OnUpdate() function threw Battery::Exception: ") + e.what());
			ShowErrorMessageBox(std::string("Some Layer's OnUpdate() function threw Battery::Exception: ") + e.what());
			LOG_CORE_ERROR("Shutting engine down...");
		}

		// Client shutdown
		try {
			LOG_CORE_INFO("Shutting down: client OnShutdown()");
			OnShutdown();
		}
		catch (const Battery::Exception& e) {
			LOG_CORE_CRITICAL(std::string("Application::OnShutdown() threw Battery::Exception: ") + e.what());
			ShowErrorMessageBox(std::string("Application::OnShutdown() threw Battery::Exception: ") + e.what());
		}

		// Unload 2D renderer
		Renderer2D::Shutdown();

		// Destroy Allegro window
		window.Destroy();

		// Clear layer stack
		LOG_CORE_INFO("Clearing any left over layers from layer stack");
		layers.ClearStack();

		// Shut the allegro framework down
		AllegroContext::GetInstance()->Destroy();
	}

	void Application::_preUpdate() {

		double now = TimeUtils::GetRuntime();

		double frametime = now - oldPreUpdateTime;

		if (oldPreUpdateTime == 0) {
			frametime = 0;
		}

		oldPreUpdateTime = now;
		framerate = 1.0 / frametime;

		// Handle events
		window.HandleEvents();
	}

	void Application::_preRender() {

	}

	void Application::_postRender() {
		Renderer2D::EndUnfinishedScene();
	}

	void Application::_mainLoop() {

		double nextFrame = TimeUtils::GetRuntime();

		while (!shouldClose) {

			// Update everything
			_preUpdate();
			_updateApp();

			// Render everything
			_preRender();
			_renderApp();
			_postRender();

			// Wait for the right time to render
			double desiredFrametime = 1.0 / desiredFramerate;
			while (TimeUtils::GetRuntime() < nextFrame);
			double timeout = TimeUtils::GetRuntime() + 1;	// Timeout of 1s
			while (nextFrame < TimeUtils::GetRuntime() || TimeUtils::GetRuntime() > timeout)
				nextFrame += desiredFrametime;

			if (TimeUtils::GetRuntime() > timeout) {
				nextFrame = TimeUtils::GetRuntime() + desiredFramerate;
			}

			// Show rendered image
			al_flip_display();

		}
	}

	void Application::_updateApp() {

		// First update the base application
		LOG_CORE_TRACE("Application::OnUpdate()");
		OnUpdate();

		// Then propagate through the stack and update all layers sequentially
		for (Layer* layer : layers.GetLayers()) {
			LOG_CORE_TRACE(std::string("Layer '") + layer->GetDebugName() + "' OnUpdate()");
			layer->OnUpdate();
		}
	}

	void Application::_renderApp() {

		// First render the base application
		LOG_CORE_TRACE("Application::OnRender()");
		OnRender();

		// Then propagate through the stack and render all layers sequentially
		for (Layer* layer : layers.GetLayers()) {
			LOG_CORE_TRACE(std::string("Layer '") + layer->GetDebugName() + "' OnUpdate()");
			layer->OnRender();
		}
	}

	void Application::_onEvent(Event* e) {

		// Propagate through the stack in reverse order
		for (size_t i = 0; i < layers.GetLayers().size(); i++) {
			Layer* layer = layers.GetLayers()[layers.GetLayers().size() - i - 1];
			LOG_CORE_TRACE(std::string("Layer '") + layer->GetDebugName() + "' OnEvent()");
			layer->OnEvent(e);

			if (e->WasHandled()) {
				LOG_CORE_TRACE(std::string("Event was handled by Layer '") + layer->GetDebugName() + "'");
				break;
			}
		}

		if (!e->WasHandled()) {
			// Lastly give the event to the base application
			LOG_CORE_TRACE("Application::OnEvent()");
			OnEvent(e);
		}
	}

	void Application::SetFramerate(double f) {
		desiredFramerate = f;
	}

	void Application::PushLayer(Layer* layer) {
		layers.PushLayer(layer, this);
	}

	void Application::PushOverlay(Layer* overlay) {
		layers.PushOverlay(overlay, this);
	}

	void Application::ClearLayerStack() {
		layers.ClearStack();
	}

	void Application::CloseApplication() {
		shouldClose = true;
	}

}
