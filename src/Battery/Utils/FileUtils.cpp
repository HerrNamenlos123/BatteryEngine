
#include "Battery/pch.h"

#include "Battery/Core.h"
#include "Battery/Core/Exception.h"
#include "Battery/Core/AllegroContext.h"
#include "Battery/Core/Config.h"
#include "Battery/Utils/FileUtils.h"
#include "Battery/AllegroDeps.h"

// We want to use std::min() and std::max()
#undef min
#undef max

#define CHECK_ALLEGRO_INIT() \
	if (!Battery::AllegroContext::GetInstance()->IsInitialized()) {	\
		throw Battery::Exception(__FUNCTION__ "(): Allegro Context is not initialized!");	\
	}

namespace Battery {
	namespace FileUtils {

		bool FilenameExists(const std::string& path) {
			return al_filename_exists(path.c_str());
		}

		bool FileExists(const std::string& path) {

			if (!FilenameExists(path))
				return false;

			ALLEGRO_FILE* file = al_fopen(path.c_str(), "r");

			if (file == nullptr)
				return false;

			al_fclose(file);
			return true;
		}

		bool DirectoryExists(const std::string& path) {

			if (!FilenameExists(path))
				return false;

			ALLEGRO_FILE* file = al_fopen(path.c_str(), "r");

			if (file == nullptr)
				return true;

			al_fclose(file);
			return false;
		}

		std::vector<std::string> GetDirectoryContent(const std::string& path) {
			std::vector<std::string> elements;

			if (!DirectoryExists(path))
				return elements;

			ALLEGRO_FS_ENTRY* dir = al_create_fs_entry(path.c_str());

			if (dir == nullptr)
				return elements;

			ALLEGRO_FS_ENTRY* e;
			al_open_directory(dir);
			do {
				e = al_read_directory(dir);

				if (e != nullptr) {
					elements.push_back(GetFilenameFromPath(al_get_fs_entry_name(e)));
				}

				al_destroy_fs_entry(e);

			} while (e != nullptr);
			al_close_directory(dir);

			al_destroy_fs_entry(dir);

			return elements;
		}

		bool MakeDirectory(const std::string& path) {
			return al_make_directory(path.c_str());
		}

		File ReadFile(const std::string& path) {

			ALLEGRO_FILE* file = al_fopen(path.c_str(), "r");

			if (file == nullptr)
				return File("", "", false);		// Return invalid file class

			char temp[BATTERY_FILE_BLOCK_SIZE];
			std::string str;

			do {
				size_t read = al_fread(file, temp, BATTERY_FILE_BLOCK_SIZE);
				str += std::string(temp, read);

				if (al_ferror(file)) {
					al_fclose(file);
					return File("", "", false);		// Error occurred, return invalid file
				}

			} while (!al_feof(file));

			al_fclose(file);
			return File(path, str, true);
		}

		bool WriteFile(const std::string& path, const std::string& content) {

			PrepareDirectory(GetDirectoryFromPath(path));

			ALLEGRO_FILE* file = al_fopen(path.c_str(), "w");

			if (file == nullptr)
				return false;

			char temp[BATTERY_FILE_BLOCK_SIZE];
			size_t index = 0;
			size_t written = 0;
			size_t totalWritten = 0;

			do {

				size_t contentRemaining = std::max(content.length() - index, (size_t)0);
				size_t amount = std::min((size_t)BATTERY_FILE_BLOCK_SIZE, contentRemaining);
				memcpy(temp, content.c_str() + index, amount);
				index += amount;

				written = al_fwrite(file, temp, amount);
				totalWritten += written;

				if (al_ferror(file)) {
					al_fclose(file);
					RemoveFile(path);
					return false;
				}

			} while (written == BATTERY_FILE_BLOCK_SIZE);

			if (totalWritten != content.length()) {
				al_fclose(file);
				RemoveFile(path);
				return false;
			}

			al_fclose(file);
			return true;
		}

		bool RemoveFile(const std::string& path) {
			return al_remove_filename(path.c_str());
		}

		bool RemoveDirectory(const std::string& path) {

			if (!DirectoryExists(path)) {							// If directory can't be accessed, check if the parent can
				return FilenameExists(GetDirectoryFromPath(path));	// be accessed to see if it's a permission problem
			}

			std::vector<std::string> content = GetDirectoryContent(path);

			for (std::string e : content) {
				if (DirectoryExists(path + "/" + e)) {
					if (!RemoveDirectory(path + "/" + e))
						return false;
				}
				else {
					if (!RemoveFile(path + "/" + e))
						return false;
				}
			}

			return RemoveFile(path);
		}

		bool RemoveDirectoryContent(const std::string& path) {

			if (!DirectoryExists(path)) {
				return false;
			}

			std::vector<std::string> content = GetDirectoryContent(path);

			for (std::string e : content) {
				if (DirectoryExists(path + "/" + e)) {
					if (!RemoveDirectory(path + "/" + e))
						return false;
				}
				else {
					if (!RemoveFile(path + "/" + e))
						return false;
				}
			}

			return true;
		}

		bool PrepareEmptyDirectory(const std::string& path) {

			// First prepare a directory so it exists
			if (!PrepareDirectory(path))
				return false;

			// Then delete all content so it's nice and empty
			return RemoveDirectoryContent(path);
		}

		bool PrepareDirectory(const std::string& path) {

			if (DirectoryExists(path))
				return true;

			return MakeDirectory(path);
		}










		
		std::string PromptFileSaveDialog(const std::vector<std::string>& acceptedFilesArray, 
			std::optional<std::reference_wrapper<AllegroWindow>> parentWindow, const std::string& defaultLocation) {

			if (!AllegroContext::GetInstance()->IsInitialized())
				throw Battery::Exception("The Engine is not initialized");

			// Load default location
			const char* defaultLoc = nullptr;
			if (defaultLocation != "") {
				defaultLoc = defaultLocation.c_str();
			}

			// Load accepted files
			std::string acceptedFiles = StringUtils::JoinStrings(acceptedFilesArray, ";");

			// Instantiate popup options
			ALLEGRO_FILECHOOSER* dialog = al_create_native_file_dialog(
				defaultLoc,
				"Save as",
				acceptedFiles.c_str(),
				ALLEGRO_FILECHOOSER_SAVE
			);

			// Load the display pointer
			ALLEGRO_DISPLAY* displayPointer = nullptr;
			if (parentWindow.has_value()) {
				displayPointer = parentWindow.value().get().allegroDisplayPointer;
			}

			// Open loaded window
			if (!al_show_native_file_dialog(displayPointer, dialog)) {
				al_destroy_native_file_dialog(dialog);
				return "";       // File was cancelled
			}

			// A File was chosen, return it
			std::string path = al_get_native_file_dialog_path(dialog, 0);
			al_destroy_native_file_dialog(dialog);
			return path;
		}
		
		std::string PromptFileOpenDialog(const std::vector<std::string>& acceptedFilesArray,
			std::optional<std::reference_wrapper<AllegroWindow>> parentWindow, const std::string& defaultLocation) {

			if (!AllegroContext::GetInstance()->IsInitialized())
				throw Battery::Exception("The Engine is not initialized");

			// Load default location
			const char* defaultLoc = nullptr;
			if (defaultLocation != "") {
				defaultLoc = defaultLocation.c_str();
			}

			// Load accepted files
			std::string acceptedFiles = StringUtils::JoinStrings(acceptedFilesArray, ";");

			// Instantiate popup options
			ALLEGRO_FILECHOOSER* dialog = al_create_native_file_dialog(
				defaultLoc,
				"Open",
				acceptedFiles.c_str(),
				0
			);

			// Load the display pointer
			ALLEGRO_DISPLAY* displayPointer = nullptr;
			if (parentWindow.has_value()) {
				displayPointer = parentWindow.value().get().allegroDisplayPointer;
			}

			// Open loaded window
			if (!al_show_native_file_dialog(displayPointer, dialog)) {
				al_destroy_native_file_dialog(dialog);
				return "";       // File was cancelled
			}

			// A File was chosen, return it
			std::string path = al_get_native_file_dialog_path(dialog, 0);
			al_destroy_native_file_dialog(dialog);
			return path;
		}

		std::vector<std::string> PromptFileOpenDialogMultiple(const std::vector<std::string>& acceptedFilesArray,
			std::optional<std::reference_wrapper<AllegroWindow>> parentWindow, const std::string& defaultLocation) {

			if (!AllegroContext::GetInstance()->IsInitialized())
				throw Battery::Exception("The Engine is not initialized");

			// Load default location
			const char* defaultLoc = nullptr;
			if (defaultLocation != "") {
				defaultLoc = defaultLocation.c_str();
			}

			// Load accepted files
			std::string acceptedFiles = StringUtils::JoinStrings(acceptedFilesArray, ";");

			// Instantiate popup options
			ALLEGRO_FILECHOOSER* dialog = al_create_native_file_dialog(
				defaultLoc,
				"Open",
				acceptedFiles.c_str(),
				ALLEGRO_FILECHOOSER_MULTIPLE
			);

			// Load the display pointer
			ALLEGRO_DISPLAY* displayPointer = nullptr;
			if (parentWindow.has_value()) {
				displayPointer = parentWindow.value().get().allegroDisplayPointer;
			}

			// Open loaded window
			if (!al_show_native_file_dialog(displayPointer, dialog)) {
				al_destroy_native_file_dialog(dialog);
				return std::vector<std::string>();       // File was cancelled
			}

			std::vector<std::string> paths;
			for (int i = 0; i < al_get_native_file_dialog_count(dialog); i++)
				paths.push_back(al_get_native_file_dialog_path(dialog, i));

			// Free memory
			al_destroy_native_file_dialog(dialog);
			return paths;
		}

		std::string PromptFileOpenDialogFolder(std::optional<std::reference_wrapper<AllegroWindow>> parentWindow, 
			const std::string& defaultLocation) {

			if (!AllegroContext::GetInstance()->IsInitialized())
				throw Battery::Exception("The Engine is not initialized");

			// Load default location
			const char* defaultLoc = nullptr;
			if (defaultLocation != "") {
				defaultLoc = defaultLocation.c_str();
			}

			// Load accepted files
			const char* acceptedFiles = "*.*";

			// Instantiate popup options
			ALLEGRO_FILECHOOSER* dialog = al_create_native_file_dialog(
				defaultLoc,
				"Open Folder",
				acceptedFiles,
				ALLEGRO_FILECHOOSER_FOLDER
			);

			// Load the display pointer
			ALLEGRO_DISPLAY* displayPointer = nullptr;
			if (parentWindow.has_value()) {
				displayPointer = parentWindow.value().get().allegroDisplayPointer;
			}

			// Open loaded window
			if (!al_show_native_file_dialog(displayPointer, dialog)) {
				al_destroy_native_file_dialog(dialog);
				return "";       // Folder was cancelled
			}

			// A Folder was chosen, return it
			std::string path = al_get_native_file_dialog_path(dialog, 0);
			al_destroy_native_file_dialog(dialog);
			return path;
		}
		
		std::string SaveFileWithDialog(const char* extension, const std::string& fileContent,
			std::optional<std::reference_wrapper<AllegroWindow>> parentWindow, const std::string& defaultLocation, 
			bool forceSave) {

			if (!AllegroContext::GetInstance()->IsInitialized())
				throw Battery::Exception("The Engine is not initialized");

			while (true) {
				std::string filename = PromptFileSaveDialog({ "*.*", ("*." + std::string(extension) + ";").c_str() }, 
					parentWindow, defaultLocation);

				if (filename != "") {	// Filename is valid

					// First append the extension if it's missing
					if (GetExtensionFromPath(filename) != std::string(".") + extension) {
						filename += std::string(".") + extension;
					}

					// Setup the allegro display pointer
					ALLEGRO_DISPLAY* displayPointer = nullptr;
					if (parentWindow.has_value()) {
						displayPointer = parentWindow.value().get().allegroDisplayPointer;
					}

					// Now check if the file already exists
					if (FileExists(filename)) {		// File already exists
						if (!ShowWarningMessageBoxYesNo(filename + " already exists. Do you really want to overwrite it?", displayPointer)) {
							// Don't overwrite the file, repeat
							continue;
						}
						// File gets overwritten
					}
					else if (FilenameExists(filename)) {
						ShowWarningMessageBox(filename + " is a directory! Please choose a file!", displayPointer);
						continue;	// Start over
					}

					// Now finally write the file
					if (WriteFile(filename, fileContent)) {	// File was successfully saved, return the file path
						return filename;
					}

					// File could not be saved, repeat
					ShowWarningMessageBox(filename + " could not be saved. Please try again!", displayPointer);
					continue;
				}
				// Else, the filename was invalid: Repeat if forcing is enabled

				if (!forceSave)
					return "";		// Return if file was cancelled

			};
		}
		
		File LoadFileWithDialog(const char* extension, std::optional<std::reference_wrapper<AllegroWindow>> parentWindow,
			const std::string& defaultLocation) {

			if (!AllegroContext::GetInstance()->IsInitialized())
				throw Battery::Exception("The Engine is not initialized");

			std::string filename = PromptFileOpenDialog({ "*.*", ("*." + std::string(extension) + ";").c_str() },
				parentWindow, defaultLocation);

			if (filename == "")		// No file was chosen, return
				return Battery::FileUtils::File("", "", false);

			// Setup the allegro display pointer
			ALLEGRO_DISPLAY* displayPointer = nullptr;
			if (parentWindow.has_value()) {
				displayPointer = parentWindow.value().get().allegroDisplayPointer;
			}

			// Check if extension is correct
			if (GetExtensionFromPath(filename) != std::string(".") + extension) {	// Wrong Extension
				ShowWarningMessageBox(filename + " has an unsupported file format. Please choose another file!", displayPointer);
				return LoadFileWithDialog(extension, parentWindow, defaultLocation);	// Try again by recursion
			}

			Battery::FileUtils::File file = ReadFile(filename);

			if (file.fail()) {
				ShowErrorMessageBox(filename + " could not be loaded!", displayPointer);
				return Battery::FileUtils::File("", "", false);
			}

			return file;
		}












		
		std::string GetExecutablePath() {
			CHECK_ALLEGRO_INIT();
			return GetAllegroStandardPath(ALLEGRO_EXENAME_PATH);
		}

		std::string GetExecutableName() {
			CHECK_ALLEGRO_INIT();
			return GetFilenameFromPath(GetExecutablePath());
		}

		std::string GetExecutableDirectory() {
			CHECK_ALLEGRO_INIT();
			return GetDirectoryFromPath(GetExecutablePath());
		}








		std::string GetFilenameFromPath(const std::string& path) {
			ALLEGRO_PATH* p = al_create_path(path.c_str());

			std::string filename = al_get_path_filename(p);

			al_destroy_path(p);
			return filename;
		}

		std::string GetBasenameFromPath(const std::string& path) {
			ALLEGRO_PATH* p = al_create_path(path.c_str());

			std::string filename = al_get_path_basename(p);

			al_destroy_path(p);
			return filename;
		}

		std::string GetExtensionFromPath(const std::string& path) {
			ALLEGRO_PATH* p = al_create_path(path.c_str());

			std::string filename = al_get_path_extension(p);

			al_destroy_path(p);
			return filename;
		}

		std::vector<std::string> GetComponentsFromPath(const std::string& path) {
			ALLEGRO_PATH* p = al_create_path(path.c_str());

			std::vector<std::string> v;

			if (path.length() == 0)
				return v;

			if (std::string(al_get_path_drive(p)) != "") {	// There is a drive letter, absolute path
				v.push_back(std::string(al_get_path_drive(p)));

				for (int i = 1; i < al_get_path_num_components(p); i++)
					v.push_back(std::string(al_get_path_component(p, i)));
			}
			else {	// There is no drive letter, relative path
				for (int i = 0; i < al_get_path_num_components(p); i++)
					v.push_back(std::string(al_get_path_component(p, i)));
			}

			if (std::string(al_get_path_filename(p)).length() > 0)
				v.push_back(std::string(al_get_path_filename(p)));

			al_destroy_path(p);
			return v;
		}

		std::string GetDirectoryFromPath(const std::string& path) {

			std::vector<std::string> arr = GetComponentsFromPath(path);
			std::string p;

			for (size_t i = 0; i < arr.size() - 1; i++)
				p += arr[i] + "/";

			return p;
		}
		
		std::string GetAllegroStandardPath(int id) {
			CHECK_ALLEGRO_INIT();

			std::string str;

			switch (id) {

			case ALLEGRO_RESOURCES_PATH:
			case ALLEGRO_TEMP_PATH:
			case ALLEGRO_USER_HOME_PATH:
			case ALLEGRO_USER_DOCUMENTS_PATH:
			case ALLEGRO_USER_DATA_PATH:
			case ALLEGRO_USER_SETTINGS_PATH:
			case ALLEGRO_EXENAME_PATH:

				{
					ALLEGRO_PATH* path = al_get_standard_path(id);
					str = al_path_cstr(path, '/');
					al_destroy_path(path);
				}
				break;
			default:
				throw Battery::Exception("Invalid enum supplied to " __FUNCTION__ "()");
			}

			return str;
		}
	}
}
