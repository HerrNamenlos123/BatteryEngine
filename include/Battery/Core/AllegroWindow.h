#pragma once

#include "Battery/AllegroDeps.h"
#include "Battery/Log/Log.h"
#include "Battery/Core/Exception.h"
#include "Battery/Core/ApplicationEvents.h"
#include "Battery/Renderer/Texture2D.h"

/// <summary>
/// Usage: allegroWindow.SetWindowExecutableIcon();
/// Set the window icon to the embedded icon of the executable. If 'IDI_ICON1' is undefined, make sure to include the (correct!!!)
/// 'resource.h', which is located in the inner project folder of your Visual Studio project. Also make sure that a .ico file is added
/// as a resource in Visual Studio.
/// </summary>
#define SetWindowExecutableIcon() __setWindowsIconID(IDI_ICON1)

#undef RegisterClipboardFormat

namespace Battery {

	typedef clip::format ClipboardFormatID;

	enum class ClipboardFormat {
		NONE  = 0,
		TEXT  = 1,
		IMAGE = 2
	};

	class AllegroWindow {
	public:
		AllegroWindow(int w, int h);
		~AllegroWindow();

		void Create(int windowFlags = 0);
		void Destroy();
		void SetEventCallback(std::function<void(Battery::Event* event)> eventCallback);
		void HandleEvents();
		void HandleEvent(Battery::Event* event);

		glm::ivec2 GetScreenPosition();
		void SetScreenPosition(const glm::ivec2& position);
		int GetWidth();
		int GetHeight();
		glm::ivec2 GetSize();
		void SetSize(const glm::vec2 size);
		void SetTitle(const std::string title);
		void Maximize();
		void Minimize();
		void Restore();
		glm::vec2 GetMousePosition();
		bool GetLeftMouseButton();
		bool GetRightMouseButton();
		bool GetMouseWheel();
		HWND GetWinHandle();
		bool IsFocused();
		bool Focus();
		bool Hide();
		bool Show();
		void HideFromTaskbar();
		void ShowInTaskbar();
		void SetFrameless(bool frameless);
		void FlipDisplay();

		ClipboardFormatID RegisterClipboardFormat(const std::string& format);
		bool HasClipboardText();
		bool HasClipboardImage();
		bool HasClipboardFormat(ClipboardFormatID format);
		std::pair<std::string, bool> GetClipboardText();
		std::optional<Battery::Texture2D> GetClipboardImage();
		std::optional<std::vector<uint8_t>> GetClipboardCustomFormat(ClipboardFormatID customFormat);
		std::optional<std::string> GetClipboardCustomFormatString(ClipboardFormatID customFormat);
		bool SetClipboardText(const std::string& text);
		bool SetClipboardImage(const Battery::Texture2D& image);
		bool SetClipboardCustomFormat(ClipboardFormatID customFormat, void* data, size_t size);
		bool SetClipboardCustomFormatString(ClipboardFormatID customFormat, const std::string& string);

		/// <summary>
		/// This function sets the cursor icon. It must be called for every mouse event.
		/// </summary>
		bool SetMouseCursor(ALLEGRO_SYSTEM_MOUSE_CURSOR cursorID);

		/// <summary>
		/// It is encouraged to use the macro SetWindowExecutableIcon() instead of this function.
		/// The supplied id should be IDI_ICON1, but must be evaluated at the compile stage
		/// in your final project. IDI_ICON1 is defined in "resource.h" in the project directory
		/// of your final project.
		/// </summary>
		/// <param name="iconID">- the id of the embedded icon in the executable.</param>
		/// <returns>bool - if loading the icon was successful</returns>
		bool __setWindowsIconID(int iconID);

		ALLEGRO_DISPLAY* allegroDisplayPointer = nullptr;
		ALLEGRO_EVENT_QUEUE* allegroEventQueue = nullptr;
		ALLEGRO_FONT* allegroDefaultFont = nullptr;

	private:
		std::string defaultFontFile = "C:\\Windows\\Fonts\\consola.ttf";

		// These are only for remembering the size until the window is created
		int width = 0;
		int height = 0;
		bool valid = false;

		ParentEventContainer eventContainer;
		std::function<void(Battery::Event* event)> eventCallback = nullptr;
	};

}
