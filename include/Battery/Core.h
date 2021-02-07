#pragma once

#include "Battery/pch.h"
#include "Battery/AllegroDeps.h"

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










		

	}
}
