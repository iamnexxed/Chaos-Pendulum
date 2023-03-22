#ifndef CIRCLE_CLASS_H
#define CIRCLE_CLASS_H

#include "Helpers/Utils.h"
#include "Mesh.h"
#include "Rectangle.h"
#include "Line.h"
#include "Globals.h"

class Circle {
    private:
        Mesh *circleMesh;
        glm::vec3 velocity = glm::vec3( 0.0f, 0.0f, 0.0f );
        glm::vec3 initialPosition = glm::vec3(-3.3, 0, 0);

        std::vector<Line> lines;

        void createSprite( float rad );

    public:
        int id = -1;
        float radius = 1.0f;
        glm::vec3 translation = glm::vec3( 0.0f, 0.0f, 0.0f );
		glm::vec3 scale = glm::vec3( 1.0f, 1.0f, 1.0f );

        Circle( float rad );
        void MoveTo( glm::vec3 position );
        void Draw( Shader& shader, Camera& camera );
        void ApplyForce( glm::vec3 force );
        void Update();
        void Translate( glm::vec3 translationVec );
        void ResetPosition();
       
        bool CheckCollision( const Rectangle& rect );
    
        void DrawDebugView( Shader& shader, Camera& camera );
        void DrawDebugView( 
            Shader& shader1, Shader& shader2, Shader& shader3, Camera& camera 
        );
};


#endif 