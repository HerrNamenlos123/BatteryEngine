#pragma once

#include "Battery/pch.h"
#include "Battery/Core/Layer.h"
#include "Battery/Log/Log.h"

namespace Battery {

	// Forward declaration
	class Application;


	class LayerStack {
	public:
		LayerStack() {}

		~LayerStack() {
			ClearStack();
		}

		void PushLayer(Layer* layer, Battery::Application* app) {
			LOG_CORE_TRACE("Pushing Layer '" + layer->GetDebugName() + "' onto Layer Stack");
			auto it = layers.insert(layers.begin() + layerNum, layer);
			(*it)->SetAppPointer(app);
			(*it)->OnAttach();
			layerNum++;
		}

		void PopLayer(Layer* layer) {
			LOG_CORE_TRACE("Popping Layer '" + layer->GetDebugName() + "' from Layer Stack");
			auto it = std::find(layers.begin(), layers.end(), layer);
			if (it != layers.end()) {
				(*it)->OnDetach();
				delete* it;
				layers.erase(it);
				layerNum--;
			}
		}

		void PushOverlay(Layer* overlay, Battery::Application* app) {
			LOG_CORE_TRACE("Pushing Overlay '" + overlay->GetDebugName() + "' onto Layer Stack");
			layers.push_back(overlay);
			layers[layers.size() - 1]->SetAppPointer(app);
			layers[layers.size() - 1]->OnAttach();
		}

		void PopOverlay(Layer* overlay) {
			LOG_CORE_TRACE("Popping Overlay '" + overlay->GetDebugName() + "' from Layer Stack");
			auto it = std::find(layers.begin(), layers.end(), overlay);
			if (it != layers.end()) {
				(*it)->OnDetach();
				delete* it;
				layers.erase(it);
			}
		}

		const std::vector<Layer*>& GetLayers() const {
			return layers;
		}

		void ClearStack() {
			for (int i = (int)layers.size() - 1; i >= 0; i--)
				PopLayer(layers[i]);
		}

	private:
		std::vector<Layer*> layers;
		size_t layerNum = 0;
	};

}
