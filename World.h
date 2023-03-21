#ifndef WORLD_CLASS_H
#define WORLD_CLASS_H

//#include <GLFW/glfw3.h>
#include "Circle.h"
#include "Camera.h"
#include "Rectangle.h"
#include "Primitives.h"


class World {
    private:
        Camera* camera;
        Shader* shaderProgram;
        Shader* selectShader;

        Shader* shader1;
        Shader* shader2;
        Shader* shader3;

        Primitives* primitives;
        GLFWwindow& window;

        Rectangle* rectangle;
        Circle* circle1;
        Circle* circle2;
        Line* line;

        glm::vec3 startPoint;
        glm::vec3 endPoint;

        float angle = 0.0f;



    public:
        const float gravity = -0.0025;
        bool ShouldPlayWorld = true;

        World( GLFWwindow& window, int windowWidth, int windowHeight );
        void Start();
        void Update();
        void Destroy();
        bool IsDead();
        void Restart();
        void Initiate();

        virtual ~World();
};


#endif 