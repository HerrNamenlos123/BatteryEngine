
#include "Battery/pch.h"
#include "Battery/Renderer/Renderer2D.h"
#include "Battery/Core/Exception.h"
#include "Battery/Core/AllegroContext.h"
#include "Battery/Log/Log.h"
#include "Battery/Core/Config.h"

#ifdef BATTERY_DEBUG
#define CHECK_INIT() \
	if (data == nullptr) { \
		LOG_CORE_ERROR(__FUNCTION__ "(): Renderer is not initialized!"); \
		return;	\
	}
#else
#define CHECK_INIT()
#endif

namespace Battery {

	VertexData::operator ALLEGRO_VERTEX() const {
		ALLEGRO_VERTEX v;

		v.x = position.x;
		v.y = position.y;
		v.z = position.z;
		v.u = uv.x;
		v.v = uv.y;
		v.color = Renderer2D::ConvertAllegroColor(color);

		return v;
	}

	struct QuadData {
		VertexData v1;
		VertexData v2;
		VertexData v3;
		VertexData v4;
		int texture;
	};

	struct LineData {
		glm::vec2 p1;
		glm::vec2 p2;
		float thickness;
		float falloff;
		glm::vec4 color;
	};

	struct CircleData {
		glm::vec2 center;
		float diameter;
		float falloff;
		glm::vec4 color;
	};

	struct Renderer2DData {
		Scene* currentScene = nullptr;	// This is a Scene reference, do not delete

		std::vector<LineData> lineBuffer;
		bool linesActive = false;

		std::vector<CircleData> circleBuffer;
		bool circlesActive = false;

		ShaderProgram* quadShader = nullptr;
		std::vector<QuadData> quadBuffer;
		int quadTextureID = -1;
		bool quadsActive = false;
	};

	static Renderer2DData* data = nullptr;







	void Renderer2D::Setup() {
		if (data == nullptr) {
			data = new Renderer2DData();
		}
		else {
			LOG_CORE_CRITICAL("Can't setup Renderer2D: Already initialized!");
		}
	}

	void Renderer2D::Shutdown() {
		if (data != nullptr) {
			delete data;
			data = nullptr;
		}
		else {
			LOG_CORE_CRITICAL("Can't shutdown Renderer2D: Not initialized!");
		}
	}

	void Renderer2D::BeginScene(Scene* scene) {
		CHECK_INIT();
		LOG_CORE_TRACE(__FUNCTION__ "()");

		// Some sanity checks
		if (data->currentScene != nullptr) {
			LOG_CORE_ERROR(__FUNCTION__ "(): Can't begin scene, another scene is still active!");
			return;
		}

		if (scene == nullptr) {
			LOG_CORE_ERROR(__FUNCTION__ "(): Can't load scene: Supplied scene pointer is null!");
			return;
		}

		if (scene->window == nullptr) {
			LOG_CORE_ERROR(__FUNCTION__ "(): Can't load scene: Supplied AllegroWindow pointer is null!");
			return;
		}

		//if (!scene->shaderProgram.IsLoaded()) {
		//	LOG_CORE_ERROR(__FUNCTION__ "(): Can't load scene: ShaderProgram is not loaded!");
		//	return;
		//}

		// Change pointer to the new scene
		data->currentScene = scene;

		// Initialize the canvas for the scene
		al_set_target_backbuffer(data->currentScene->window->allegroDisplayPointer);
	}

	void Renderer2D::EndScene() {
		CHECK_INIT();
		LOG_CORE_TRACE(__FUNCTION__ "()");

		if (data->currentScene == nullptr) {
			LOG_CORE_ERROR(__FUNCTION__ "(): Can't end scene: No scene is currently active!");
			return;
		}

		/*if (data->quadsActive) {
			LOG_CORE_WARN(__FUNCTION__ "(): The Quad buffer is still active! Make sure to call Renderer2D::EndQuads()!");
			EndQuads();
		}

		if (data->linesActive) {
			LOG_CORE_WARN(__FUNCTION__ "(): The Line buffer is still active! Make sure to call Renderer2D::EndLines()!");
			EndLines();
		}

		if (data->circlesActive) {
			LOG_CORE_WARN(__FUNCTION__ "(): The Circle buffer is still active! Make sure to call Renderer2D::EndCircles()!");
			EndCircles();
		}*/

		// Let go of the reference to the scene object
		data->currentScene = nullptr;
	}

	void Renderer2D::EndUnfinishedScene() {
		CHECK_INIT();

		if (data->currentScene != nullptr) {
			LOG_CORE_WARN(__FUNCTION__ "(): The most recent scene is still active, make sure to call Renderer2D::EndScene()!");
			EndScene();
		}
	}

	/*void Renderer2D::BeginQuads(ShaderProgram* shaderProgram, int textureID) {
		CHECK_INIT();
		LOG_CORE_TRACE(__FUNCTION__ "()");

		// Some sanity checks
		if (data->currentScene == nullptr) {
			LOG_CORE_ERROR(__FUNCTION__ "(): No scene is currently active!"); return;
		}

		if (shaderProgram == nullptr) {
			LOG_CORE_ERROR(__FUNCTION__ "(): Supplied shaderProgram reference is nullptr!"); return;
		}

		// Change pointer to the new scene
		data->quadShader = shaderProgram;
		data->quadTextureID = textureID;
		data->quadsActive = true;
		data->quadBuffer.clear();
	}

	void Renderer2D::EndQuads() {
		CHECK_INIT();
		LOG_CORE_TRACE(__FUNCTION__ "(): Rendering " + std::to_string(data->quadBuffer.size()) + " quads!");

		// Some sanity checks
		if (!data->quadShader->IsLoaded()) {
			LOG_CORE_ERROR(__FUNCTION__ "(): Can't render quads: The supplied shader is not loaded!");
			return;
		}

		if (!data->quadsActive) {
			LOG_CORE_ERROR(__FUNCTION__ "(): Quads are not active! Start quads with Renderer2D::BeginQuads()!");
			return;
		}

		// Now finally render the quads
		int64_t numOfQuads = static_cast<int>(data->quadBuffer.size());
		ALLEGRO_VERTEX* vertices = new ALLEGRO_VERTEX[numOfQuads * 4];
		int* indices = new int[numOfQuads * 6];

		for (int64_t i = 0; i < numOfQuads; i++) {
			vertices[i * 4 + 0] = (ALLEGRO_VERTEX)data->quadBuffer[i].v1;
			vertices[i * 4 + 1] = (ALLEGRO_VERTEX)data->quadBuffer[i].v2;
			vertices[i * 4 + 2] = (ALLEGRO_VERTEX)data->quadBuffer[i].v3;
			vertices[i * 4 + 3] = (ALLEGRO_VERTEX)data->quadBuffer[i].v4;
		}

		for (int i = 0; i < numOfQuads; i++) {
			indices[i * 6 + 0] = i * 4 + 0;
			indices[i * 6 + 1] = i * 4 + 1;
			indices[i * 6 + 2] = i * 4 + 2;
			indices[i * 6 + 3] = i * 4 + 0;
			indices[i * 6 + 4] = i * 4 + 2;
			indices[i * 6 + 5] = i * 4 + 3;
		}

		// Load the texture
		data->quadShader->Use();
		ALLEGRO_BITMAP* texture = nullptr;

		// Find the selected texture
		if (data->quadTextureID < data->currentScene->textures.size() && data->quadTextureID >= 0) {
			texture = data->currentScene->textures[data->quadTextureID].GetAllegroBitmap();

			if (texture == nullptr) {
				LOG_WARN(__FUNCTION__ "(): The specified texture is not valid");
			}
		}
		else {
			if (data->quadTextureID != -1) {
				LOG_WARN(__FUNCTION__ "(): Invalid texture ID specified!");
			}
		}

		// Render all loaded quads
		al_draw_indexed_prim(vertices, NULL, texture, indices, numOfQuads * 6, ALLEGRO_PRIM_TRIANGLE_LIST);
		data->quadShader->Release();

		// Clean everything
		data->quadBuffer.clear();
		data->quadsActive = false;
		delete[] vertices;
		delete[] indices;
	}

	void Renderer2D::DrawQuad(const VertexData& v1, const VertexData& v2, const VertexData& v3, const VertexData& v4) {
		CHECK_INIT();
		if (!data->quadsActive) {
			LOG_CORE_ERROR(__FUNCTION__ "(): Quad context is not active! Start a context with Renderer2D::BeginQuads()!");
			return;
		}

		data->quadBuffer.push_back({ v1, v2, v3, v4 });
	}*/

	void Renderer2D::DrawQuad(const VertexData& v1, const VertexData& v2, const VertexData& v3, const VertexData& v4, 
			ShaderProgram* shaderProgram, int textureID) {
		CHECK_INIT();
		LOG_CORE_TRACE(__FUNCTION__ "(): Rendering a quad now!");

		if (!shaderProgram->IsLoaded()) {
			LOG_CORE_ERROR(__FUNCTION__ "(): Can't render quad: The supplied shader is not loaded!");
			return;
		}

		ALLEGRO_VERTEX vertices[4] = { v1, v2, v3, v4 };
		int indices[6] = { 0, 1, 2, 
						   0, 2, 3 };

		// Load the texture
		shaderProgram->Use();
		ALLEGRO_BITMAP* texture = nullptr;

		// Find the selected texture
		if (textureID < data->currentScene->textures.size() && textureID >= 0) {
			texture = data->currentScene->textures[textureID].GetAllegroBitmap();

			if (texture == nullptr) {
				LOG_WARN(__FUNCTION__ "(): The specified texture is not valid");
			}
		}
		else {
			if (textureID != -1) {
				LOG_WARN(__FUNCTION__ "(): Invalid texture ID specified!");
			}
		}

		// Render the quad
		al_draw_indexed_prim(vertices, NULL, texture, indices, 6, ALLEGRO_PRIM_TRIANGLE_LIST);
		data->quadShader->Release();
	}

	/*void Renderer2D::BeginLines() {
		CHECK_INIT();
		LOG_CORE_TRACE(__FUNCTION__ "()");

		// Some sanity checks
		if (data->currentScene == nullptr) {
			LOG_CORE_ERROR(__FUNCTION__ "(): No scene is currently active!"); return;
		}

		if (!data->currentScene->lineShader->IsLoaded()) {
			LOG_CORE_ERROR(__FUNCTION__ "(): The ShaderProgram for Lines is not loaded!"); return;
		}

		// Change pointer to the new scene
		data->linesActive = true;
		data->lineBuffer.clear();
	}

	void Renderer2D::EndLines() {
		CHECK_INIT();
		LOG_CORE_TRACE(__FUNCTION__ "(): Rendering " + std::to_string(data->lineBuffer.size()) + " buffered lines");

		// Some sanity checks
		if (!data->linesActive) {
			LOG_CORE_ERROR(__FUNCTION__ "(): Lines are not active! Start lines with Renderer2D::BeginLines()!");
			return;
		}

		// Now render the lines
		for (LineData& line : data->lineBuffer) {

			glm::vec2 atob = glm::normalize(line.p2 - line.p1);
			glm::vec2 anorm = glm::vec2(atob.y, -atob.x);

			float r = line.thickness / 2;
			BeginQuads(data->currentScene->lineShader);

			data->currentScene->lineShader->Use();
			data->currentScene->lineShader->SetUniformFloat("line_p1", line.p1);
			data->currentScene->lineShader->SetUniformFloat("line_p2", line.p2);
			data->currentScene->lineShader->SetUniformFloat("line_thickness", line.thickness);
			data->currentScene->lineShader->SetUniformFloat("line_falloff", line.falloff);

			VertexData v1 = VertexData(glm::vec3(line.p1 - atob * r + anorm * r, 0), line.color);
			VertexData v2 = VertexData(glm::vec3(line.p1 - atob * r - anorm * r, 0), line.color);
			VertexData v3 = VertexData(glm::vec3(line.p2 + atob * r - anorm * r, 0), line.color);
			VertexData v4 = VertexData(glm::vec3(line.p2 + atob * r + anorm * r, 0), line.color);

			DrawQuad(v1, v2, v3, v4);

			EndQuads();
		}

		LOG_CORE_TRACE(__FUNCTION__ "(): Clearing line buffer");
		data->linesActive = false;
		data->lineBuffer.clear();
	}

	void Renderer2D::DrawLine(const glm::vec2& p1, const glm::vec2& p2, float thickness, const glm::vec4& color, float falloff) {
		CHECK_INIT();
		if (!data->linesActive) {
			LOG_CORE_ERROR(__FUNCTION__ "(): Lines context is not active! Start a context with Renderer2D::BeginLines()!");
			return;
		}

		thickness = max(thickness, 0.f);
		falloff = max(falloff, 0.f);

		data->lineBuffer.push_back({ p1, p2, thickness, falloff, color });
	}*/

	void Renderer2D::DrawLine(const glm::vec2& p1, const glm::vec2& p2, float thickness, const glm::vec4& color, float falloff) {
		CHECK_INIT();
		LOG_CORE_TRACE(__FUNCTION__ "(): Drawing line");

		thickness = max(thickness, 0.f);
		falloff = max(falloff, 0.f);

		glm::vec2 atob = glm::normalize(p2 - p1);
		glm::vec2 anorm = glm::vec2(atob.y, -atob.x);

		float r = thickness / 2;

		data->currentScene->lineShader->Use();
		data->currentScene->lineShader->SetUniformFloat("line_p1", p1);
		data->currentScene->lineShader->SetUniformFloat("line_p2", p2);
		data->currentScene->lineShader->SetUniformFloat("line_thickness", thickness);
		data->currentScene->lineShader->SetUniformFloat("line_falloff", falloff);

		VertexData v1 = VertexData(glm::vec3(p1 - atob * r + anorm * r, 0), color);
		VertexData v2 = VertexData(glm::vec3(p1 - atob * r - anorm * r, 0), color);
		VertexData v3 = VertexData(glm::vec3(p2 + atob * r - anorm * r, 0), color);
		VertexData v4 = VertexData(glm::vec3(p2 + atob * r + anorm * r, 0), color);

		// Line
		if (color.w != 0.f)
			DrawQuad(v1, v2, v3, v4, data->currentScene->lineShader);
		else
			LOG_CORE_TRACE("Line color alpha is 0: Skipping line");
	}

	void Renderer2D::DrawArc(const glm::vec2& center, float radius, float startAngle, float endAngle, float thickness,
		const glm::vec4& color, float falloff) {
		CHECK_INIT();
		LOG_CORE_TRACE(__FUNCTION__ "(): Rendering arc");

		thickness = max(thickness, 0.f);
		falloff = max(falloff, 0.f);
		radius = max(radius, 0.f);
		startAngle = glm::radians(startAngle - 360.f * floor(startAngle / 360.f));
		endAngle = glm::radians(endAngle - 360.f * floor(endAngle / 360.f));

		float margin = thickness / 2.f + falloff;

		glm::vec2 toTop = glm::vec2(0, 1) * radius + glm::vec2(0, margin);
		glm::vec2 toRight = glm::vec2(1, 0) * radius + glm::vec2(margin, 0);

		if (!data->currentScene->arcShader->IsLoaded()) {
			LOG_CORE_ERROR(__FUNCTION__ "(): Can't render circle: Circle shader is not loaded!"
				" Make sure a valid scene is active!");
			return;
		}

		data->currentScene->arcShader->Use();
		data->currentScene->arcShader->SetUniformFloat("arc_center", center);
		data->currentScene->arcShader->SetUniformFloat("arc_radius", radius);
		data->currentScene->arcShader->SetUniformFloat("arc_start_angle", startAngle);
		data->currentScene->arcShader->SetUniformFloat("arc_end_angle", endAngle);
		data->currentScene->arcShader->SetUniformFloat("arc_thickness", thickness);
		data->currentScene->arcShader->SetUniformFloat("arc_falloff", falloff);

		VertexData v1 = VertexData(glm::vec3(center - toRight + toTop, 0), color);
		VertexData v2 = VertexData(glm::vec3(center + toRight + toTop, 0), color);
		VertexData v3 = VertexData(glm::vec3(center + toRight - toTop, 0), color);
		VertexData v4 = VertexData(glm::vec3(center - toRight - toTop, 0), color);

		// Arc
		if (color.w != 0.f)
			DrawQuad(v1, v2, v3, v4, data->currentScene->arcShader);
		else
			LOG_CORE_TRACE("Arc color alpha is 0: Skipping arc");
	}

	/*void Renderer2D::BeginCircles() {
		CHECK_INIT();
		LOG_CORE_TRACE(__FUNCTION__ "()");

		// Some sanity checks
		if (data->currentScene == nullptr) {
			LOG_CORE_ERROR(__FUNCTION__ "(): No scene is currently active!"); return;
		}

		if (!data->currentScene->circleShader->IsLoaded()) {
			LOG_CORE_ERROR(__FUNCTION__ "(): The ShaderProgram for Circles is not loaded!"); return;
		}

		// Change pointer to the new scene
		data->circlesActive = true;
		data->circleBuffer.clear();
	}

	void Renderer2D::EndCircles() {
		CHECK_INIT();
		LOG_CORE_TRACE(__FUNCTION__ "(): Rendering " + std::to_string(data->circleBuffer.size()) + " buffered circles");

		// Some sanity checks
		if (!data->circlesActive) {
			LOG_CORE_ERROR(__FUNCTION__ "(): Circles are not active! Start circles with Renderer2D::BeginCircles()!");
			return;
		}

		// Now render the circles
		for (CircleData& circle : data->circleBuffer) {

			float r = circle.diameter / 2.0;
			glm::vec2 toTop = glm::vec2(0, 1) * r;
			glm::vec2 toRight = glm::vec2(1, 0) * r;

			BeginQuads(data->currentScene->circleShader);

			data->currentScene->circleShader->Use();
			data->currentScene->circleShader->SetUniformFloat("circle_center", circle.center);
			data->currentScene->circleShader->SetUniformFloat("circle_diameter", circle.diameter);
			data->currentScene->circleShader->SetUniformFloat("circle_falloff", circle.falloff);

			VertexData v1 = VertexData(glm::vec3(circle.center - toRight + toTop, 0), circle.color);
			VertexData v2 = VertexData(glm::vec3(circle.center + toRight + toTop, 0), circle.color);
			VertexData v3 = VertexData(glm::vec3(circle.center + toRight - toTop, 0), circle.color);
			VertexData v4 = VertexData(glm::vec3(circle.center - toRight - toTop, 0), circle.color);

			DrawQuad(v1, v2, v3, v4);

			EndQuads();
		}

		LOG_CORE_TRACE(__FUNCTION__ "(): Clearing circle buffer");
		data->circlesActive = false;
		data->circleBuffer.clear();
	}

	void Renderer2D::DrawCircle(const glm::vec2& center, float diameter, const glm::vec4& color, float falloff) {
		CHECK_INIT();

		if (!data->circlesActive) {
			LOG_CORE_ERROR(__FUNCTION__ "(): Circles context is not active! Start a context with Renderer2D::BeginCircles()!");
			return;
		}

		diameter = max(diameter, 0.f);
		falloff = max(falloff, 0.f);

		data->circleBuffer.push_back({ center, diameter, falloff, color });
	}*/

	void Renderer2D::DrawCircle(const glm::vec2& center, float radius, float outlineThickness,
		const glm::vec4& outlineColor, const glm::vec4& fillColor, float falloff) {
		CHECK_INIT();
		LOG_CORE_TRACE(__FUNCTION__ "(): Rendering circle");

		radius = max(radius, 0.f);
		falloff = max(falloff, 0.f);

		glm::vec2 toTop = glm::vec2(0, 1) * radius;
		glm::vec2 toRight = glm::vec2(1, 0) * radius;

		if (!data->currentScene->circleShader->IsLoaded()) {
			LOG_CORE_ERROR(__FUNCTION__ "(): Can't render circle: Circle shader is not loaded!"
				" Make sure a valid scene is active!");
			return;
		}

		data->currentScene->circleShader->Use();
		data->currentScene->circleShader->SetUniformFloat("circle_center", center);
		data->currentScene->circleShader->SetUniformFloat("circle_radius", radius);
		data->currentScene->circleShader->SetUniformFloat("circle_falloff", falloff);

		VertexData v1 = VertexData(glm::vec3(center - toRight + toTop, 0), fillColor);
		VertexData v2 = VertexData(glm::vec3(center + toRight + toTop, 0), fillColor);
		VertexData v3 = VertexData(glm::vec3(center + toRight - toTop, 0), fillColor);
		VertexData v4 = VertexData(glm::vec3(center - toRight - toTop, 0), fillColor);

		// Fill
		if (fillColor.w != 0.f)
			DrawQuad(v1, v2, v3, v4, data->currentScene->circleShader);
		else
			LOG_CORE_TRACE("Circle fillColor alpha is 0: Skipping fill");

		// Outline
		if (outlineColor.w != 0.f)
			DrawArc(center, radius, 0, 360, outlineThickness, outlineColor, falloff);
		else
			LOG_CORE_TRACE("Circle outlineColor alpha is 0: Skipping outline");
	}



	void Renderer2D::DrawRectangle(const glm::vec2& point1, const glm::vec2& point2, float outlineThickness,
			const glm::vec4& outlineColor, const glm::vec4& fillColor, float falloff) {
		CHECK_INIT();
		LOG_CORE_TRACE(__FUNCTION__ "(): Rendering rectangle");

		if (!data->currentScene->rectangleShader->IsLoaded()) {
			LOG_CORE_ERROR(__FUNCTION__ "(): Can't render rectangle: Rectangle shader is not loaded!"
				" Make sure a valid scene is active!");
			return;
		}

		VertexData v1 = VertexData(glm::vec3(point1.x, point1.y, 0), fillColor);
		VertexData v2 = VertexData(glm::vec3(point2.x, point1.y, 0), fillColor);
		VertexData v3 = VertexData(glm::vec3(point2.x, point2.y, 0), fillColor);
		VertexData v4 = VertexData(glm::vec3(point1.x, point2.y, 0), fillColor);

		// Fill
		if (fillColor.w != 0.f)
			DrawQuad(v1, v2, v3, v4, data->currentScene->rectangleShader);
		else
			LOG_CORE_TRACE("Rectangle fillColor alpha is 0: Skipping fill");

		// Outline
		if (outlineColor.w != 0.f) {
			DrawLine({ point1.x, point1.y }, { point2.x, point1.y }, outlineThickness, outlineColor, falloff);
			DrawLine({ point2.x, point1.y }, { point2.x, point2.y }, outlineThickness, outlineColor, falloff);
			DrawLine({ point2.x, point2.y }, { point1.x, point2.y }, outlineThickness, outlineColor, falloff);
			DrawLine({ point1.x, point2.y }, { point1.x, point1.y }, outlineThickness, outlineColor, falloff);
		}
		else {
			LOG_CORE_TRACE("Rectangle outlineColor alpha is 0: Skipping outline");
		}
	}







	// Primitive drawing routines

	void Renderer2D::DrawBackground(const glm::vec4& color) {
		CHECK_INIT();
		al_clear_to_color(ConvertAllegroColor(color));
	}

	void Renderer2D::DrawPrimitiveLine(const glm::vec2& p1, const glm::vec2& p2, float thickness, const glm::vec4& color) {
		CHECK_INIT();
		al_draw_line(p1.x, p1.y, p2.x, p2.y, ConvertAllegroColor(color), thickness);
	}

	ALLEGRO_COLOR Renderer2D::ConvertAllegroColor(const glm::vec4& color) {
		return al_map_rgba(color.r, color.g, color.b, color.a);
	}

}
