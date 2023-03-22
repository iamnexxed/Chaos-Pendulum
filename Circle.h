#ifndef CIRCLE_CLASS_H
#define CIRCLE_CLASS_H

#include "Helpers/Utils.h"
#include "Mesh.h"
#include "Rectangle.h"
#include "Line.h"
#include "Globals.h"

class Circle {
    private:
        const Primitives& primitives;
        glm::vec3 velocity = glm::vec3( 0.0f, 0.0f, 0.0f );
        glm::vec3 initialPosition = glm::vec3(-3.3, 0, 0);
        glm::mat4 matrix = glm::mat4( 1.0f );
        glm::quat rotation = glm::quat( 1.0f, 0.0f, 0.0f, 0.0f );

    public:
        float radius = 1.0f;
        glm::vec3 translation = glm::vec3( 0.0f, 0.0f, 0.0f );
		glm::vec3 scale = glm::vec3( 1.0f, 1.0f, 1.0f );

        Circle( const Primitives& primitives, float rad );
        void MoveTo( glm::vec3 position );
        void Draw( Shader& shader, Camera& camera );
        void ApplyForce( glm::vec3 force );
        void Update();
        void Translate( glm::vec3 translationVec );
        void ResetPosition();
        bool CheckCollision( const Rectangle& rect );
};


#endif 