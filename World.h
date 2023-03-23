#ifndef WORLD_CLASS_H
#define WORLD_CLASS_H

//#include <GLFW/glfw3.h>
#include "Circle.h"
#include "Camera.h"
#include "Rectangle.h"
#include "Primitives.h"
#include "DoublePendulum.h"

/**
* Notes: 
* 1. The origin is at the center of the window
* 2. Positive X is along right direction
* 3. Negative X is along left direction
* 4. Positive Y is along up direction
* 5. Negative Y is along down direction
*/

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

        DoublePendulum *pendulum;
        std::vector<Circle*> trail;

    public:
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