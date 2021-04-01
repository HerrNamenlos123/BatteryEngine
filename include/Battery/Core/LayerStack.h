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
			if (layers.size() > 0)
				ClearStack();
		}

		void PushLayer(std::unique_ptr<Layer> layer, Battery::Application* app) {
			LOG_CORE_TRACE("Pushing Layer '" + layer->GetDebugName() + "' onto Layer Stack");
			auto it = layers.insert(layers.begin() + layerNum, std::move(layer));
			(*it)->SetAppPointer(app);
			LOG_CORE_TRACE("Layer '{}' OnAttach()", (*it)->GetDebugName().c_str());
			(*it)->OnAttach();
			layerNum++;
		}
		
		//void PopLayer(Layer* layer) {
		//	LOG_CORE_TRACE("Popping Layer '" + layer->GetDebugName() + "' from Layer Stack");
		//	auto it = std::find(layers.begin(), layers.end(), layer);
		//	if (it != layers.end()) {
		//		(*it)->OnDetach();
		//		layers.erase(it);
		//		layerNum--;
		//	}
		//}
		
		void PushOverlay(std::unique_ptr<Layer> overlay, Battery::Application* app) {
			LOG_CORE_TRACE("Pushing Overlay '" + overlay->GetDebugName() + "' onto Layer Stack");
			layers.push_back(std::move(overlay));
			layers[layers.size() - 1]->SetAppPointer(app);
			LOG_CORE_TRACE("Layer '{}' OnAttach()", layers[layers.size() - 1]->GetDebugName().c_str());
			layers[layers.size() - 1]->OnAttach();
		}
		
		//void PopOverlay(Layer* overlay) {
		//	LOG_CORE_TRACE("Popping Overlay '" + overlay->GetDebugName() + "' from Layer Stack");
		//	auto it = std::find(layers.begin(), layers.end(), overlay);
		//	if (it != layers.end()) {
		//		(*it)->OnDetach();
		//		layers.erase(it);
		//	}
		//}

		const std::vector<std::unique_ptr<Layer>>& GetLayers() const {
			return layers;
		}

		void ClearStack() {
			LOG_CORE_TRACE("Popping all left over layers from Layer Stack");

			while (layers.size() > 0) {
				LOG_CORE_TRACE("Layer '{}' OnDetach()", layers[layers.size() - 1]->GetDebugName().c_str());
				layers[layers.size() - 1]->OnDetach();
				layers.pop_back();
			}
		}

	private:
		std::vector<std::unique_ptr<Layer>> layers;
		size_t layerNum = 0;
	};

}
