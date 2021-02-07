#pragma once

#include "Battery/pch.h"
#include "Battery/Core/Layer.h"

namespace Battery {

	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer() {}

		void OnAttach() final {		// Prevent from being overridden again

			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			io = ImGui::GetIO();
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsClassic();

			// Setup Platform/Renderer backends
			ImGui_ImplAllegro5_Init(app->window.display);

			io = ImGui::GetIO();
			OnImGuiAttach();
		}

		void OnDetach() final {
			io = ImGui::GetIO();
			OnImGuiDetach();
			ImGui_ImplAllegro5_Shutdown();
		}
		
		void OnRender() final {

			// Start ImGui Frame
			ImGui_ImplAllegro5_NewFrame();
			ImGui::NewFrame();

			io = ImGui::GetIO();
			OnImGuiRender();

			// Draw the result on the screen
			ImGui::Render();
			ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
		}

		void OnUpdate() final {
			io = ImGui::GetIO();
			OnImGuiUpdate();
		}

		void OnEvent(Battery::Event* event) final {

			ImGui_ImplAllegro5_ProcessEvent(event->GetAllegroEvent());

			if (event->GetType() == EventType::WindowResize) {
				ImGui_ImplAllegro5_InvalidateDeviceObjects();
				al_acknowledge_resize(app->window.display);
				ImGui_ImplAllegro5_CreateDeviceObjects();
			}

			io = ImGui::GetIO();
			OnImGuiEvent(event);
		}

		virtual void OnImGuiRender() {
			LOG_CORE_WARN("The function ImGuiLayer::OnImGuiRender() was not overridden, you probably want to?");
		}

		// Rename all other functions too to be consistent
		virtual void OnImGuiAttach() {}
		virtual void OnImGuiDetach() {}
		virtual void OnImGuiUpdate() {}
		virtual void OnImGuiEvent(Battery::Event* event) {}

		ImGuiIO& io = dummyIO;

	private:
		ImGuiIO dummyIO;
	};

}
