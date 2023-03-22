#include "Circle.h"

Circle::Circle( 
    const Primitives& primitives, 
    float rad ): primitives(primitives) {
    this->translation = this->initialPosition;
    this->radius = rad;
    this->scale.x = rad;
    this->scale.y = rad;
}

void Circle::MoveTo( glm::vec3 position ) {
    this->translation = position;
}

void Circle::Draw( Shader& shader, Camera& camera ) {
    this->primitives.DrawCircle( 
        shader, 
        camera,
        this->matrix, 
        this->translation,
        this->rotation,
        this->scale );
}

void Circle::ApplyForce( glm::vec3 force ) {
    this->velocity += force;
}

void Circle::Update() {
    //this->translation += this->velocity;
}

void Circle::Translate( glm::vec3 translationVec ) {
    this->translation += translationVec;
}

void Circle::ResetPosition() {
    this->MoveTo( glm::vec3( this->translation.x, 0, 0 ) );
    this->velocity = glm::vec3( 0, 0, 0 );
}


bool Circle::CheckCollision( const Rectangle& rect ) {
    glm::vec3 circleDistance;

    circleDistance.x = std::abs( this->translation.x - rect.translation.x );
    circleDistance.y = std::abs( this->translation.y - rect.translation.y );

    if( circleDistance.x > ( rect.width / 2 + this->radius ) ) return false;
    if( circleDistance.y > ( rect.height / 2 + this->radius ) ) return false;

    if( circleDistance.x <= rect.width / 2 ) return true;
    if( circleDistance.y <= rect.height / 2 ) return true;

    float cornerDistanceSquared = 
        std::pow( circleDistance.x - rect.width / 2, 2 ) + 
        std::pow( circleDistance.y - rect.height / 2, 2 );

    return cornerDistanceSquared <= std::pow( this->radius, 2 );
}


