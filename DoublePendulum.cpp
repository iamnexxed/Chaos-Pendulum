#include "DoublePendulum.h"

DoublePendulum::DoublePendulum( 
    const Primitives& primitives, 
    glm::vec3 o ) {
    _origin = o;
    _angle1 = Globals::DEFAULT_ANGLE1 * DEG2RAD;
    _angle2 = Globals::DEFAULT_ANGLE2 * DEG2RAD;
    _length1 = Globals::LENGTH1;
    _length2 = Globals::LENGTH2;

    // Upper line index 0
    _lines.push_back( 
        new Line( 
            primitives,
            _origin,
            _m1Pos,
            Globals::ROPE_THICKNESS ) );

    _circles.push_back(
        new Circle( Globals::MASS1 ) );

    // Lower line index 1
    _lines.push_back( 
        new Line( 
            primitives,
            _m1Pos,
            _m2Pos,
            Globals::ROPE_THICKNESS ) );

    _circles.push_back(
        new Circle( Globals::MASS2 ) );

    _calculateBobPositions();
}

void DoublePendulum::_calculateBobPositions() {
    glm::vec3 _m1Pos;
    _m1Pos.x = _origin.x + _length1 * std::sin( _angle1 );
    _m1Pos.y = _origin.y - _length1 * std::cos( _angle1 );
    _m1Pos.z = 0.0f;

    glm::vec3 _m2Pos;
    _m2Pos.x =  _m1Pos.x + _length2 * std::sin( _angle2 );
    _m2Pos.y =  _m1Pos.y - _length2 * std::cos( _angle2 );
    _m2Pos.z = 0.0f;

    if( _lines.size() == 2 && _circles.size() == 2 ) {
        _lines[0]->SetEndPosition( _m1Pos );
        _lines[1]->SetStartPosition( _m1Pos );
        _lines[1]->SetEndPosition( _m2Pos );
        _circles[0]->MoveTo( _m1Pos );
        _circles[1]->MoveTo( _m2Pos );
    }
    //Utils::showGlmVec3( _lines[0]->GetEndPosition() );
    //std::cout << "\nL2 : " << _lines[1]->GetLength();
}

 void DoublePendulum::Draw( Shader& shader, Camera& camera ) {
    for( int i = 0; i < _lines.size(); ++i ) {
        _lines[i]->Draw( shader, camera );
        _circles[i]->Draw( shader, camera );
    }
 }

  void DoublePendulum::Draw( 
    Shader& shader1,
    Shader& shader2,
    Camera& camera ) {

    for( int i = 0; i < _circles.size(); ++i ) {
        _circles[i]->Draw( shader2, camera );
    }

    for( int i = 0; i < _lines.size(); ++i ) {
        _lines[i]->Draw( shader1, camera );
    }

}

 void DoublePendulum::SetAngle1Deg( float value ) {
    _angle1 = value * DEG2RAD;
    _calculateBobPositions();
 }

  void DoublePendulum::SetAngle2Deg( float value ) {
    _angle2 = value * DEG2RAD;
    _calculateBobPositions();
 }