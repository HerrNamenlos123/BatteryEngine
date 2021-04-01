#pragma once

#include "Battery/pch.h"
#include "Battery/Core/Layer.h"
#include "Battery/Core/Config.h"
#include "Battery/Core/Application.h"

namespace Battery {

	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer() {}
		ImGuiLayer(const std::string& debugName) : Layer(debugName) {}

		void OnAttach() final {		// Prevent from being overridden again

			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImPlot::CreateContext();
			io = ImGui::GetIO();
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsClassic();

			// Setup Platform/Renderer backends
			ImGui_ImplAllegro5_Init(applicationPointer->window.allegroDisplayPointer);

			io = ImGui::GetIO();
			font = io.Fonts->AddFontFromFileTTF(BATTERY_PROFILING_FONT, BATTERY_PROFILING_FONT_SIZE);
			OnImGuiAttach();

			memset(profilerFilter, 0, BATTERY_PROFILING_MAX_TIMEPOINT_NUMBER - 1);
		}

		void OnDetach() final {
			io = ImGui::GetIO();
			OnImGuiDetach();
			ImGui_ImplAllegro5_Shutdown();

			ImPlot::DestroyContext();
			ImGui::DestroyContext();
		}
		
		void OnRender() final {

			// Start ImGui Frame
			ImGui_ImplAllegro5_NewFrame();
			ImGui::NewFrame();
			
			io = ImGui::GetIO();
			OnImGuiRender();
			
			// Render various windows
			if (enableProfiling) {
				RenderProfiler();
			}
			if (enableImGuiDemoWindow) {
				ImGui::ShowDemoWindow();
			}
			if (enableImPlotDemoWindow) {
				ImPlot::ShowDemoWindow();
			}
			
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
				al_acknowledge_resize(applicationPointer->window.allegroDisplayPointer);
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

		void RenderProfiler() {
			TimeUtils::ProfilerStorage& storage = TimeUtils::ProfilerStorage::GetInstance();
			
			TimeUtils::ScopedTimer timer(__FUNCTION__"()",
				&storage.temp_profilerResultsRenderTime);

#ifndef BATTERY_PROFILING
			LOG_CORE_WARN(__FUNCTION__"(): Can't render profiler results: Battery engine was compiled without profiler support!");
			storage.engineProfilingResults.Clear();
			storage.clientProfilingResults.Clear();
			return;
#endif
			
			int w = applicationPointer->window.GetWidth();
			int h = applicationPointer->window.GetHeight();

			ImGui::PushFont(font);
			float wd = 450;
			float ht = 200;
			ImGui::SetNextWindowPos({ w / 2.f - wd / 2.f, h / 2.f - ht / 2.f }, ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize({ wd, ht }, ImGuiCond_FirstUseEver);
			ImGui::Begin("Profiler");
			
			// Engine profiling
			size_t size = storage.engineProfilingResults.nextIndex;
			if (size > 0) {
				ImGui::Text("Engine scope profiling: ");
				for (size_t i = 0; i < size; i++) {
					char str[1024];
					snprintf(str, 1024, "%s% 8.03f ms -> %s", str >= 0 ? "+" : "-", 
						storage.engineProfilingResults.times[i] * 1000.0, storage.engineProfilingResults.names[i]);
					ImGui::Text(str);
				}
				storage.engineProfilingResults.Clear();
			}

			ImGui::Separator();

			// Client profiling
			size = storage.clientProfilingResults.nextIndex;
			if (size > 0) {
				ImGui::Text("Client scope profiling: ");
				for (size_t i = 0; i < size; i++) {
					char str[1024];
					snprintf(str, 1024, "%s% 8.03f ms -> %s", str >= 0 ? "+" : "-",
						storage.clientProfilingResults.times[i] * 1000.0, storage.clientProfilingResults.names[i]);
					ImGui::Text(str);
				}
				storage.clientProfilingResults.Clear();
			}

			ImGui::Separator();

			// Previous profile from this function itself
			size = storage.profilerResultsRenderTime.nextIndex;
			if (size >= 1) {
				ImGui::Text("Previous duration rendering this window: ");
				char str[1024];
				snprintf(str, 1024, "%s% 8.03f ms -> %s", str >= 0 ? "+" : "-",
					storage.profilerResultsRenderTime.times[0] * 1000.0, storage.profilerResultsRenderTime.names[0]);
				ImGui::Text(str);
				storage.profilerResultsRenderTime.Clear();
			}
			else {
				LOG_CORE_TRACE("Skipping rendering profiler timepoint: Profiler window timepoint buffer is empty!");
			}

			ImGui::Separator();




			// Now render the timestamp profiling
			size = storage.timestampProfilerComplete.nextIndex;
			char* labels[BATTERY_PROFILING_MAX_TIMEPOINT_NUMBER - 1];

			if (size >= 1) {
				ImGui::Text("Timestamp profiling:");
				for (size_t i = 0; i < size; i++) {
					double diff = 0;
					if (i > 0) {
						diff = storage.timestampProfilerComplete.timestamps[i] -
							   storage.timestampProfilerComplete.timestamps[i - 1];

						// Moving average filter
						profilerFilter[i - 1] = profilerFilter[i - 1] * 0.8f + diff * 1000.f * 0.2f;
						labels[i - 1] = storage.timestampProfilerComplete.names[i];
					}
			
					char str[1024];
					snprintf(str, 1024, "%s%09.04f ms -> %s", str >= 0 ? "+" : "-",
						diff * 1000.0, storage.timestampProfilerComplete.names[i]);
					ImGui::Text(str);
				}
			
				ImGui::Text("Unprofiled frame time:");
				char str[1024];
				snprintf(str, 1024, "%s%09.04f ms", str >= 0 ? "+" : "-",
					storage.frametimeMissed * 1000.0);
				ImGui::Text(str);
			}
			else {
				LOG_CORE_TRACE("Skipping rendering timestamp profiling: Buffer is empty!");
			}
			
			ImPlot::SetNextPlotLimits(0, 1, 0, 1, ImGuiCond_Always);
			if (ImPlot::BeginPlot("##Pie1", NULL, NULL, ImVec2(600, 600), ImPlotFlags_Equal | ImPlotFlags_NoMousePos, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations)) {
				ImPlot::PlotPieChart(labels, profilerFilter, (int)size - 1, 0.5, 0.5, 0.4, true, "%.2f");
				ImPlot::EndPlot();
			}

			ImGui::End();
			ImGui::PopFont();
		}

		ImGuiIO& io = dummyIO;
		bool enableProfiling = false;
		bool enableImGuiDemoWindow = false;
		bool enableImPlotDemoWindow = false;

	private:
		ImGuiIO dummyIO;
		ImFont* font = nullptr;
		float profilerFilter[BATTERY_PROFILING_MAX_TIMEPOINT_NUMBER - 1];
	};

}
