
#include "Battery/pch.h"
#include "Battery/Core/Application.h"
#include "Battery/Platform/Dialog.h"
#include "Battery/Log/Log.h"

int main(int argc, const char** argv) {

	try {
		LOG_INIT();

		LOG_CORE_TRACE("Creating Application");
		Battery::Application* app = Battery::CreateApplication();

		LOG_CORE_TRACE("Application created, running");
		app->Run(argc, argv);

		LOG_CORE_TRACE("Application stopped, destroying");
		delete app;
	}
	catch (const Battery::Exception& e) {
		LOG_CORE_CRITICAL(std::string("Unhandled Battery::Exception from scope main(): ") + e.what());
		Battery::ShowErrorMessageBox(std::string("Unhandled Battery::Exception from scope main(): ") + e.what());
	}

	LOG_CORE_TRACE("Application destroyed");
}
