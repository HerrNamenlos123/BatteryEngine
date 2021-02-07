
#include "Battery/pch.h"
#include "Battery/Core.h"
#include "Battery/Core/Exception.h"
#include "Battery/FileUtils.h"
#include "Battery/AllegroDeps.h"

namespace Battery {
	namespace Core {

		void Initialize() {

			// Return if it is already initialized
			if (IsInitialized())
				return;

			// Only continue if an application name was set for the %appdata% path
			if (Core::GetApplicationName() == "" && Core::GetOrganizationName() == "")
				throw Battery::Exception("Neither the Application nor the Organization name was set, should be set before initialization");

			// Initialize the Allegro Framework and all components
			if (!al_init())
				throw Battery::Exception("Allegro failed to initialize");

			if (!al_init_font_addon())
				throw Battery::Exception("Failed to initialize the Allegro font addon");

			if (!al_init_ttf_addon())
				throw Battery::Exception("Failed to initialize the Allegro TrueType font addon");

			if (!al_init_primitives_addon())
				throw Battery::Exception("Failed to initialize the Allegro Primitives addon");

			if (!al_init_image_addon())
				throw Battery::Exception("Failed to initialize the Allegro Image addon");

			if (!al_install_keyboard())
				throw Battery::Exception("Failed to install the Allegro keyboard addon");

			if (!al_install_mouse())
				throw Battery::Exception("Failed to initialize the Allegro mouse addon");

		}

		bool IsInitialized() {
			return al_is_system_installed();
		}

		void Shutdown() {
			// Addons, keyboard and mouse are all shut down automatically
			al_uninstall_system();
		}


















		std::string GetApplicationName() {
			return al_get_app_name();
		}

		std::string GetOrganizationName() {
			return al_get_org_name();
		}

		void SetApplicationName(const std::string& name) {
			al_set_app_name(name.c_str());
		}

		void SetOrganizationName(const std::string& name) {
			al_set_org_name(name.c_str());
		}

		std::string GetAllegroVersion() {
			uint32_t version = al_get_allegro_version();
			int major = version >> 24;
			int minor = (version >> 16) & 255;
			int revision = (version >> 8) & 255;
			int release = version & 255;

			return std::to_string(major) + "." + std::to_string(minor) + "." + 
				   std::to_string(revision) + "." + std::to_string(release);
		}








		

	}
}
