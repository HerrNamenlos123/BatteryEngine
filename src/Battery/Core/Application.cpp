
#include "Battery/pch.h"
#include "Battery/Core/Application.h"
#include "Battery/Renderer/Renderer2D.h"

namespace Battery {

	// Global static application pointer
	Application* Application::applicationPointer = nullptr;


	Application::Application(int width, int height, const std::string applicationFolderName) : window(width, height) {
		this->applicationFolderName = applicationFolderName;
	}

	Application::~Application() {
	}










	bool Application::GetKey(int allegroKeycode) {
		ALLEGRO_KEYBOARD_STATE keyboard;
		al_get_keyboard_state(&keyboard);
		return al_key_down(&keyboard, allegroKeycode);
	}

	std::string Application::GetKeyName(int allegroKeycode) {
		return al_keycode_to_name(allegroKeycode);
	}











	void Application::Run(int argc, const char** argv) {

		// Initialize the Allegro framework
		if (!AllegroContext::GetInstance()->Initialize(applicationFolderName)) {
			LOG_CORE_WARN("The Allegro context failed to initialize, closing application...");
			return;
		}

		// Create Allegro window
		window.Create(windowFlags);
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
		LOG_CORE_TRACE("Shutting down 2D Renderer");
		Renderer2D::Shutdown();

		// Destroy Allegro window
		window.Destroy();

		// Clear layer stack
		LOG_CORE_TRACE("Clearing any left over layers from layer stack");
		layers.ClearStack();

		// Shut the allegro framework down
		LOG_CORE_TRACE("Destroying Allegro context");
		AllegroContext::GetInstance()->Destroy();

		LOG_CORE_INFO("Application stopped");
	}

	void Application::_preUpdate() {
		double now = TimeUtils::GetRuntime();

		frametime = now - oldPreUpdateTime;

		if (oldPreUpdateTime == 0) {
			frametime = 0.f;
		}

		oldPreUpdateTime = now;
		if (frametime != 0.f)
			framerate = 1.0 / frametime;

		// Handle events
		window.HandleEvents();
	}

	void Application::_postUpdate() {
		framecount++;
	}

	void Application::_preRender() {
		// Paint the background by default
		Renderer2D::DrawBackground(BATTERY_DEFAULT_BACKGROUND_COLOR);
	}

	void Application::_postRender() {
		Renderer2D::EndUnfinishedScene();
	}

	void Application::_mainLoop() {

		double nextFrame = TimeUtils::GetRuntime();

		while (!shouldClose) {

			LOG_CORE_TRACE("Main loop started");

			// Update everything
			_preUpdate();
			_updateApp();
			_postUpdate();

			// Render everything
			_preRender();
			_renderApp();
			_postRender();

			// Wait for the right time to render
			double desiredFrametime = 1.0 / desiredFramerate;
			LOG_CORE_TRACE("Waiting for frametime before flipping screen");
			TimeUtils::Sleep(nextFrame - TimeUtils::GetRuntime());
			while (TimeUtils::GetRuntime() < nextFrame);	// Shouldn't do anything, for safety if sleeping
															// was not successful

			// Set time for next frame
			double now = TimeUtils::GetRuntime();
			if (now <= nextFrame + desiredFrametime) {	// Everything is on time, just increment for
				nextFrame += desiredFrametime;			// maximum accuracy in framerate
			}
			else {	// Too slow, a frame was missed, increment from timepoint now, but framerate will
				nextFrame = now + desiredFrametime;					// not be 100% accurate
			}

			// Show rendered image
			LOG_CORE_TRACE("Flipping displays");
			al_set_current_opengl_context(window.allegroDisplayPointer);
			al_flip_display();

			LOG_CORE_TRACE("Main loop finished");

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
			LOG_CORE_TRACE(std::string("Layer '") + layer->GetDebugName() + "' OnRender()");
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

	void Application::SetWindowFlags(int flags) {
		this->windowFlags = flags;
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

	Application* Application::GetApplicationPointer() {
		return applicationPointer;
	}

	glm::ivec2 Application::GetPrimaryMonitorSize() {
		ALLEGRO_MONITOR_INFO monitor;
		al_get_monitor_info(0, &monitor);
		return glm::ivec2(monitor.x2 - monitor.x1, monitor.y2 - monitor.y1);
	}









// Private

	void Application::SetApplicationPointer(Application* app) {
		applicationPointer = app;
	}

}
