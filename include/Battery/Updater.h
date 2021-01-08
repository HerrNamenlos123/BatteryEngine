#pragma once

#include <string>
#include <iomanip>

namespace Battery {
	namespace Updater {

		/// <summary>
		/// Get the version string, which was saved in the "version" file in the application settings folder.
		/// The returned string is the content of the file, which will be what it was previously set to
		/// or the version string from the Github release. When the file can't be found or is empty, nothing ("")
		/// is returned
		/// </summary>
		/// <exception cref="Battery::Exception - Thrown when the Engine was not initialized or when the application
		/// or organization name was not set"></exception>
		/// <returns>std::string - The first line of the version file or ""</returns>
		std::string GetApplicationVersion();

		/// <summary>
		/// Set the application version in the version file in the application settings folder (%appdata%).
		/// The string will replace the file content or create it if it does not exist.
		/// This version string is used to check for new available versions and generally contains the version
		/// string from the installed Github release.
		/// </summary>
		/// <param name="version">- The version string</param>
		/// <exception cref="Battery::Exception - Thrown when the Engine was not initialized or when the application
		/// or organization name was not set"></exception>
		/// <returns>bool - If the file was successfully written or created</returns>
		bool SetApplicationVersion(const std::string& version);

		/// <summary>
		/// Check if a new Github release is available and the version tag is different from the currently installed one.
		/// </summary>
		/// <param name="githubUser">- The username of the Github repository</param>
		/// <param name="repository">- The Github repository name</param>
		/// <returns>bool - if a new update is available</returns>
		bool CheckForGithubUpdate(const std::string& githubUser, const std::string& repository);

	}
}
