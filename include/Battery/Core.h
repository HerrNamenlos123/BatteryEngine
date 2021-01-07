#pragma once

#include <string>

namespace Battery {
	namespace Core {

		/// <summary>
		/// Initialize the Allegro context
		/// </summary>
		/// <exception cref="Battery::Exception - Thrown when neither the application name nor the organization name
		/// were set before initialization"></exception>
		/// <returns>bool - if initialization was successful</returns>
		bool Initialize();

		/// <summary>
		/// Check if the Allegro context is already initialized
		/// </summary>
		/// <returns>bool - if context is initialized</returns>
		bool IsInitialized();

		/// <summary>
		/// Shut the engine down with all dependencies, should be the last cleanup step after the program terminates
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

	}
}
