#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <engine/vector2.h>

class Mesh
{
public:
	Mesh();

	virtual ~Mesh();

	enum drawModeSettings { //How are the vertices of the mesh set up?
		triangles = GL_TRIANGLES,
		polygons = GL_POLYGON,
		lines = GL_LINES
	};
	

	void setAsSquare(int width, int height, float uvWidth, float uvHeight);
	void setUvOffset(Vector2 offset);
	void setFromVertices(std::vector<Vector2> vertices);
	void setFromVerticesAndUvs(std::vector<Vector2> vertices, std::vector<Vector2> uvs);
	void setDrawMode(drawModeSettings settings);
	void setLineThickness(float newThickness);

	float getLineThickness() { return lineThickness; }
	GLuint getVertexBuffer() { return vertexBuffer; }
	GLuint getUvBuffer() { return uvBuffer; }
	
	int getWidth() { return width; }
	int getHeight() { return height; }

	int getTextureWidth() { return textureWidth; }
	int getTextureHeight() { return textureHeight; }
	int getNumVerts() { return numVerts; }

	Vector2 getUvOffset() { return  uvOffset; }

	GLuint getDrawMode();

private:
	void generateBuffers(std::vector<Vector2> vertices, std::vector<Vector2> uvs);

	GLuint vertexBuffer;
	GLuint uvBuffer;

	int width;
	int height;

	int textureWidth;
	int textureHeight;

	float lineThickness;

	Vector2 uvOffset;
	
	int numVerts;

	drawModeSettings drawMode;
};

