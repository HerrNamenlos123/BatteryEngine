
#include "Battery/FileUtils.h"

#include "Battery/Core.h"
#include "Battery/Exception.h"
#include "Battery/_AllegroDependencies.h"

#include <iostream>

#define FILE_BLOCK_SIZE 1024

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

		File LoadFile(const std::string& path) {
			
			ALLEGRO_FILE* file = al_fopen(path.c_str(), "r");
			
			if (file == nullptr)
				return File("", "", false);		// Return invalid file class

			char temp[FILE_BLOCK_SIZE];
			std::string str;

			do {
				size_t read = al_fread(file, temp, FILE_BLOCK_SIZE);
				str += std::string(temp, read);

				if (al_ferror(file)) {
					al_fclose(file);
					return File("", "", false);		// Error occurred, return invalid file
				}

			} while (!al_feof(file));

			al_fclose(file);
			return File(path, str, true);
		}

		bool SaveFile(const std::string& path, const std::string& content) {

			PrepareDirectory(GetDirectoryFromPath(path));

			ALLEGRO_FILE* file = al_fopen(path.c_str(), "w");

			if (file == nullptr)
				return false;

			char temp[FILE_BLOCK_SIZE];
			size_t index = 0;
			size_t written = 0;
			size_t totalWritten = 0;

			do {

				size_t contentRemaining = max(content.length() - index, 0);
				size_t amount = min(FILE_BLOCK_SIZE, contentRemaining);
				memcpy(temp, content.c_str() + index, amount);
				index += amount;

				written = al_fwrite(file, temp, amount);
				totalWritten += written;

				if (al_ferror(file)) {
					al_fclose(file);
					RemoveFile(path);
					return false;
				}

			} while (written == FILE_BLOCK_SIZE);

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











		std::string PromptFileSaveDialog(const std::string& acceptedFiles, ALLEGRO_DISPLAY* parentWindow, const std::string& defaultLocation) {

			if (!Core::IsInitialized())
				throw Battery::Exception("The Engine is not initialized");

			// Load default location
			const char* defaultLoc = nullptr;
			if (defaultLocation != "") {
				defaultLoc = defaultLocation.c_str();
			}

			// Instantiate popup options
			ALLEGRO_FILECHOOSER* dialog = al_create_native_file_dialog(
				defaultLoc,
				"Save as",
				acceptedFiles.c_str(),
				ALLEGRO_FILECHOOSER_SAVE
			);

			// Open queued window
			if (!al_show_native_file_dialog(parentWindow, dialog)) {
				al_destroy_native_file_dialog(dialog);
				return "";       // File was cancelled
			}

			// A File was chosen
			std::string path = al_get_native_file_dialog_path(dialog, 0);

			// Free memory
			al_destroy_native_file_dialog(dialog);

			return path;
		}

		std::string SaveFileWithDialog(const std::string& extension, const std::string& fileContent, ALLEGRO_DISPLAY* parentWindow,
																					const std::string& defaultLocation, bool forceSave)
		{
			if (!Core::IsInitialized())
				throw Battery::Exception("The Engine is not initialized");

			while (true) {
				std::string file = PromptFileSaveDialog(std::string("*.*;*.") + extension + ";", parentWindow, defaultLocation);

				if (file != "") {	// Filename is valid

					// First append the extension if it's missing
					if (GetExtensionFromPath(file) != std::string(".") + extension) {
						file += "." + extension;
					}

					// Now check if the file already exists
					if (FileExists(file)) {		// File already exists
						if (!Core::ShowWarningMessageBoxYesNo(file + " already exists. Do you really want to overwrite it?", parentWindow)) {
							// Don't overwrite the file, repeat
							continue;
						}
					}
					else if (FilenameExists(file)) {
						Core::ShowWarningMessageBox(file + " is a directory! Please choose another file!", parentWindow);
						continue;
					}

					bool saved = SaveFile(file, fileContent);

					if (saved) {	// File was successfully saved, return the file path
						return file;
					}

					// File could not be saved, repeat
					Core::ShowWarningMessageBox(file + " could not be saved. Try again!", parentWindow);
					continue;
				}

				if (!forceSave)
					break;		// Break if file was cancelled

			};

			return "";
		}

		std::string PromptFileOpenDialog(const std::string& acceptedFiles, ALLEGRO_DISPLAY* parentWindow, const std::string& defaultLocation) {

			if (!Core::IsInitialized())
				throw Battery::Exception("The Engine is not initialized");

			// Load default location
			const char* defaultLoc = nullptr;
			if (defaultLocation != "") {
				defaultLoc = defaultLocation.c_str();
			}

			// Instantiate popup options
			ALLEGRO_FILECHOOSER* dialog = al_create_native_file_dialog(
				defaultLoc,
				"Save as",
				acceptedFiles.c_str(),
				0
			);

			// Open queued window
			if (!al_show_native_file_dialog(parentWindow, dialog)) {
				al_destroy_native_file_dialog(dialog);
				return "";       // File was cancelled
			}

			// A File was chosen
			std::string path = al_get_native_file_dialog_path(dialog, 0);

			// Free memory
			al_destroy_native_file_dialog(dialog);

			return path;
		}

		std::vector<std::string> PromptFileOpenDialogMultiple(const std::string& acceptedFiles, ALLEGRO_DISPLAY* parentWindow, const std::string& defaultLocation) {

			if (!Core::IsInitialized())
				throw Battery::Exception("The Engine is not initialized");

			// Load default location
			const char* defaultLoc = nullptr;
			if (defaultLocation != "") {
				defaultLoc = defaultLocation.c_str();
			}

			// Instantiate popup options
			ALLEGRO_FILECHOOSER* dialog = al_create_native_file_dialog(
				defaultLoc,
				"Save as",
				acceptedFiles.c_str(),
				ALLEGRO_FILECHOOSER_MULTIPLE
			);

			// Open queued window
			if (!al_show_native_file_dialog(parentWindow, dialog)) {
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

		File LoadFileWithDialog(const std::string& extension, ALLEGRO_DISPLAY* parentWindow, const std::string& defaultLocation) {

			if (!Core::IsInitialized())
				throw Battery::Exception("The Engine is not initialized");

			std::string path = PromptFileOpenDialog(std::string("*.*;*.") + extension + ";", parentWindow, defaultLocation);

			if (path == "")		// No file was chosen, return
				return File("", "", false);

			// Check if extension is correct
			if (GetExtensionFromPath(path) != std::string(".") + extension) {	// Wrong Extension
				Core::ShowWarningMessageBox(path + " has an incorrect file format. Please choose another file!", parentWindow);
				return LoadFileWithDialog(extension, parentWindow, defaultLocation);	// Try again by recursion
			}

			File file = LoadFile(path);

			if (file.fail()) {
				Core::ShowErrorMessageBox(path + " could not be loaded!", parentWindow);
				return File("", "", false);
			}

			return file;
		}













		std::string GetExecutablePath() {

			if (!Core::IsInitialized())
				throw Battery::Exception("The Engine is not initialized");

			return GetAllegroStandardPath(ALLEGRO_EXENAME_PATH);
		}

		std::string GetExecutableName() {

			if (!Core::IsInitialized())
				throw Battery::Exception("The Engine is not initialized");

			return GetFilenameFromPath(GetExecutablePath());
		}

		std::string GetExecutableDirectory() {

			if (!Core::IsInitialized())
				throw Battery::Exception("The Engine is not initialized");

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

			if (!Core::IsInitialized())
				throw Battery::Exception("The Engine is not initialized");

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
				throw Battery::Exception("Invalid enum supplied to GetAllegroStandardPath()");
			}

			return str;
		}
	}
}
