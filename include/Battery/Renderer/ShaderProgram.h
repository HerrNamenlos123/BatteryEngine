#pragma once

#include "Battery/pch.h"

#include "glm/glm.hpp"
#include "Battery/Core.h"
#include "Battery/Core/Exception.h"
#include "Battery/AllegroDeps.h"

namespace Battery {

	class ShaderProgram {
	public:

		ShaderProgram();
		ShaderProgram(ALLEGRO_DISPLAY* display, const std::string vertexShader, const std::string fragmentShader);
		~ShaderProgram();

		bool Load(ALLEGRO_DISPLAY* display, const std::string vertexShader, const std::string fragmentShader);
		bool LoadSource(ALLEGRO_DISPLAY* display, const std::string vertexShader, const std::string fragmentShader);

		void Unload();
		bool IsLoaded();

		void Use();
		void Release();

		bool SetUniformSampler(const std::string name, ALLEGRO_BITMAP* texture, int ID);
		bool SetUniformMatrix(const std::string name, glm::mat4 matrix);
		bool SetUniformInt(const std::string name, int n);
		bool SetUniformInt(const std::string name, glm::ivec2 n);
		bool SetUniformInt(const std::string name, glm::ivec3 n);
		bool SetUniformInt(const std::string name, glm::ivec4 n);
		bool SetUniformFloat(const std::string name, float n);
		bool SetUniformFloat(const std::string name, glm::vec2 n);
		bool SetUniformFloat(const std::string name, glm::vec3 n);
		bool SetUniformFloat(const std::string name, glm::vec4 n);
		bool SetUniformBool(const std::string name, bool n);

	private:

		bool loaded = false;
		ALLEGRO_DISPLAY* display = nullptr;	// This is just a supplied reference pointer, do not delete!!!
		ALLEGRO_SHADER* shader = nullptr;	// This is an object pointer and must be destroyed!

	};

}
