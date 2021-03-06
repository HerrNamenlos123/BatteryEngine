
#include "Battery/pch.h"
#include "Battery/Renderer/ShaderProgram.h"
#include "Battery/Core/AllegroContext.h"
#include "Battery/Utils/FileUtils.h"
#include "Battery/Log/Log.h"

namespace Battery {

	ShaderProgram::ShaderProgram() {

	}

	ShaderProgram::ShaderProgram(ALLEGRO_DISPLAY* display, const std::string vertexShader, const std::string fragmentShader) {
		LOG_CORE_TRACE("Constructing Battery::ShaderProgram");
		Load(display, vertexShader, fragmentShader);
	}

	ShaderProgram::~ShaderProgram() {
		LOG_CORE_TRACE("Destructing Battery::ShaderProgram");

		if (loaded)
			al_destroy_shader(shader);
	}








	bool ShaderProgram::Load(ALLEGRO_DISPLAY* display, const std::string vertexShader, const std::string fragmentShader) {
		LOG_CORE_TRACE(__FUNCTION__ "()");

		this->display = display;

		// Some checks in the beginning
		if (loaded) {
			LOG_CORE_ERROR("Can't load shader program: Is already loaded, unload first");
			return false;
		}

		if (!FileUtils::FileExists(vertexShader)) {
			LOG_CORE_ERROR("Can't find vertex shader file: " + vertexShader);
			return false;
		}

		if (!FileUtils::FileExists(fragmentShader)) {
			LOG_CORE_ERROR("Can't find fragment shader file: " + fragmentShader);
			return false;
		}

		// Check if the display flags are correct
		int flags = al_get_display_flags(display);
		if (!(flags & ALLEGRO_PROGRAMMABLE_PIPELINE))
			throw Battery::Exception("Can't load Shader Program: The display flag 'ALLEGRO_PROGRAMMABLE_PIPELINE' was not set!");
		if (!(flags & ALLEGRO_OPENGL))
			throw Battery::Exception("Can't load Shader Program: The display flag 'ALLEGRO_OPENGL' was not set!");


		if (!AllegroContext::GetInstance()->IsInitialized())
			throw Battery::Exception("Can't load Shader Program: The Allegro Context was not initialized yet!");

		// Now load everything
		shader = al_create_shader(ALLEGRO_SHADER_GLSL);

		if (!shader) {
			LOG_CORE_ERROR("Failed to create Allegro shader");
			return false;
		}

		if (!al_attach_shader_source_file(shader, ALLEGRO_VERTEX_SHADER, vertexShader.c_str())) {
			LOG_CORE_ERROR(std::string("Vertex shader failed: ") + al_get_shader_log(shader));
			al_destroy_shader(shader);
			return false;
		}

		if (!al_attach_shader_source_file(shader, ALLEGRO_PIXEL_SHADER, fragmentShader.c_str())) {
			LOG_CORE_ERROR(std::string("Fragment shader failed: ") + al_get_shader_log(shader));
			al_destroy_shader(shader);
			return false;
		}

		if (!al_build_shader(shader)) {
			LOG_CORE_ERROR(std::string("Shader linking failed: ") + al_get_shader_log(shader));
			al_destroy_shader(shader);
			return false;
		}

		loaded = true;
		return true;
	}

	bool ShaderProgram::LoadSource(ALLEGRO_DISPLAY* display, const std::string vertexShader, const std::string fragmentShader) {
		LOG_CORE_TRACE(__FUNCTION__ "()");

		if (display == nullptr)
			throw Battery::Exception("The Allegro display pointer is NULL!");

		this->display = display;

		// Some checks in the beginning
		if (loaded) {
			LOG_CORE_ERROR("Can't load shader program: Is already loaded, unload first");
			return false;
		}

		// Check if the display flags are correct
		int flags = al_get_display_flags(display);
		if (!(flags & ALLEGRO_PROGRAMMABLE_PIPELINE))
			throw Battery::Exception("Can't load Shader Program: The display flag 'ALLEGRO_PROGRAMMABLE_PIPELINE' was not set!");
		if (!(flags & ALLEGRO_OPENGL))
			throw Battery::Exception("Can't load Shader Program: The display flag 'ALLEGRO_OPENGL' was not set!");


		if (!AllegroContext::GetInstance()->IsInitialized())
			throw Battery::Exception("Can't load Shader Program: The Allegro Context was not initialized yet!");

		// Now load everything
		shader = al_create_shader(ALLEGRO_SHADER_GLSL);

		if (!shader) {
			LOG_CORE_ERROR("Failed to create Allegro shader");
			return false;
		}

		if (!al_attach_shader_source(shader, ALLEGRO_VERTEX_SHADER, vertexShader.c_str())) {
			LOG_CORE_ERROR(std::string("Vertex shader failed: ") + al_get_shader_log(shader));
			al_destroy_shader(shader);
			return false;
		}

		if (!al_attach_shader_source(shader, ALLEGRO_PIXEL_SHADER, fragmentShader.c_str())) {
			LOG_CORE_ERROR(std::string("Fragment shader failed: ") + al_get_shader_log(shader));
			al_destroy_shader(shader);
			return false;
		}

		if (!al_build_shader(shader)) {
			LOG_CORE_ERROR(std::string("Shader linking failed: ") + al_get_shader_log(shader));
			al_destroy_shader(shader);
			return false;
		}

		loaded = true;
		return true;
	}

	void ShaderProgram::Unload() {
		LOG_CORE_TRACE(__FUNCTION__ "()");

		if (loaded) {

			if (!AllegroContext::GetInstance()->IsInitialized())
				throw Battery::Exception("Can't unload Shader Program: The Engine is not initialized, .Unload() "
					"must be called in Shutdown(), as when the Application destructor is called, the engine is already down!");

			al_destroy_shader(shader);
			shader = nullptr;
			loaded = false;
		}
	}

	bool ShaderProgram::IsLoaded() {
		return loaded;
	}

	void ShaderProgram::Use() {
		LOG_CORE_TRACE(__FUNCTION__ "()");
		al_use_shader(shader);
	}

	void ShaderProgram::Release() {
		LOG_CORE_TRACE(__FUNCTION__ "()");
		al_use_shader(NULL);
	}







	// Uniforms

	bool ShaderProgram::SetUniformSampler(const std::string name, ALLEGRO_BITMAP* texture, int ID) {
		LOG_CORE_TRACE(__FUNCTION__ "()");
		if (!al_set_shader_sampler(name.c_str(), texture, ID)) {
			LOG_CORE_WARN("WARNING: The Shader Sampler uniform '" + name + "' was not set correctly or is not being used!");
			return false;
		}

		return true;
	}

	bool ShaderProgram::SetUniformMatrix(const std::string name, glm::mat4 matrix) {
		LOG_CORE_TRACE(__FUNCTION__ "()");

		ALLEGRO_TRANSFORM m;
		for (int x = 0; x < 4; x++)
			for (int y = 0; y < 4; y++)
				m.m[x][y] = matrix[x][y];

		if (!al_set_shader_matrix(name.c_str(), &m)) {
			LOG_CORE_WARN("WARNING: The Shader Matrix uniform '" + name + "' was not set correctly or is not being used!");
			return false;
		}

		return true;
	}

	bool ShaderProgram::SetUniformInt(const std::string name, int n) {
		LOG_CORE_TRACE(__FUNCTION__ "()");

		if (!al_set_shader_int(name.c_str(), n)) {
			LOG_CORE_WARN("WARNING: The Shader Integer uniform '" + name + "' was not set correctly or is not being used!");
			return false;
		}

		return true;
	}

	bool ShaderProgram::SetUniformInt(const std::string name, glm::ivec2 n) {
		LOG_CORE_TRACE(__FUNCTION__ "()");

		if (!al_set_shader_int_vector(name.c_str(), 2, &n[0], 1)) {
			LOG_CORE_WARN("WARNING: The Shader Integer uniform '" + name + "' was not set correctly or is not being used!");
			return false;
		}

		return true;
	}

	bool ShaderProgram::SetUniformInt(const std::string name, glm::ivec3 n) {
		LOG_CORE_TRACE(__FUNCTION__ "()");

		if (!al_set_shader_int_vector(name.c_str(), 3, &n[0], 1)) {
			LOG_CORE_WARN("WARNING: The Shader Integer uniform '" + name + "' was not set correctly or is not being used!");
			return false;
		}

		return true;
	}

	bool ShaderProgram::SetUniformInt(const std::string name, glm::ivec4 n) {
		LOG_CORE_TRACE(__FUNCTION__ "()");

		if (!al_set_shader_int_vector(name.c_str(), 4, &n[0], 1)) {
			LOG_CORE_WARN("WARNING: The Shader Integer uniform '" + name + "' was not set correctly or is not being used!");
			return false;
		}

		return true;
	}

	bool ShaderProgram::SetUniformFloat(const std::string name, float n) {
		LOG_CORE_TRACE(__FUNCTION__ "()");

		if (!al_set_shader_float(name.c_str(), n)) {
			LOG_CORE_WARN("WARNING: The Shader Float uniform '" + name + "' was not set correctly or is not being used!");
			return false;
		}

		return true;
	}

	bool ShaderProgram::SetUniformFloat(const std::string name, glm::vec2 n) {
		LOG_CORE_TRACE(__FUNCTION__ "()");

		if (!al_set_shader_float_vector(name.c_str(), 2, &n[0], 1)) {
			LOG_CORE_WARN("WARNING: The Shader Float uniform '" + name + "' was not set correctly or is not being used!");
			return false;
		}

		return true;
	}

	bool ShaderProgram::SetUniformFloat(const std::string name, glm::vec3 n) {
		LOG_CORE_TRACE(__FUNCTION__ "()");

		if (!al_set_shader_float_vector(name.c_str(), 3, &n[0], 1)) {
			LOG_CORE_WARN("WARNING: The Shader Float uniform '" + name + "' was not set correctly or is not being used!");
			return false;
		}

		return true;
	}

	bool ShaderProgram::SetUniformFloat(const std::string name, glm::vec4 n) {
		LOG_CORE_TRACE(__FUNCTION__ "()");

		if (!al_set_shader_float_vector(name.c_str(), 4, &n[0], 1)) {
			LOG_CORE_WARN("WARNING: The Shader Float uniform '" + name + "' was not set correctly or is not being used!");
			return false;
		}

		return true;
	}

	bool ShaderProgram::SetUniformBool(const std::string name, bool n) {
		LOG_CORE_TRACE(__FUNCTION__ "()");

		if (!al_set_shader_bool(name.c_str(), n)) {
			LOG_CORE_WARN("WARNING: The Shader Bool uniform '" + name + "' was not set correctly or is not being used!");
			return false;
		}

		return true;
	}
}