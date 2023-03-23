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
        new Circle( primitives, Globals::MASS1 ) );

    // Lower line index 1
    _lines.push_back( 
        new Line( 
            primitives,
            _m1Pos,
            _m2Pos,
            Globals::ROPE_THICKNESS ) );

    _circles.push_back(
        new Circle( primitives, Globals::MASS2 ) );

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

void DoublePendulum::Update() {
    // Calculate the angular acceleration
    float a = 
        -Globals::GRAVITY * ( 
        2 * Globals::MASS1 + Globals::MASS2 ) *
        std::sin( _angle1 );

    float b = 
        Globals::MASS2 * 
        Globals::GRAVITY * 
        std::sin( _angle1 - 2 * _angle2 );

    float c = 2 * std::sin( _angle1 - _angle2 ) * Globals::MASS2;

    float d = 
        _angularVel2 * _angularVel2 * _length2 +
        _angularVel1 * _angularVel1 * _length1 * 
        std::cos( _angle1 - _angle2 );

    float e = 
        2 * Globals::MASS1 + Globals::MASS2 - 
        Globals::MASS2 * std::cos( 2 * _angle1 - 2 * _angle2 );

    float angularAcc1 = ( a - b - c * d ) / ( _length1 * e );

    a = 2 * std::sin( _angle1 - _angle2 );

    b =  
        _angularVel1 * _angularVel1 * _length1 * 
        ( Globals::MASS1 + Globals::MASS2 );

    c = 
        Globals::GRAVITY * ( 
            Globals::MASS1 + Globals::MASS2 ) *
        std::cos( _angle1 );

    d = 
        _angularVel2 * 
        _angularVel2 * 
        _length2 * 
        Globals::MASS2 * 
        std::cos( _angle1 - _angle2 );

    float angularAcc2 = ( a * ( b + c + d ) ) / ( _length2 * e );
    
    // Add the angular acceleration to the angular velocity
    _angularVel1 += angularAcc1;
    _angularVel2 += angularAcc2;
    _angularVel1 = Mathematics::Clamp( 
        _angularVel1, 
        -Globals::MAX_ANGULAR_VELOCITY,
        Globals::MAX_ANGULAR_VELOCITY );
    _angularVel2 = Mathematics::Clamp( 
        _angularVel2, 
        -Globals::MAX_ANGULAR_VELOCITY,
        Globals::MAX_ANGULAR_VELOCITY );

    // Add anglular velocity to the angle
    _angle1 += _angularVel1;
    _angle2 += _angularVel2;

    _calculateBobPositions();
}

glm::vec3 DoublePendulum::GetBobPosition() {
    return _circles[1]->translation;
}