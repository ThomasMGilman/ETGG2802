
#include <stdafx.h>
#include "ParticleSystem.h"
#include "math3d.h"

ParticleSystem::ParticleSystem(vec3 origin){
    if(ParticleSystem::vao == 0){
        ParticleSystem::prog = new Program("particle_system_vs.txt", "bill_board_gs.txt", "particle_system_fs.txt");
        ParticleSystem::tex = new ImageTexture2DArray("nova.png");
        GLuint tmp[1]; 
        glGenVertexArrays(1,tmp);
        ParticleSystem::vao = tmp[0];
        glBindVertexArray(ParticleSystem::vao);
        
        //vbuff contains the initial velocities of each particle
        std::vector<vec3> L;
        for(int i=0;i<NUM_PARTICLES;++i){
            vec3 v( uniform( -1,1 ), 
                    uniform( -1,1 ),
                    uniform( -1,1 ));
            v = normalize(v);
            float length = uniform(0.0001f,0.001f);
            v = length * v;
            L.push_back(v);
        }
    
        Buffer vbuff(L);
        vbuff.bind(GL_ARRAY_BUFFER);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3,GL_FLOAT,false,3*4,0);
        glBindVertexArray(0);
    }

    this->origin = origin;
    this->totalElapsed = 0;
}

void ParticleSystem::update(int elapsed){    
    this->totalElapsed += elapsed;
}
    
bool ParticleSystem::is_dead(){
    return this->totalElapsed >= ParticleSystem::MAX_LIFE;
}
    
void ParticleSystem::draw(){

    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glDepthMask(false);

    auto curr = Program::current;
    ParticleSystem::prog->use();
    ParticleSystem::tex->bind(0);
    Program::setUniform("halfBoardSize", vec2(.025));
    Program::setUniform("psOrigin",this->origin);
    Program::setUniform("psElapsed",this->totalElapsed);
    Program::setUniform("psMaxLife", ParticleSystem::MAX_LIFE);

    glBindVertexArray(ParticleSystem::vao);
    glDrawArrays(GL_POINTS,0,ParticleSystem::NUM_PARTICLES);
    if(curr)
        curr->use();
    glDepthMask(true);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

GLuint ParticleSystem::vao = 0;
ImageTexture2DArray* ParticleSystem::tex = nullptr;
Program* ParticleSystem::prog = nullptr;