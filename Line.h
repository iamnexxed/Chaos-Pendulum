#ifndef LINE_CLASS_H
#define LINE_CLASS_H

#include "VAO.h"
#include <glm/glm.hpp>
#include "Camera.h"
#include "Rectangle.h"
#include "Helpers/Mathematics.h"

class Line {
private:
	Rectangle *lineRect;
	void _adjustRectangle();
public:
	std::vector<Vertex> line_vertices;
	float thickness = -1.0f;
	
	
	VAO vao;
	VBO* vbo;
	Line();

	Line(
		const glm::vec3& startPosition,
		const glm::vec3& endPosition,
		const glm::vec3& color );

	Line(
		const Primitives& primitives,
		const glm::vec3& startPosition,
		const glm::vec3& endPosition,
		float thickness );

	void Draw(
		Shader& shader, 
		Camera& camera );

	void createGLBuffer(	
		const glm::vec3& startPosition,
		const glm::vec3& endPosition,
		const glm::vec3& color = glm::vec3(1, 1, 1) );

	
	void setStartPosition( glm::vec3 newPosition );
	glm::vec3 getStartPosition();
	void setEndPosition( glm::vec3 newPosition );
	glm::vec3 getEndPosition();
	void setPoints( glm::vec3 startPosition,glm::vec3 endPosition );
	void setColor( glm::vec3 newColor );
	float getLength();
	glm::vec3 getMidPoint();
};

#endif // !LINE_CLASS_H
