#include "World.h"

World::World( GLFWwindow& window, int windowWidth, int windowHeight ) :
    window(window) {
        
    this->camera = new Camera( 
        windowWidth, 
        windowHeight, 
        glm::vec3( 0.0f, 0.0f, 5.0f ) 
    );

    this->shaderProgram = new Shader( "sprite.vert", "sprite.frag" );
    this->shaderProgram->Activate();
    this->shaderProgram->SetFloatVecUniform3fv( "baseColor", glm::vec3( 1.0f, 1.0f, 1.0f ) );
    
    this->selectShader = new Shader( "sprite.vert", "sprite.frag" );
    this->selectShader->Activate();
    this->selectShader->SetFloatVecUniform3fv( "baseColor", glm::vec3( 1.0f, 0.0f, 0.0f ) );

    this->shader1 = new Shader( "sprite.vert", "sprite.frag" );
    this->shader1->Activate();
    this->shader1->SetFloatVecUniform3fv( "baseColor", glm::vec3( 1.0f, 1.0f, 0.0f ) );
    
    this->shader2 = new Shader( "sprite.vert", "sprite.frag" );
    this->shader2->Activate();
    this->shader2->SetFloatVecUniform3fv( "baseColor", glm::vec3( 0.0f, 1.0f, 1.0f ) );

    this->shader3 = new Shader( "sprite.vert", "sprite.frag" );
    this->shader3->Activate();
    this->shader3->SetFloatVecUniform3fv( "baseColor", glm::vec3( 0.0f, 1.0f, 1.0f ) );
    
    this->primitives = new Primitives();

    this->rectangle = new Rectangle( *this->primitives, 1.4f, 2.5f );
    this->rectangle->pivot = glm::vec3( -2, 1, 0 );

    this->circle = new Circle( 0.1f );
    this->circle->translation = this->rectangle->pivot;

    this->line = new Line( 
        glm::vec3( 3.0f, 3.0f, 0.0f ),  
        glm::vec3( 0.0f, 1.0f, 0.0f ),  
        glm::vec3( 1.0f, 1.0f, 1.0f ) );

    this->ShouldPlayWorld = true;
    std::cout << "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ World Constructed ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
}

void World::Start() {
    //std::cout << "\nWorld Start";
    this->Initiate();
}

void World::Update() {


    // Play the game



    // Update Calls
    this->camera->UpdateMatrix( 60.0f, 0.1f, 100.0f );
    
    angle += 0.01f;
    this->rectangle->Rotate( angle );

    //Utils::showGlmVec3( this->circle->translation );

    // Draw Calls
    this->rectangle->Draw( *this->selectShader, *this->camera );
    this->circle->Draw( *this->selectShader, *this->camera );
    this->line->Draw( *this->shader3, *this->camera );
}

void World::Destroy() {
    std::cout << "\nWorld Destroy\n";
}

void World::Restart() {
    // Reinitialize setup
    this->Initiate();

}

bool World::IsDead() {
    return false;
}

void World::Initiate() {
   
}



World::~World() {
    delete this->camera;
    delete this->shaderProgram;
    delete this->primitives;
}