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
    this->shader1->SetFloatVecUniform3fv( "baseColor", glm::vec3( 65.0f / 255.0f, 53.0f / 255.0f, 67.0f / 255.0f ) );
    
    this->shader2 = new Shader( "sprite.vert", "sprite.frag" );
    this->shader2->Activate();
    this->shader2->SetFloatVecUniform3fv( "baseColor", glm::vec3( 143.0f / 255.0f, 67.0f / 255.0f, 238.0f / 255.0f ) );

    this->shader3 = new Shader( "sprite.vert", "sprite.frag" );
    this->shader3->Activate();
    this->shader3->SetFloatVecUniform3fv( "baseColor", glm::vec3( 240.0f / 255.0f, 235.0f / 255.0f, 141.0f / 255.0f ) );
    
    this->primitives = new Primitives();

    this->pendulum = new DoublePendulum( 
        *this->primitives, 
        glm::vec3( 0, 1.0f, 0 ) );

    this->ShouldPlayWorld = true;

    
    std::cout << "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ World Constructed ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
}

void World::Start() {
    //std::cout << "\nWorld Start";
    this->Initiate();
}

void World::Update() {
    // Handle Input
    if ( glfwGetKey( &this->window, GLFW_KEY_SPACE ) == GLFW_PRESS ) {
            this->shouldUpdate = !this->shouldUpdate;
    } 


    // Update Calls
    this->camera->UpdateMatrix( 60.0f, 0.1f, 100.0f );

    if( this->shouldUpdate ) {
        this->pendulum->Update();
        if( this->trail.size() < Globals::MAX_TRAIL_POINTS ) {
            this->trail.push_back( new Circle( *this->primitives, 0.01f ) );
            int lastIndex = this->trail.size() - 1;
            this->trail[lastIndex]->translation = this->pendulum->GetBobPosition();
        }
    }
    
    


    // Draw Calls
    this->pendulum->Draw( 
        *this->shader1, 
        *this->shader2, 
        *this->camera );
    for( int i = 0; i < this->trail.size(); ++i ) {
        this->trail[i]->Draw( *this->shader3, *this->camera );
    }
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