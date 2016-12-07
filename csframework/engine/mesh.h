#pragma once

#include <GL/glew.h>
#include <engine/vector2.h>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>
class Mesh
{
public:
	Mesh();

	virtual ~Mesh();

	enum drawModeSettings {
		triangles = GL_TRIANGLES,
		polygons = GL_POLYGON,
		lines = GL_LINES
	};
	//How are the vertices of the mesh set up?

	GLuint getVertexBuffer() { return vertexBuffer; }
	GLuint getUvBuffer() { return uvBuffer; }
	
	int getWidth() { return width; }
	int getHeight() { return height; }

	int getTextureWidth() { return textureWidth; }
	int getTextureHeight() { return textureHeight; }
	int getNumVerts() { return numVerts; }

	Vector2 getUvOffset() { return  uvOffset; }

	void setAsSquare(int width, int height, float uvWidth, float uvHeight);
	void setUvOffset(Vector2 offset);
	void setFromVertices(std::vector<glm::vec3> vertices);
	void setFromVerticesAndUvs(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs);
	void setDrawMode(drawModeSettings settings);

	GLuint getDrawMode();

private:
	void generateBuffers(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs);

	GLuint vertexBuffer;
	GLuint uvBuffer;

	int width;
	int height;

	int textureWidth;
	int textureHeight;

	Vector2 uvOffset;
	
	int numVerts;

	drawModeSettings drawMode;
};

