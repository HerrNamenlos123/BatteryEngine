#pragma once

#include "Battery/AllegroDeps.h"
#include "Battery/Core/AllegroContext.h"
#include "Battery/Log/Log.h"

#undef LoadBitmap

namespace Battery {

	class Texture2D {
	public:
		Texture2D() {}

		Texture2D(const Texture2D& texture) {
			if (texture.allegroBitmap != nullptr) {
				al_set_new_bitmap_flags(texture.GetFlags());
				allegroBitmap = al_clone_bitmap(texture.allegroBitmap);
			}
		}

		Texture2D(const std::string& path, int flags = 0) {
			Load(path, flags);
		}

		Texture2D(Texture2D& texture) {
			if (texture.allegroBitmap != nullptr) {
				allegroBitmap = al_clone_bitmap(texture.allegroBitmap);
			}
		}

		Texture2D(Texture2D&& texture) {
			if (texture.allegroBitmap != nullptr) {
				allegroBitmap = texture.allegroBitmap;
				texture.allegroBitmap = nullptr;
			}
		}

		~Texture2D() {
			if (!AllegroContext::GetInstance()->IsInitialized()) {
				LOG_CORE_CRITICAL(__FUNCTION__"(): Can't destroy texture: The Allegro Context is not loaded anymore!"
					" Make sure to unload or destroy the Texture2D object before Allegro is shut down!");
			}

			al_destroy_bitmap(allegroBitmap);
		}

		void operator=(const Texture2D& texture) {
			Unload();
			if (texture.allegroBitmap != nullptr) {
				allegroBitmap = al_clone_bitmap(texture.allegroBitmap);
			}
		}

		void operator=(Texture2D&& texture) {
			Unload();
			if (texture.allegroBitmap != nullptr) {
				allegroBitmap = texture.allegroBitmap;
				texture.allegroBitmap = nullptr;
			}
		}




		bool Load(const std::string& path, int flags = 0) {

			// If it's valid, unload first
			if (allegroBitmap != nullptr) {
				LOG_CORE_TRACE(__FUNCTION__"(): Bitmap is already loaded, overwriting previous...");
				Unload();
			}

			// Now load the new texture
			al_set_new_bitmap_flags(flags);
			allegroBitmap = al_load_bitmap(path.c_str());

			if (allegroBitmap == nullptr) {
				LOG_CORE_ERROR("Failed to load Allegro bitmap: '" + path + "'");
				return false;
			}

			return true;
		}

		bool SetFlags(int flags) {
			al_set_new_bitmap_flags(flags);
			ALLEGRO_BITMAP* temp = al_clone_bitmap(allegroBitmap);
			al_destroy_bitmap(allegroBitmap);
			allegroBitmap = temp;
			return allegroBitmap != nullptr;
		}

		int GetFlags() const {
			if (allegroBitmap != nullptr) {
				return al_get_bitmap_flags(allegroBitmap);
			}

			return 0;
		}






		ALLEGRO_BITMAP* GetAllegroBitmap() {
			return allegroBitmap;
		}

		void Unload() {
			if (!AllegroContext::GetInstance()->IsInitialized()) {
				throw Battery::Exception(__FUNCTION__"(): Can't destroy texture: The Allegro Context is not loaded anymore!"
					" Make sure to unload or destroy the Texture2D object before Allegro is shut down!");
			}

			al_destroy_bitmap(allegroBitmap);
			allegroBitmap = nullptr;
		}

	private:
		ALLEGRO_BITMAP* allegroBitmap = nullptr;
	};

}
