#include "Rectangle.h"

Rectangle::Rectangle( 
    const Primitives& primitives, 
    float width,
    float height ) : 
    primitives( primitives ), 
    width( width ), 
    height( height ) {

    this->matrix 		= glm::mat4( 1.0f );
    this->translation 	= glm::vec3( 0.0f, 0.0f, 0.0f );
    this->rotation 		= glm::quat( 1.0f, 0.0f, 0.0f, 0.0f );
    this->scale 		= glm::vec3( 1.0f, 1.0f, 1.0f );
    this->pivot         = glm::vec3( 0.0f, 0.0f, 0.0f );

    this->scale.x = width / 2;
    this->scale.y = height / 2;
    this->UpdateBounds();
}

void Rectangle::UpdateBounds() {
    this->minBound.x = this->translation.x - this->width / 2;
    this->minBound.y = this->translation.y - this->height / 2;

    this->maxBound.x = this->translation.x + this->width / 2;
    this->maxBound.y = this->translation.y + this->height / 2;
}

void Rectangle::Draw( Shader& shader, Camera& camera ) {
    this->primitives.DrawRectangle( 
        shader, 
        camera, 
        this->matrix, 
        this->translation, 
        this->rotation,
        this->scale,
        this->pivot
    );
}

void Rectangle::Translate( glm::vec3 displacementVec ) {
    this->translation += displacementVec;
    this->UpdateBounds();
}

void Rectangle::MoveTo( glm::vec3 newPosition ) {
    this->translation = newPosition;
    this->UpdateBounds();
}

void Rectangle::Rotate( float angle ) {
    glm::vec3 rotVec( 0, 0, angle );
    //this->Translate( this->pivot );
    this->rotation = glm::quat( rotVec );
    //this->Translate( -this->pivot );
}

