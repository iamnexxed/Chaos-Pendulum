#include "Line.h"

Line::Line() {
	glm::vec3 defaultVec = glm::vec3(1, 1, 1);
	this->_createGLBuffer(defaultVec, defaultVec, defaultVec);
}

Line::Line(
	const glm::vec3& startPosition, 
	const glm::vec3& endPosition, 
	const glm::vec3& color ) {
	this->_createGLBuffer(startPosition, endPosition, color);
}

Line::Line(
	const Primitives& primitives,
	const glm::vec3& startPosition,
	const glm::vec3& endPosition,
	float thickness ) {
	this->thickness = thickness;
	this->lineRect = new Rectangle( primitives, 0.0f, 0.0f );
	this->line_vertices.resize( 2 );
	this->SetPoints( startPosition, endPosition );
}

void Line::SetStartPosition(glm::vec3 newPosition) {
	this->line_vertices[0].position = newPosition;
	if( this->lineRect == nullptr ) {
		this->vbo->SetVertex(0, line_vertices[0]);
	} else {
		this->_adjustRectangle();
	}
}

void Line::SetEndPosition(glm::vec3 newPosition) {
	this->line_vertices[1].position = newPosition;
	if( this->lineRect == nullptr ) {
		this->vbo->SetVertex(1, line_vertices[1]);
	} else {
		this->_adjustRectangle();
	}
}

glm::vec3 Line::GetStartPosition() {
	return this->line_vertices[0].position;
}


glm::vec3 Line::GetEndPosition() {
	return this->line_vertices[1].position;
}

void Line::SetPoints( glm::vec3 startPosition, glm::vec3 endPosition ) {
	this->SetStartPosition(startPosition);
	this->SetEndPosition(endPosition);
}

// Reference: https://stackoverflow.com/questions/14486291/how-to-draw-line-in-opengl
void Line::Draw(
	Shader& shader, 
	Camera& camera ) {
	if( this->lineRect == nullptr ) {
		shader.Activate();
		this->vao.Bind();
		glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(shader, "camMatrix");

		glm::mat4 trans = glm::mat4(1.0f);
		glm::mat4 rot = glm::mat4(1.0f);
		glm::mat4 sca = glm::mat4(1.0f);
		glm::mat4 modelMatrix = glm::mat4(1.0f);

		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		//glLineWidth( 50.0f );
		// Reference: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDrawArrays.xhtml
		glDrawArrays(GL_LINE_STRIP, 0, 2);
	} else {
		this->lineRect->Draw( shader, camera );
	}
	
}

void Line::_createGLBuffer(	
	const glm::vec3& startPosition,
	const glm::vec3& endPosition,
	const glm::vec3& color ) {
	this->lineRect = nullptr;
	this->vao.Bind();

										//COORDINATES				/			NORMALS				/			COLORS					/		TEXTURE COORDINATES
	this->line_vertices.push_back(Vertex{ startPosition,				glm::vec3(0.0f, 1.0f, 0.0f),				color,				glm::vec2(0.0f, 0.0f) });
	this->line_vertices.push_back(Vertex{ endPosition,				glm::vec3(0.0f, 1.0f, 0.0f),				color,				glm::vec2(0.0f, 1.0f) });


	this->vbo = new VBO(line_vertices, false);


	this->vao.LinkAttribute(*vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	this->vao.LinkAttribute(*vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	this->vao.LinkAttribute(*vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	//vao.LinkAttribute(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
	
	this->vao.Unbind();
	this->vbo->Unbind();
}

void Line::SetColor( glm::vec3 newColor ) {
	this->line_vertices[0].color = newColor;
	this->vbo->SetVertex(0, line_vertices[0]);

	this->line_vertices[1].color = newColor;
	this->vbo->SetVertex(1, line_vertices[1]);
}

float Line::GetLength() {
	glm::vec3 direction = this->line_vertices[1].position - this->line_vertices[0].position;
	return glm::length(direction);
}

glm::vec3 Line::GetMidPoint() {
	return glm::vec3(
		(
			this->line_vertices[0].position.x + 
			this->line_vertices[1].position.x ) / 2.0f,
		(
			this->line_vertices[0].position.y + 
			this->line_vertices[1].position.y ) / 2.0f,
		(
			this->line_vertices[0].position.z + 
			this->line_vertices[1].position.z ) / 2.0f );
}

void Line::_adjustRectangle() {
	if( this->line_vertices.size() == 2 ) {
		this->lineRect->MoveTo( glm::vec3( 0, 0, 0 ) );  
		// Set length as the width of the rectangle
		this->lineRect->SetWidth( this->GetLength() );
		// Set thickness as the height of the rectangle
		this->lineRect->SetHeight( this->thickness );

		
		// Set the rotation of the rectangle according to the orientation of the line
		float rise = this->GetEndPosition().y - this->GetStartPosition().y;
		float run = this->GetEndPosition().x - this->GetStartPosition().x;
		float radians = std::atan2( rise, run );
		this->lineRect->Rotate( radians );

		// Get the midpoint of the line and assign it the center of the rectangle
		this->lineRect->MoveTo( this->GetMidPoint() );  
	}
	
}