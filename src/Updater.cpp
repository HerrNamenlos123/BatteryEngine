
#include "Battery/Updater.h"
#include "Battery/Core.h"
#include "Battery/Exception.h"
#include "Battery/FileUtils.h"
#include "Battery/StringUtils.h"
#include "Battery/Config.h"

#include <iostream>

namespace Battery {
	namespace Updater {

		std::string GetApplicationVersion() {

			if (Core::GetApplicationName() != "" || Core::GetOrganizationName() != "") {	// A valid name is given

				std::string file = FileUtils::GetAllegroStandardPath(ALLEGRO_USER_DATA_PATH) + BATTERY_VERSION_FILE;

				if (FileUtils::FileExists(file)) {	// File exists
					FileUtils::File f = FileUtils::LoadFile(file);

					if (!f.fail()) {	// File was loaded successfully
						auto fileLines = StringUtils::SplitString(f.str(), '\n');

						if (fileLines.size() > 0) {	// If fileLines contains anything, 
							return fileLines[0];	// index 0 must be the first line of the file
						}
					}
				}
			}
			else {
				throw Battery::Exception("Neither the Application nor the Organization name was set");
			}

			// File did not exist or could not be read, return nothing
			return "";
		}

		std::string GetApplicationReleaseDate() {

			if (Core::GetApplicationName() != "" || Core::GetOrganizationName() != "") {	// A valid name is given

				std::string file = FileUtils::GetAllegroStandardPath(ALLEGRO_USER_DATA_PATH) + BATTERY_VERSION_FILE;

				if (FileUtils::FileExists(file)) {	// File exists
					FileUtils::File f = FileUtils::LoadFile(file);

					if (!f.fail()) {	// File was loaded successfully
						auto fileLines = StringUtils::SplitString(f.str(), '\n');

						if (fileLines.size() >= 2) {	// If fileLines contains at least two lines
							return fileLines[1];		// index 1 must be the second line of the file
						}
					}
				}
			}
			else {
				throw Battery::Exception("Neither the Application nor the Organization name was set");
			}

			// File did not exist or could not be read, return nothing
			return "";
		}

		bool SetApplicationVersion(const std::string& version) {

			if (Core::GetApplicationName() != "" || Core::GetOrganizationName() != "") {	// A valid name is given

				std::string file = FileUtils::GetAllegroStandardPath(ALLEGRO_USER_DATA_PATH) + BATTERY_VERSION_FILE;
				std::cout << file << std::endl;

				if (FileUtils::FileExists(file)) {	// File exists
					FileUtils::File f = FileUtils::LoadFile(file);

					if (!f.fail()) {	// File was loaded successfully
						auto fileLines = StringUtils::SplitString(f.str(), '\n');

						if (fileLines.size() > 0) {	// If fileLines contains anything
							fileLines[0] = version;		// Replace the first line
						}
						else {	// If the file was empty, just create a new line
							fileLines.push_back(version);
						}

						// Now save the file
						std::string fileContent = StringUtils::JoinStrings(fileLines, "\n");
						return FileUtils::SaveFile(file, fileContent);
					}
					// The file failed to load, return below
				}
				else {	// File did not exist, create a new one
					std::vector<std::string> fileLines;

					// Simply add a new line to the empty file
					fileLines.push_back(version);

					// Now save the file
					std::string fileContent = StringUtils::JoinStrings(fileLines, "\n");
					std::cout << "saving file" << std::endl;
					return FileUtils::SaveFile(file, fileContent);
				}
			}
			else {
				throw Battery::Exception("Neither the Application nor the Organization name was set");
			}

			// File could not be read
			return false;
		}

		bool SetApplicationReleaseDate(const std::string& timestamp) {

			if (Core::GetApplicationName() != "" || Core::GetOrganizationName() != "") {	// A valid name is given

				std::string file = FileUtils::GetAllegroStandardPath(ALLEGRO_USER_DATA_PATH) + BATTERY_VERSION_FILE;

				if (FileUtils::FileExists(file)) {	// File exists
					FileUtils::File f = FileUtils::LoadFile(file);

					if (!f.fail()) {	// File was loaded successfully
						auto fileLines = StringUtils::SplitString(f.str(), '\n');

						if (fileLines.size() >= 2) {	// If fileLines contains at least two lines
							fileLines[1] = timestamp;		// Replace the second line
						}
						else {	// If the file was empty, just create two lines
							fileLines.push_back("");		// Add an empty line, timestamp must be on the second line
							fileLines.push_back(timestamp);
						}

						// Now save the file
						std::string fileContent = StringUtils::JoinStrings(fileLines, "\n");
						return FileUtils::SaveFile(file, fileContent);
					}
					// The file failed to load, return below
				}
				else {	// File did not exist, create a new one
					std::vector<std::string> fileLines;

					// Simply add the two lines to the empty file
					fileLines.push_back("");			// Add an empty line, timestamp must be on the second line
					fileLines.push_back(timestamp);

					// Now save the file
					std::string fileContent = StringUtils::JoinStrings(fileLines, "\n");
					return FileUtils::SaveFile(file, fileContent);
				}
			}
			else {
				throw Battery::Exception("Neither the Application nor the Organization name was set");
			}

			// File could not be read
			return false;
		}

	}
}
