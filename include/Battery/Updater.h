#pragma once

#include <string>
#include <iomanip>

namespace Battery {
	namespace Updater {

		/// <summary>
		/// Get the version string, which was saved in the "version" file in the application settings folder.
		/// The returned string is the first line of the file, which will be what it was previously set to
		/// or the version string from the Github release. When the file can't be found or is empty, nothing ("")
		/// is returned
		/// </summary>
		/// <exception cref="Battery::Exception - Thrown when the Engine was not initialized or when the application
		/// or organization name was not set"></exception>
		/// <returns>std::string - The first line of the version file or ""</returns>
		std::string GetApplicationVersion();

		/// <summary>
		/// Get the release date string, which was saved in the "version" file in the application settings folder.
		/// The returned string is the second line of the file, which will be what it was previously set to
		/// or the Unix timestamp string from the Github release in the field "published_at". 
		/// When the file can't be found, is empty or does not have a second line, nothing ("") is returned
		/// </summary>
		/// <exception cref="Battery::Exception - Thrown when the Engine was not initialized or when the application
		/// or organization name was not set"></exception>
		/// <returns>std::string - The second line of the version file or ""</returns>
		std::string GetApplicationReleaseDate();

		/// <summary>
		/// Set the application version in the version file in the application settings folder (%appdata%).
		/// The string will replace the first line of the file or create it if it does not exist.
		/// This version string is used to check for new available versions and generally contains the version
		/// string from the installed Github release.
		/// </summary>
		/// <param name="version">- The version string</param>
		/// <exception cref="Battery::Exception - Thrown when the Engine was not initialized or when the application
		/// or organization name was not set"></exception>
		/// <returns>bool - If the file was successfully written or created</returns>
		bool SetApplicationVersion(const std::string& version);

		/// <summary>
		/// Set the application release date in the version file in the application settings folder (%appdata%).
		/// The string will replace the second line of the file or create it if it does not exist.
		/// This release date string will usually be a Unix timestamp and is used to check for new available versions 
		/// and generally contains the release date from the installed Github release in the field "published_at".
		/// </summary>
		/// <param name="timestamp">- The release date as a Unix timestamp in string format</param>
		/// <exception cref="Battery::Exception - Thrown when the Engine was not initialized or when the application
		/// or organization name was not set"></exception>
		/// <returns>bool - If the file was successfully written or created</returns>
		bool SetApplicationReleaseDate(const std::string& timestamp);

	}
}
