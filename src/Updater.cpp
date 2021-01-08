
#include "Battery/Updater.h"
#include "Battery/Core.h"
#include "Battery/Exception.h"
#include "Battery/FileUtils.h"
#include "Battery/StringUtils.h"
#include "Battery/Config.h"
#include "Battery/NetUtils.h"
#include "json.hpp"

#include <iostream>

namespace Battery {
	namespace Updater {

		std::string GetApplicationVersion() {

			if (Core::GetApplicationName() != "" || Core::GetOrganizationName() != "") {	// A valid name is given

				std::string file = FileUtils::GetAllegroStandardPath(ALLEGRO_USER_DATA_PATH) + BATTERY_VERSION_FILE;
				FileUtils::File f = FileUtils::LoadFile(file);

				if (!f.fail())
					return "";

				// File was loaded successfully
				return f.str();
			}

			throw Battery::Exception("Neither the Application nor the Organization name was set");
		}

		bool SetApplicationVersion(const std::string& version) {

			if (Core::GetApplicationName() != "" || Core::GetOrganizationName() != "") {	// A valid name is given

				std::string file = FileUtils::GetAllegroStandardPath(ALLEGRO_USER_DATA_PATH) + BATTERY_VERSION_FILE;
				std::cout << file << std::endl;

				// Simply save the file and return success
				return FileUtils::SaveFile(file, version);
			}

			throw Battery::Exception("Neither the Application nor the Organization name was set");
		}


		bool CheckForGithubUpdate(const std::string& githubUser, const std::string& repository) {

			std::string url = "https://api.github.com/repos/" + githubUser + "/" + repository + "/releases/latest";
			NetUtils::HTTPRequest request = NetUtils::GetHTTPRequest(url, NETWORK_CERTIFICATE_FILE, "gister");

			// If the Github API could not successfully be requested, no new update is available
			if (!request.valid)
				return false;

			std::string availableVersion;

			// Get the version of the latest release available
			try {
				nlohmann::json json = nlohmann::json::parse(request.response);
				
				availableVersion = json["tag_name"].get<std::string>();

				// When getting the version fails, just "" is returned, it's time to update to restore the version file
				std::string currentVersion = GetApplicationVersion();

				if (availableVersion.length() > 0) {	// Available release is valid

					if (currentVersion != availableVersion) {

						//std::cout << "Available: " << availableVersion << std::endl;
						//std::cout << "Installed: " << currentVersion << std::endl;

						// A new version is available and is different from the currently installed one
						return true;
					}
				}
			}
			catch (...) {		// No new update available, an error occurred while parsing the response
				return false;	// The response was probably wrong or corrupted
			}

			return false;
		}

	}
}
