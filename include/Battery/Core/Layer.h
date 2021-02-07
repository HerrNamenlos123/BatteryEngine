#pragma once

#include "Battery/pch.h"
#include "Battery/Core/ApplicationEvents.h"
#include "Battery/Log/Log.h"

namespace Battery {

	// Forward declaration
	class Application;


	class Layer {
	public:
		Layer() {
			debugName = "Unnamed layer";
		}

		Layer(std::string debugName) {
			this->debugName = debugName;
		}

		~Layer() {

		}

		virtual void OnAttach() {}

		virtual void OnDetach() {}

		virtual void OnUpdate() {}

		virtual void OnRender() {}

		virtual void OnEvent(Battery::Event* e) {}

		const std::string& GetDebugName() const {
			return debugName;
		}

		void SetAppPointer(Battery::Application* app) {
			this->app = app;

			if (app == nullptr) {
				LOG_CORE_WARN("Layer: The supplied Application pointer is nullptr!");
			}
		}

	protected:

		std::string debugName;
		Battery::Application* app = nullptr;
	};

}
