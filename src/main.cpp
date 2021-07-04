
#include "Battery/pch.h"
#include "Battery/Core/Application.h"
#include "Battery/Platform/Dialog.h"
#include "Battery/Log/Log.h"

int main(int argc, const char** argv) {

	try {
		LOG_INIT();

		// Create the client application
		LOG_CORE_TRACE("Creating Application");
		std::unique_ptr<Battery::Application> app(Battery::CreateApplication());

		// Save application pointer, so it can be accessed statically and globally
		Battery::Application::SetApplicationPointer(app.get());

		// Start the engine up
		LOG_CORE_TRACE("Application created, running");
		app->Run(argc, argv);

		// Destroy the application
		LOG_CORE_TRACE("Application stopped, destroying");
	}
	catch (const Battery::Exception& e) {
		LOG_CORE_CRITICAL(std::string("Unhandled Battery::Exception from scope main(): ") + e.what());
		Battery::ShowErrorMessageBox(std::string("Unhandled Battery::Exception from scope main(): ") + e.what());
	}
	catch (...) {
		LOG_CORE_CRITICAL("Unhandled exception from scope main(): Unknown type, no further information");
		Battery::ShowErrorMessageBox("Unhandled exception from scope main(): Unknown type, no further information");
	}

	LOG_CORE_TRACE("Application destroyed, main() returned");
}
