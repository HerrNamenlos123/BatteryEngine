#pragma once

#include <string>
#include "Battery/_AllegroDependencies.h"

namespace Battery {
	namespace Core {

		/// <summary>
		/// Initialize the Allegro framework and all components
		/// </summary>
		/// <exception cref="Battery::Exception - Thrown when any component of the Allegro framework 
		/// failed to initialize"></exception>
		void Initialize();

		/// <summary>
		/// Check if the Allegro framework is already initialized
		/// </summary>
		/// <returns>bool - if framework is initialized</returns>
		bool IsInitialized();

		/// <summary>
		/// Shut the Allegro framework and all components down
		/// </summary>
		void Shutdown();






		/// <summary>
		/// Get the global application name, will be seen in the user path (%appdata% folder)
		/// </summary>
		std::string GetApplicationName();

		/// <summary>
		/// Get the global organization name, will be seen in the user path (%appdata% folder)
		/// </summary>
		std::string GetOrganizationName();

		/// <summary>
		/// Set the global application name, will be seen in the user path (%appdata% folder)
		/// </summary>
		/// <param name="name">- The string to set the name to</param>
		void SetApplicationName(const std::string& name);

		/// <summary>
		/// Set the global organization name, will be seen in the user path (%appdata% folder)
		/// </summary>
		/// <param name="name">- The string to set the name to</param>
		void SetOrganizationName(const std::string& name);

		/// <summary>
		/// Get the compiled version string of Allegro, for example: "5.2.6.1"
		/// </summary>
		/// <returns>std::string - The version string</returns>
		std::string GetAllegroVersion();










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
