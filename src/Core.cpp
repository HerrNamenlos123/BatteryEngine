
#include "Battery/Core.h"
#include "Battery/Platform.h"
#include "Battery/Exception.h"
#include "Battery/_AllegroDependencies.h"

#include <iostream>

namespace Battery {
	namespace Core {

		bool Initialize() {

			if (Core::GetApplicationName() == "" && Core::GetOrganizationName() == "") {
				throw Battery::Exception("Neither the Application nor the Organization name was set, should be set before initialization");
			}
			
			if (!al_init())
				return false;

			if (!Platform::InitializeNativeDialogAddon())
				return false;

			return true;
		}

		bool IsInitialized() {
			return al_is_system_installed();
		}

		void Shutdown() {
			Platform::ShutdownNativeDialogAddon();
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
