#include <engine/mesh.h>

Mesh::Mesh()
{	
	vertexBuffer = 0;
	uvBuffer = 0;
	drawMode = drawModeSettings::triangles;
	lineThickness = 3.0f;
}


Mesh::~Mesh()
{
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &uvBuffer);
}

void Mesh::setAsSquare(int width, int height, float uvWidth, float uvHeight)
{
	this->width = width;
	this->height = height;

	std::vector<Vector2> vertices;

	vertices.push_back(Vector2(0.5f * width, -0.5f * height));
	vertices.push_back(Vector2(-0.5f * width, -0.5f * height));
	vertices.push_back(Vector2(-0.5f * width, 0.5f * height));

	vertices.push_back(Vector2(-0.5f * width, 0.5f * height));
	vertices.push_back(Vector2(0.5f * width, 0.5f * height));
	vertices.push_back(Vector2(0.5f * width, -0.5f * height));

	std::vector<Vector2> uvs;
	
	// Two UV coordinates for each vertex.
	uvs.push_back(Vector2(uvWidth, 0.0f));
	uvs.push_back(Vector2(0.0f, 0.0f));
	uvs.push_back(Vector2(0.0f, uvHeight));
	uvs.push_back(Vector2(0.0f, uvHeight));
	uvs.push_back(Vector2(uvWidth, uvHeight));
	uvs.push_back(Vector2(uvWidth, 0.0f));
	numVerts = vertices.size();
	generateBuffers(vertices, uvs);
}

void Mesh::generateBuffers(std::vector<Vector2> vertices, std::vector<Vector2> uvs)
{
	std::vector<glm::vec3> vec3Verts;
	std::vector<glm::vec2> vec2Uvs;
	int numVerts = vertices.size();
	int numUvs = uvs.size();
	for (int i = 0; i < numVerts; i++)
	{
		vec3Verts.push_back(glm::vec3(vertices[i].x, vertices[i].y, 0.0f));
	}

	for (int i = 0; i < numUvs; i++)
	{
		vec2Uvs.push_back(glm::vec2(uvs[i].x, uvs[i].y));
	}

	if (vertexBuffer != 0 && uvBuffer != 0)
	{
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &uvBuffer);
	}

	glGenBuffers(1, &this->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(glm::vec3), &vec3Verts[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, numUvs * sizeof(glm::vec2), &vec2Uvs[0], GL_STATIC_DRAW);

}

void Mesh::setUvOffset(Vector2 offset)
{
	uvOffset = offset;
}

void Mesh::setFromVertices(std::vector<Vector2> vertices)
{
	numVerts = vertices.size();

	std::vector<Vector2> uvs;

	Vector2 min;
	Vector2 max;

	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].x < min.x)
		{
			min.x = vertices[i].x;
		}

		if (vertices[i].y < min.y)
		{
			min.y = vertices[i].y;
		}

		if (vertices[i].x > max.x)
		{
			max.x = vertices[i].x;
		}

		if (vertices[i].y > max.y)
		{
			max.y = vertices[i].y;
		}
	}
	
	this->width = abs(max.x)*2.0f;
	this->height = abs(max.y)*2.0f;
	Vector2 scalar = Vector2(1.0f / (max.x - min.x), 1.0f / (max.y - min.y));

	
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		Vector2 uvPos = Vector2((vertices[i].x - min.x) * scalar.x, (vertices[i].y - min.y) * scalar.y);
		uvs.push_back(uvPos);
	}
	generateBuffers(vertices, uvs);
}

void Mesh::setFromVerticesAndUvs(std::vector<Vector2> vertices, std::vector<Vector2> uvs)
{
	Vector2 min;
	Vector2 max;

	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].x < min.x)
		{
			min.x = vertices[i].x;
		}

		if (vertices[i].y < min.y)
		{
			min.y = vertices[i].y;
		}

		if (vertices[i].x > max.x)
		{
			max.x = vertices[i].x;
		}

		if (vertices[i].y > max.y)
		{
			max.y = vertices[i].y;
		}
	}

	this->width = abs(max.x)*2.0f;
	this->height = abs(max.y)*2.0f;

	numVerts = vertices.size();
	generateBuffers(vertices, uvs);
}

void Mesh::setDrawMode(drawModeSettings settings)
{
	drawMode = settings;
}

void Mesh::setLineThickness(float newThickness)
{
	lineThickness = newThickness;
}

GLuint Mesh::getDrawMode()
{
	return drawMode;
}

