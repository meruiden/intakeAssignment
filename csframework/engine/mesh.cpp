#include <engine/mesh.h>

Mesh::Mesh()
{	
	vertexBuffer = 0;
	uvBuffer = 0;
	drawMode = drawModeSettings::triangles;
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

	std::vector<glm::vec3> vertices;

	vertices.push_back(glm::vec3(0.5f * width, -0.5f * height, 0.0f));
	vertices.push_back(glm::vec3(-0.5f * width, -0.5f * height, 0.0f));
	vertices.push_back(glm::vec3(-0.5f * width, 0.5f * height, 0.0f));

	vertices.push_back(glm::vec3(-0.5f * width, 0.5f * height, 0.0f));
	vertices.push_back(glm::vec3(0.5f * width, 0.5f * height, 0.0f));
	vertices.push_back(glm::vec3(0.5f * width, -0.5f * height, 0.0f));

	std::vector<glm::vec2> uvs;
	
	// Two UV coordinates for each vertex.
	uvs.push_back(glm::vec2(uvWidth, 0.0f));
	uvs.push_back(glm::vec2(0.0f, 0.0f));
	uvs.push_back(glm::vec2(0.0f, uvHeight));
	uvs.push_back(glm::vec2(0.0f, uvHeight));
	uvs.push_back(glm::vec2(uvWidth, uvHeight));
	uvs.push_back(glm::vec2(uvWidth, 0.0f));
	numVerts = vertices.size();
	generateBuffers(vertices, uvs);
}

void Mesh::generateBuffers(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs)
{

	glGenBuffers(1, &this->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec3), &uvs[0], GL_STATIC_DRAW);

}

void Mesh::setUvOffset(Vector2 offset)
{
	uvOffset = offset;
}

void Mesh::setFromVertices(std::vector<glm::vec3> vertices)
{
	numVerts = vertices.size();

	std::vector<glm::vec2> uvs;

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
	
	Vector2 scalar = Vector2(1.0f / (max.x - min.x), 1.0f / (max.y - min.y));

	
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		glm::vec2 uvPos = glm::vec2((vertices[i].x - min.x) * scalar.x, (vertices[i].y - min.y) * scalar.y);
		uvs.push_back(uvPos);
	}
	generateBuffers(vertices, uvs);
}

void Mesh::setFromVerticesAndUvs(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs)
{
	numVerts = vertices.size();
	generateBuffers(vertices, uvs);
}

void Mesh::setDrawMode(drawModeSettings settings)
{
	drawMode = settings;
}

GLuint Mesh::getDrawMode()
{
	return drawMode;
}
