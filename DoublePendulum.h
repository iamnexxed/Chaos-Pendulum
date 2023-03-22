#ifndef DOUBLEPENDULUM_CLASS_H
#define DOUBLEPENDULUM_CLASS_H
#include <glm/glm.hpp>
#include "Globals.h"
#include "Line.h"
#include "Circle.h"
#include "Helpers/Mathematics.h"
#include "Helpers/Utils.h"

class DoublePendulum {
    private:
        float _angle1;
        float _angle2;
        float _length1;
        float _length2;
        glm::vec3 _origin;
        glm::vec3 _m1Pos;
        glm::vec3 _m2Pos;

        std::vector<Line*> _lines;
        std::vector<Circle*> _circles;

        void _calculateBobPositions();

    public:
        DoublePendulum( 
            const Primitives& primitives, 
            glm::vec3 o );
        void SetAngle1Deg( float value );
        void SetAngle2Deg( float value );
        void Draw( Shader& shader, Camera& camera );
        void Draw( 
            Shader& shader1,
            Shader& shader2,
            Camera& camera );
};


#endif