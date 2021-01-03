#pragma once

#include <string>
#include "Battery/_AllegroDependencies.h"

namespace Battery {
	namespace Platform {

		/// <summary>
		/// Initialize the Allegro native dialog addon, needs to be called before other functions are used
		/// </summary>
		/// <returns>bool - if initialization was successful</returns>
		bool InitializeNativeDialogAddon();

		/// <summary>
		/// Check if the Allegro native dialog addon was initialized
		/// </summary>
		/// <returns>bool - if it was initialized</returns>
		bool IsNativeDialogAddonInitialized();

		/// <summary>
		/// Shut the Allegro native file dialog addon down
		/// </summary>
		void ShutdownNativeDialogAddon();











		/// <summary>
		/// Show a windows error message, the display can be nullptr if the messagebox is not owned by a window
		/// </summary>
		/// <param name="msg">- The message in the box</param>
		/// <param name="display">- An ALLEGRO_DISPLAY pointer</param>
		void ShowErrorMessageBox(const std::string& msg, ALLEGRO_DISPLAY* display = nullptr);

		/// <summary>
		/// Show a windows error message, the display can be nullptr if the messagebox is not owned by a window.
		/// Yes and No buttons are displayed and when yes is pressed, true is returned, otherwise false
		/// </summary>
		/// <param name="msg">- The message in the box</param>
		/// <param name="display">- An ALLEGRO_DISPLAY pointer</param>
		/// <returns>bool - if yes was pressed</returns>
		bool ShowErrorMessageBoxYesNo(const std::string& msg, ALLEGRO_DISPLAY* display = nullptr);

		/// <summary>
		/// Show a windows error message, the display can be nullptr if the messagebox is not owned by a window.
		/// Ok and Cancel buttons are displayed and when ok is pressed, true is returned, otherwise false
		/// </summary>
		/// <param name="msg">- The message in the box</param>
		/// <param name="display">- An ALLEGRO_DISPLAY pointer</param>
		/// <returns>bool - if ok was pressed</returns>
		bool ShowErrorMessageBoxOkCancel(const std::string& msg, ALLEGRO_DISPLAY* display = nullptr);

		/// <summary>
		/// Show a windows warning message, the display can be nullptr if the messagebox is not owned by a window
		/// </summary>
		/// <param name="msg">- The message in the box</param>
		/// <param name="display">- An ALLEGRO_DISPLAY pointer</param>
		void ShowWarningMessageBox(const std::string& msg, ALLEGRO_DISPLAY* display = nullptr);

		/// <summary>
		/// Show a windows warning message, the display can be nullptr if the messagebox is not owned by a window.
		/// Yes and No buttons are displayed and when yes is pressed, true is returned, otherwise false
		/// </summary>
		/// <param name="msg">- The message in the box</param>
		/// <param name="display">- An ALLEGRO_DISPLAY pointer</param>
		/// <returns>bool - if yes was pressed</returns>
		bool ShowWarningMessageBoxYesNo(const std::string& msg, ALLEGRO_DISPLAY* display = nullptr);

		/// <summary>
		/// Show a windows warning message, the display can be nullptr if the messagebox is not owned by a window.
		/// Ok and Cancel buttons are displayed and when ok is pressed, true is returned, otherwise false
		/// </summary>
		/// <param name="msg">- The message in the box</param>
		/// <param name="display">- An ALLEGRO_DISPLAY pointer</param>
		/// <returns>bool - if ok was pressed</returns>
		bool ShowWarningMessageBoxOkCancel(const std::string& msg, ALLEGRO_DISPLAY* display = nullptr);

		/// <summary>
		/// Show a windows info message, the display can be nullptr if the messagebox is not owned by a window
		/// </summary>
		/// <param name="msg">- The message in the box</param>
		/// <param name="display">- An ALLEGRO_DISPLAY pointer</param>
		void ShowInfoMessageBox(const std::string& msg, ALLEGRO_DISPLAY* display = nullptr);

		/// <summary>
		/// Show a windows info message, the display can be nullptr if the messagebox is not owned by a window.
		/// Yes and No buttons are displayed and when yes is pressed, true is returned, otherwise false
		/// </summary>
		/// <param name="msg">- The message in the box</param>
		/// <param name="display">- An ALLEGRO_DISPLAY pointer</param>
		/// <returns>bool - if yes was pressed</returns>
		bool ShowInfoMessageBoxYesNo(const std::string& msg, ALLEGRO_DISPLAY* display = nullptr);

		/// <summary>
		/// Show a windows info message, the display can be nullptr if the messagebox is not owned by a window.
		/// Ok and Cancel buttons are displayed and when ok is pressed, true is returned, otherwise false
		/// </summary>
		/// <param name="msg">- The message in the box</param>
		/// <param name="display">- An ALLEGRO_DISPLAY pointer</param>
		/// <returns>bool - if ok was pressed</returns>
		bool ShowInfoMessageBoxOkCancel(const std::string& msg, ALLEGRO_DISPLAY* display = nullptr);




	}
}
