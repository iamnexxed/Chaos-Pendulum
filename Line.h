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
	void _createGLBuffer(	
		const glm::vec3& startPosition,
		const glm::vec3& endPosition,
		const glm::vec3& color = glm::vec3(1, 1, 1) );
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

	void Draw( Shader& shader, Camera& camera );	
	void SetStartPosition( glm::vec3 newPosition );
	glm::vec3 GetStartPosition();
	void SetEndPosition( glm::vec3 newPosition );
	glm::vec3 GetEndPosition();
	void SetPoints( glm::vec3 startPosition,glm::vec3 endPosition );
	void SetColor( glm::vec3 newColor );
	float GetLength();
	glm::vec3 GetMidPoint();
};

#endif // !LINE_CLASS_H
