#pragma once

#include <string>
#include <iostream>

#include "glm/glm.hpp"
#include "Battery/Core.h"
#include "Battery/Exception.h"
#include "Battery/FileUtils.h"
#include "Battery/_AllegroDependencies.h"

namespace Battery {

	class ShaderProgram {

		bool loaded = false;
		ALLEGRO_DISPLAY* display = nullptr;	// This is just a supplied reference pointer, do not delete!!!
		ALLEGRO_SHADER* shader = nullptr;	// This is an object pointer and must be destroyed!

	public:

		/// <summary>
		/// Default constructor, no action.
		/// </summary>
		ShaderProgram() {
		}

		/// <summary>
		/// Load a shader program by supplying the paths of a vertex shader file and a fragment shader file.
		/// Any errors are printed to std::cout.
		/// </summary>
		/// <param name="display">- The Allegro display pointer</param>
		/// <param name="vertexShader">- The path to a file containing the vertex shader</param>
		/// <param name="fragmentShader">- The path to a file containing the fragment shader</param>
		/// <exception cref="Battery::Exception - Thrown when display flags are missing"></exception>
		/// <returns>bool - if loading the shader was successful</returns>
		ShaderProgram(ALLEGRO_DISPLAY* display, const std::string vertexShader, const std::string fragmentShader) {
			Load(display, vertexShader, fragmentShader);
		}

		~ShaderProgram() {
			Unload();
		}

		/// <summary>
		/// Load a shader program by supplying the paths of a vertex shader file and a fragment shader file.
		/// Any errors are printed to std::cout.
		/// </summary>
		/// <param name="display">- The Allegro display pointer</param>
		/// <param name="vertexShader">- The path to a file containing the vertex shader</param>
		/// <param name="fragmentShader">- The path to a file containing the fragment shader</param>
		/// <exception cref="Battery::Exception - Thrown when display flags are missing or the engine is not loaded"></exception>
		/// <returns>bool - if loading the shader was successful</returns>
		bool Load(ALLEGRO_DISPLAY* display, const std::string vertexShader, const std::string fragmentShader) {

			this->display = display;

			// Some checks in the beginning
			if (loaded) {
				std::cout << "Can't load shader program: Is already loaded, unload first" << std::endl;
				return false;
			}

			if (!FileUtils::FileExists(vertexShader)) {
				std::cout << "Can't find vertex shader file: " << vertexShader << std::endl;
				return false;
			}

			if (!FileUtils::FileExists(fragmentShader)) {
				std::cout << "Can't find fragment shader file: " << fragmentShader << std::endl;
				return false;
			}

			// Check if the display flags are correct
			int flags = al_get_display_flags(display);
			if (!(flags & ALLEGRO_PROGRAMMABLE_PIPELINE))
				throw Battery::Exception("Can't load Shader Program: The display flag 'ALLEGRO_PROGRAMMABLE_PIPELINE' was not set!");
			if (!(flags & ALLEGRO_OPENGL))
				throw Battery::Exception("Can't load Shader Program: The display flag 'ALLEGRO_OPENGL' was not set!");


			if (!Core::IsInitialized())
				throw Battery::Exception("Can't load Shader Program: The Engine was not initialized!");

			// Now load everything
			shader = al_create_shader(ALLEGRO_SHADER_GLSL);

			if (!shader) {
				std::cout << "Failed to create shader" << std::endl;
				return false;
			}

			if (!al_attach_shader_source_file(shader, ALLEGRO_VERTEX_SHADER, vertexShader.c_str())) {
				std::cout << "Vertex shader failed: " << al_get_shader_log(shader) << std::endl;
				al_destroy_shader(shader);
				return false;
			}

			if (!al_attach_shader_source_file(shader, ALLEGRO_PIXEL_SHADER, fragmentShader.c_str())) {
				std::cout << "Fragment shader failed: " << al_get_shader_log(shader) << std::endl;
				al_destroy_shader(shader);
				return false;
			}

			if (!al_build_shader(shader)) {
				std::cout << "Shader linking failed: " << al_get_shader_log(shader) << std::endl;
				al_destroy_shader(shader);
				return false;
			}

			loaded = true;
			return true;
		}

		/// <summary>
		/// Load a shader program by directly supplying the shader sources as strings.
		/// Any errors are printed to std::cout.
		/// </summary>
		/// <param name="display">- The Allegro display pointer</param>
		/// <param name="vertexShader">- The vertex shader source code</param>
		/// <param name="fragmentShader">- The fragment shader source code</param>
		/// <exception cref="Battery::Exception - Thrown when display flags are missing or the engine is not loaded"></exception>
		/// <returns>bool - if loading the shader was successful</returns>
		bool LoadSource(ALLEGRO_DISPLAY* display, const std::string vertexShader, const std::string fragmentShader) {

			if (display == nullptr)
				throw Battery::Exception("The Allegro display pointer is NULL!");

			this->display = display;

			// Some checks in the beginning
			if (loaded) {
				std::cout << "Can't load shader program: Is already loaded, unload first" << std::endl;
				return false;
			}

			// Check if the display flags are correct
			int flags = al_get_display_flags(display);
			if (!(flags & ALLEGRO_PROGRAMMABLE_PIPELINE))
				throw Battery::Exception("Can't load Shader Program: The display flag 'ALLEGRO_PROGRAMMABLE_PIPELINE' was not set!");
			if (!(flags & ALLEGRO_OPENGL))
				throw Battery::Exception("Can't load Shader Program: The display flag 'ALLEGRO_OPENGL' was not set!");


			if (!Core::IsInitialized())
				throw Battery::Exception("Can't load Shader Program: The Engine was not initialized!");

			// Now load everything
			shader = al_create_shader(ALLEGRO_SHADER_GLSL);

			if (!shader) {
				std::cout << "Failed to create shader" << std::endl;
				return false;
			}

			if (!al_attach_shader_source(shader, ALLEGRO_VERTEX_SHADER, vertexShader.c_str())) {
				std::cout << "Vertex shader failed: " << al_get_shader_log(shader) << std::endl;
				al_destroy_shader(shader);
				return false;
			}

			if (!al_attach_shader_source(shader, ALLEGRO_PIXEL_SHADER, fragmentShader.c_str())) {
				std::cout << "Fragment shader failed: " << al_get_shader_log(shader) << std::endl;
				al_destroy_shader(shader);
				return false;
			}

			if (!al_build_shader(shader)) {
				std::cout << "Shader linking failed: " << al_get_shader_log(shader) << std::endl;
				al_destroy_shader(shader);
				return false;
			}

			loaded = true;
			return true;
		}

		/// <summary>
		/// Unload the shader program if it was loaded. 
		/// Called automatically in the class destructor.
		/// <exception cref="Battery::Exception - Thrown when engine is not loaded"></exception>
		/// </summary>
		void Unload() {

			if (loaded) {

				if (!Core::IsInitialized())
					throw Battery::Exception("Can't unload Shader Program: The Engine is not initialized, .Unload() "
						"must be called in Shutdown(), as when the Application destructor is called, the engine is already down!");

				al_destroy_shader(shader);
				shader = nullptr;
				loaded = false;
			}
		}

		bool Loaded() {
			return loaded;
		}

		/// <summary>
		/// Tell OpenGL to use this shader program.
		/// </summary>
		void Use() {
			al_use_shader(shader);
		}

		/// <summary>
		/// Tell OpenGL to no longer use this shader program, 
		/// but the default one.
		/// </summary>
		void Release () {
			al_use_shader(NULL);
		}

		/// <summary>
		/// Sets a uniform with the given name to the supplied texture at the given ID, 
		/// the default ID is 0, should be different with multiple textures. The uniform
		/// is set for the currently bound Shader program, must be bound with .Use() before this call!
		/// On failure, a warning is printed to std::cout.
		/// </summary>
		/// <param name="name">- The name of the uniform in the glsl shader</param>
		/// <param name="texture">- An Allegro Bitmap, which contains the texture</param>
		/// <param name="ID">- The OpenGL Texture ID, 0 or more for several textures</param>
		/// <returns>bool - If setting the uniform was successful</returns>
		bool SetUniformSampler(const std::string name, ALLEGRO_BITMAP* texture, int ID) {
			if (!al_set_shader_sampler(name.c_str(), texture, ID)) {
				std::cout << "WARNING: The Shader Sampler uniform '" << name << "' was not set correctly or is not being used!" << std::endl;
				return false;
			}

			return true;
		}

		/// <summary>
		/// Sets a uniform with the given name to the supplied matrix. The uniform
		/// is set for the currently bound Shader program, must be bound with .Use() before this call!
		/// On failure, a warning is printed to std::cout.
		/// </summary>
		/// <param name="name">- The name of the uniform in the glsl shader</param>
		/// <param name="matrix">- The 4x4 matrix</param>
		/// <returns>bool - If setting the uniform was successful</returns>
		bool SetUniformMatrix(const std::string name, glm::mat4 matrix) {

			ALLEGRO_TRANSFORM m;
			for (int x = 0; x < 4; x++)
				for (int y = 0; y < 4; y++)
					m.m[x][y] = matrix[x][y];

			if (!al_set_shader_matrix(name.c_str(), &m)) {
				std::cout << "WARNING: The Shader Matrix uniform '" << name << "' was not set correctly or is not being used!" << std::endl;
				return false;
			}

			return true;
		}

		/// <summary>
		/// Sets a uniform with the given name to the supplied integer value. The uniform
		/// is set for the currently bound Shader program, must be bound with .Use() before this call!
		/// On failure, a warning is printed to std::cout.
		/// </summary>
		/// <param name="name">- The name of the uniform in the glsl shader</param>
		/// <param name="n">- The integer value</param>
		/// <returns>bool - If setting the uniform was successful</returns>
		bool SetUniformInt(const std::string name, int n) {

			if (!al_set_shader_int(name.c_str(), n)) {
				std::cout << "WARNING: The Shader Integer uniform '" << name << "' was not set correctly or is not being used!" << std::endl;
				return false;
			}

			return true;
		}

		/// <summary>
		/// Sets a uniform with the given name to the supplied integer values. The uniform
		/// is set for the currently bound Shader program, must be bound with .Use() before this call!
		/// On failure, a warning is printed to std::cout.
		/// </summary>
		/// <param name="name">- The name of the uniform in the glsl shader</param>
		/// <param name="n">- The integer vector</param>
		/// <returns>bool - If setting the uniform was successful</returns>
		bool SetUniformInt(const std::string name, glm::ivec2 n) {

			if (!al_set_shader_int_vector(name.c_str(), 2, &n[0], 1)) {
				std::cout << "WARNING: The Shader Integer uniform '" << name << "' was not set correctly or is not being used!" << std::endl;
				return false;
			}

			return true;
		}

		/// <summary>
		/// Sets a uniform with the given name to the supplied integer values. The uniform
		/// is set for the currently bound Shader program, must be bound with .Use() before this call!
		/// On failure, a warning is printed to std::cout.
		/// </summary>
		/// <param name="name">- The name of the uniform in the glsl shader</param>
		/// <param name="n">- The integer vector</param>
		/// <returns>bool - If setting the uniform was successful</returns>
		bool SetUniformInt(const std::string name, glm::ivec3 n) {

			if (!al_set_shader_int_vector(name.c_str(), 3, &n[0], 1)) {
				std::cout << "WARNING: The Shader Integer uniform '" << name << "' was not set correctly or is not being used!" << std::endl;
				return false;
			}

			return true;
		}

		/// <summary>
		/// Sets a uniform with the given name to the supplied integer values. The uniform
		/// is set for the currently bound Shader program, must be bound with .Use() before this call!
		/// On failure, a warning is printed to std::cout.
		/// </summary>
		/// <param name="name">- The name of the uniform in the glsl shader</param>
		/// <param name="n">- The integer vector</param>
		/// <returns>bool - If setting the uniform was successful</returns>
		bool SetUniformInt(const std::string name, glm::ivec4 n) {

			if (!al_set_shader_int_vector(name.c_str(), 4, &n[0], 1)) {
				std::cout << "WARNING: The Shader Integer uniform '" << name << "' was not set correctly or is not being used!" << std::endl;
				return false;
			}

			return true;
		}

		/// <summary>
		/// Sets a uniform with the given name to the supplied float value. The uniform
		/// is set for the currently bound Shader program, must be bound with .Use() before this call!
		/// On failure, a warning is printed to std::cout.
		/// </summary>
		/// <param name="name">- The name of the uniform in the glsl shader</param>
		/// <param name="n">- The float value</param>
		/// <returns>bool - If setting the uniform was successful</returns>
		bool SetUniformFloat(const std::string name, float n) {

			if (!al_set_shader_float(name.c_str(), n)) {
				std::cout << "WARNING: The Shader Float uniform '" << name << "' was not set correctly or is not being used!" << std::endl;
				return false;
			}

			return true;
		}

		/// <summary>
		/// Sets a uniform with the given name to the supplied float values. The uniform
		/// is set for the currently bound Shader program, must be bound with .Use() before this call!
		/// On failure, a warning is printed to std::cout.
		/// </summary>
		/// <param name="name">- The name of the uniform in the glsl shader</param>
		/// <param name="n">- The float vector</param>
		/// <returns>bool - If setting the uniform was successful</returns>
		bool SetUniformFloat(const std::string name, glm::vec2 n) {

			if (!al_set_shader_float_vector(name.c_str(), 2, &n[0], 1)) {
				std::cout << "WARNING: The Shader Float uniform '" << name << "' was not set correctly or is not being used!" << std::endl;
				return false;
			}

			return true;
		}

		/// <summary>
		/// Sets a uniform with the given name to the supplied float values. The uniform
		/// is set for the currently bound Shader program, must be bound with .Use() before this call!
		/// On failure, a warning is printed to std::cout.
		/// </summary>
		/// <param name="name">- The name of the uniform in the glsl shader</param>
		/// <param name="n">- The float vector</param>
		/// <returns>bool - If setting the uniform was successful</returns>
		bool SetUniformFloat(const std::string name, glm::vec3 n) {

			if (!al_set_shader_float_vector(name.c_str(), 3, &n[0], 1)) {
				std::cout << "WARNING: The Shader Float uniform '" << name << "' was not set correctly or is not being used!" << std::endl;
				return false;
			}

			return true;
		}

		/// <summary>
		/// Sets a uniform with the given name to the supplied float values. The uniform
		/// is set for the currently bound Shader program, must be bound with .Use() before this call!
		/// On failure, a warning is printed to std::cout.
		/// </summary>
		/// <param name="name">- The name of the uniform in the glsl shader</param>
		/// <param name="n">- The float vector</param>
		/// <returns>bool - If setting the uniform was successful</returns>
		bool SetUniformFloat(const std::string name, glm::vec4 n) {

			if (!al_set_shader_float_vector(name.c_str(), 4, &n[0], 1)) {
				std::cout << "WARNING: The Shader Float uniform '" << name << "' was not set correctly or is not being used!" << std::endl;
				return false;
			}

			return true;
		}

		/// <summary>
		/// Sets a uniform with the given name to the supplied bool value. The uniform
		/// is set for the currently bound Shader program, must be bound with .Use() before this call!
		/// On failure, a warning is printed to std::cout.
		/// </summary>
		/// <param name="name">- The name of the uniform in the glsl shader</param>
		/// <param name="n">- The bool value</param>
		/// <returns>bool - If setting the uniform was successful</returns>
		bool SetUniformBool(const std::string name, bool n) {

			if (!al_set_shader_bool(name.c_str(), n)) {
				std::cout << "WARNING: The Shader Bool uniform '" << name << "' was not set correctly or is not being used!" << std::endl;
				return false;
			}

			return true;
		}

	};

}
