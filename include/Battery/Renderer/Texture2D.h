#pragma once

#include "Battery/AllegroDeps.h"
#include "Battery/Core/AllegroContext.h"
#include "Battery/Log/Log.h"
#include "clip.h"

#undef LoadBitmap

namespace Battery {

	class Texture2D {
	public:
		Texture2D();
		Texture2D(const Texture2D& texture);
		Texture2D(const std::string& path, int flags = 0);
		Texture2D(Texture2D& texture);
		Texture2D(Texture2D&& texture) noexcept;
		Texture2D(int width, int height, int flags = 0);
		Texture2D(const clip::image& image, int flags = 0);
		~Texture2D();

		void operator=(const Texture2D& texture);
		void operator=(Texture2D&& texture);

		bool Load(const std::string& path, int flags = 0);
		bool Load(ALLEGRO_BITMAP* bitmap, int flags = 0);
		bool SetFlags(int flags);
		int GetFlags() const;
		int GetWidth() const;
		int GetHeight() const;

		bool SaveImage(const std::string& file) const;
		std::optional<std::pair<std::vector<uint32_t>, clip::image_spec>> GetClipImage() const;
		bool LoadEmbeddedResource(int id);

		ALLEGRO_BITMAP* GetAllegroBitmap() const;
		bool CreateBitmap(int width, int height, int flags = 0);
		void Unload();
		bool IsValid() const;

	private:
		ALLEGRO_BITMAP* allegroBitmap = nullptr;
	};

}
