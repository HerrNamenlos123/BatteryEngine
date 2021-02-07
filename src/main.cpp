
#include "Battery/pch.h"
#include "Battery/Core/Application.h"
#include "Battery/Log/Log.h"

int main(int argc, const char** argv) {

	LOG_INIT();

	LOG_CORE_TRACE("Creating Application");
	Battery::Application* app = Battery::CreateApplication();

	LOG_CORE_TRACE("Application created, running");
	app->Run(argc, argv);

	LOG_CORE_TRACE("Application stopped, destroying");
	delete app;

	LOG_CORE_TRACE("Application destroyed");
}
