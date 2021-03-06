#pragma once

#include "Battery/pch.h"

namespace Battery {

	template<typename... Targs>
	class StaticImGuiWindow {
	public:

		bool isMouseOnWindow = false;
		glm::vec2 windowPosition;
		glm::vec2 windowSize;
		std::string name;

		StaticImGuiWindow(const std::string& _name, glm::vec2 position, glm::vec2 size, ImGuiWindowFlags flags = 
			ImGuiWindowFlags_NoTitleBar | 
			ImGuiWindowFlags_NoMove | 
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse
		) {
			name = _name;

			windowPosition = position;
			windowSize = size;
			firstUpdate = true;

			if ((flags & ImGuiWindowFlags_NoTitleBar) > 0) imgui_no_titlebar = true;
			if ((flags & ImGuiWindowFlags_NoScrollbar) > 0) imgui_no_scrollbar = true;
			if ((flags & ImGuiWindowFlags_NoMove) > 0) imgui_no_move = true;
			if ((flags & ImGuiWindowFlags_NoResize) > 0) imgui_no_resize = true;
			if ((flags & ImGuiWindowFlags_MenuBar) > 0) imgui_no_menu = false;
			if ((flags & ImGuiWindowFlags_NoCollapse) > 0) imgui_no_collapse = true;
			if ((flags & ImGuiWindowFlags_NoNav) > 0) imgui_no_nav = true;
			if ((flags & ImGuiWindowFlags_NoBackground) > 0) imgui_no_background = true;
			if ((flags & ImGuiWindowFlags_NoBringToFrontOnFocus) > 0) imgui_no_bring_to_front = true;
		}

		virtual void OnUpdate(Targs... args) {
		}

		virtual void OnRender() {
		}

		void Update(Targs... args) {
			OnUpdate(args...);
		}

		void Render() {

			ImGuiWindowFlags window_flags = 0;
			if (imgui_no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
			if (imgui_no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
			if (imgui_no_move)			  window_flags |= ImGuiWindowFlags_NoMove;
			if (imgui_no_resize)		  window_flags |= ImGuiWindowFlags_NoResize;
			if (!imgui_no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
			if (imgui_no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
			if (imgui_no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
			if (imgui_no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
			if (imgui_no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
			window_flags |= ImGuiWindowFlags_NoSavedSettings;

			if (imgui_no_move || firstUpdate)
				ImGui::SetNextWindowPos(ImVec2(windowPosition.x, windowPosition.y));

			if (imgui_no_resize || firstUpdate)
				ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));

			ImGui::GetStyle().WindowRounding = 0.0f;

			bool* p_open = nullptr;	// Hide close button
			ImGui::Begin(name.c_str(), p_open, window_flags);

			// Check if mouse in on window
			isMouseOnWindow = ImGui::GetMousePos().x >= ImGui::GetWindowPos().x &&
				ImGui::GetMousePos().x <= ImGui::GetWindowPos().x + ImGui::GetWindowSize().x &&
				ImGui::GetMousePos().y >= ImGui::GetWindowPos().y &&
				ImGui::GetMousePos().y <= ImGui::GetWindowPos().y + ImGui::GetWindowSize().y;

			// Virtual function, overridden by a derived class
			OnRender();

			ImGui::End();

			firstUpdate = false;
		}

	protected:

		bool imgui_no_titlebar = false;
		bool imgui_no_scrollbar = false;
		bool imgui_no_move = false;
		bool imgui_no_resize = false;
		bool imgui_no_menu = true;
		bool imgui_no_collapse = false;
		bool imgui_no_nav = false;
		bool imgui_no_background = false;
		bool imgui_no_bring_to_front = false;

	private:
		bool firstUpdate;
	};

}
