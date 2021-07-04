#pragma once

#include "Battery/pch.h"
#include "Battery/Renderer/Texture2D.h"
#include "Battery/Graphics.h"

#undef LoadBitmap

namespace Battery {

	Texture2D::Texture2D() {}

	Texture2D::Texture2D(const Texture2D& texture) {
		if (texture.allegroBitmap != nullptr) {
			al_set_new_bitmap_flags(texture.GetFlags());
			allegroBitmap = al_clone_bitmap(texture.allegroBitmap);
		}
	}

	Texture2D::Texture2D(const std::string& path, int flags) {
		Load(path, flags);
	}

	Texture2D::Texture2D(Texture2D& texture) {
		if (texture.allegroBitmap != nullptr) {
			allegroBitmap = al_clone_bitmap(texture.allegroBitmap);
		}
	}

	Texture2D::Texture2D(Texture2D&& texture) noexcept {
		if (texture.allegroBitmap != nullptr) {
			allegroBitmap = texture.allegroBitmap;
			texture.allegroBitmap = nullptr;
		}
	}

	Texture2D::Texture2D(int width, int height, int flags) {
		CreateBitmap(width, height, flags);
	}

	Texture2D::Texture2D(const clip::image& image, int flags) {
		const clip::image_spec& spec = image.spec();

		if (!CreateBitmap(spec.width, spec.height, flags))
			throw Battery::Exception(__FUNCTION__"(): Can't load image: Allegro Bitmap could not be created!");

		LOG_CORE_TRACE(__FUNCTION__"(): Loading Battery::Texture2D from clip::image now!");

		// Now load the image data
		ALLEGRO_BITMAP* oldBuffer = al_get_target_bitmap();
		al_set_target_bitmap(allegroBitmap);
		al_lock_bitmap(allegroBitmap, al_get_bitmap_format(allegroBitmap), ALLEGRO_LOCK_WRITEONLY);

		//LOG_WARN("width: {}", spec.width);
		//LOG_WARN("height: {}", spec.height);
		//LOG_WARN("bits_per_pixel: {}", spec.bits_per_pixel);
		//LOG_WARN("bytes_per_row: {}", spec.bytes_per_row);
		//LOG_WARN("red_mask: {}", spec.red_mask);
		//LOG_WARN("green_mask: {}", spec.green_mask);
		//LOG_WARN("blue_mask: {}", spec.blue_mask);
		//LOG_WARN("alpha_mask: {}", spec.alpha_mask);
		//LOG_WARN("red_shift: {}", spec.red_shift);
		//LOG_WARN("green_shift: {}", spec.green_shift);
		//LOG_WARN("blue_shift: {}", spec.blue_shift);
		//LOG_WARN("alpha_shift: {}", spec.alpha_shift);

		uint8_t* data = (uint8_t*)image.data();
		size_t row_offset = 0;

		for (unsigned long y = 0; y < spec.height; y++) {

			size_t byte_offset = 0;
			for (unsigned long x = 0; x < spec.width; x++) {

				uint32_t pixel = 0;
				memcpy(&pixel, data + row_offset + byte_offset, spec.bits_per_pixel / 8);

				uint32_t red = (pixel & spec.red_mask) >> spec.red_shift;
				uint32_t green = (pixel & spec.green_mask) >> spec.green_shift;
				uint32_t blue = (pixel & spec.blue_mask) >> spec.blue_shift;
				uint32_t alpha = (pixel & spec.alpha_mask) >> spec.alpha_shift;

				byte_offset += spec.bits_per_pixel / 8;

				if (spec.alpha_mask == 0) alpha = 255;

				al_put_pixel(x, y, Graphics::ConvertAllegroColor(glm::vec4(red, green, blue, alpha)));
			}
			row_offset += spec.bytes_per_row;
		}

		al_unlock_bitmap(allegroBitmap);
		al_set_target_bitmap(oldBuffer);
	}

	Texture2D::~Texture2D() {
		if (!AllegroContext::GetInstance()->IsInitialized()) {
			LOG_CORE_CRITICAL(__FUNCTION__"(): Can't destroy texture: The Allegro Context is not loaded anymore!"
				" Make sure to unload or destroy the Texture2D object before Allegro is shut down!");
		}

		if (allegroBitmap != nullptr)
			al_destroy_bitmap(allegroBitmap);
	}

	void Texture2D::operator=(const Texture2D& texture) {
		Unload();
		if (texture.allegroBitmap != nullptr) {
			allegroBitmap = al_clone_bitmap(texture.allegroBitmap);
		}
	}

	void Texture2D::operator=(Texture2D&& texture) {
		Unload();
		if (texture.allegroBitmap != nullptr) {
			allegroBitmap = texture.allegroBitmap;
			texture.allegroBitmap = nullptr;
		}
	}




	bool Texture2D::Load(const std::string& path, int flags) {

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

	bool Texture2D::Load(ALLEGRO_BITMAP* bitmap, int flags) {

		// If it's valid, unload first
		if (allegroBitmap != nullptr) {
			LOG_CORE_TRACE(__FUNCTION__"(): Bitmap is already loaded, overwriting previous...");
			Unload();
		}

		// Now clone the new image
		al_set_new_bitmap_flags(flags);
		allegroBitmap = al_clone_bitmap(bitmap);
		return allegroBitmap != nullptr;
	}

	bool Texture2D::SetFlags(int flags) {
		al_set_new_bitmap_flags(flags);
		ALLEGRO_BITMAP* temp = al_clone_bitmap(allegroBitmap);
		al_destroy_bitmap(allegroBitmap);
		allegroBitmap = temp;
		return allegroBitmap != nullptr;
	}

	int Texture2D::GetFlags() const {
		if (allegroBitmap != nullptr) {
			return al_get_bitmap_flags(allegroBitmap);
		}

		return 0;
	}

	int Texture2D::GetWidth() const {

		if (!IsValid())
			return 0;

		return al_get_bitmap_width(allegroBitmap);
	}

	int Texture2D::GetHeight() const {

		if (!IsValid())
			return 0;

		return al_get_bitmap_height(allegroBitmap);
	}





	bool Texture2D::SaveImage(const std::string& file) const {

		if (allegroBitmap == nullptr) {
			return false;
		}

		return al_save_bitmap(file.c_str(), allegroBitmap);
	}

	std::optional<std::pair<std::vector<uint32_t>, clip::image_spec>> Texture2D::GetClipImage() const {

		if (!IsValid())
			return std::nullopt;

		LOG_CORE_TRACE(__FUNCTION__"(): Generating clip::image now!");

		uint64_t width = al_get_bitmap_width(allegroBitmap);
		uint64_t height = al_get_bitmap_height(allegroBitmap);

		// Create an image with 8-bit RGBA values (32 bit per pixel)
		std::vector<uint32_t> data(width * height, 0);

		// Now load the image data
		ALLEGRO_BITMAP* oldBuffer = al_get_target_bitmap();
		al_set_target_bitmap(allegroBitmap);
		al_lock_bitmap(allegroBitmap, al_get_bitmap_format(allegroBitmap), ALLEGRO_LOCK_READONLY);

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {

				glm::vec4 color = Graphics::ConvertAllegroColor(al_get_pixel(allegroBitmap, x, y));
				data[y * width + x] = (uint32_t)color.a << 24 | 
									  (uint32_t)color.b << 16 | 
									  (uint32_t)color.g << 8 | 
									  (uint32_t)color.r;
			}
		}

		al_unlock_bitmap(allegroBitmap);
		al_set_target_bitmap(oldBuffer);

		// Now setup the image specs
		clip::image_spec spec;
		spec.width = width;
		spec.height = height;
		spec.bits_per_pixel = 32;
		spec.bytes_per_row = width * 4;
		spec.red_mask = 0x000000FF;
		spec.green_mask = 0x0000FF00;
		spec.blue_mask = 0x00FF0000;
		spec.alpha_mask = 0xFF000000;
		spec.red_shift = 0;
		spec.green_shift = 8;
		spec.blue_shift = 16;
		spec.alpha_shift = 24;

		return std::make_optional<std::pair<std::vector<uint32_t>, clip::image_spec>>(std::make_pair(std::move(data), spec));
	}

	bool Texture2D::LoadEmbeddedResource(int id) {
		HMODULE hMod = GetModuleHandle(NULL);
		HRSRC hRes = FindResource(hMod, MAKEINTRESOURCE(id), L"PNG");
		HGLOBAL hGlobal = LoadResource(hMod, hRes);
		void* data = LockResource(hGlobal);
		size_t size = SizeofResource(hMod, hRes);

		if (data == nullptr || size == 0)
			return false;

		ALLEGRO_FILE* file = al_open_memfile(data, size, "r");
		ALLEGRO_BITMAP* bmp = al_load_bitmap_f(file, ".png");

		bool success = Load(bmp, 0);

		al_fclose(file);
		al_destroy_bitmap(bmp);

		return success;
	}







	ALLEGRO_BITMAP* Texture2D::GetAllegroBitmap() const {
		return allegroBitmap;
	}

	bool Texture2D::CreateBitmap(int width, int height, int flags) {
		Unload();

		al_set_new_bitmap_flags(flags);
		allegroBitmap = al_create_bitmap(width, height);

		return allegroBitmap != nullptr;
	}

	void Texture2D::Unload() {
		if (!AllegroContext::GetInstance()->IsInitialized()) {
			throw Battery::Exception(__FUNCTION__"(): Can't destroy texture: The Allegro Context is not loaded anymore!"
				" Make sure to unload or destroy the Texture2D object before Allegro is shut down!");
		}

		al_destroy_bitmap(allegroBitmap);
		allegroBitmap = nullptr;
	}

	bool Texture2D::IsValid() const {
		return allegroBitmap;
	}

}
