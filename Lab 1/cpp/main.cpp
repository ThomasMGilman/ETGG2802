
#include <SDL.h>
#include <functional>
#include <cmath>
#include "glfuncs.h"
#include "Program.h"
#include "Buffer.h"
#include "utils.h"
#include "Bullet.h"
#include "Sampler.h"
#include "LightManager.h"
#include "MagicLantern.h"
#include "PowerUp.h"
#include "Globals.h"
#include "setupgl.h"

Globals* globs;


void addPowerUps(vec3 pos)
{
    globs->candyCanes.push_back(PowerUp(pos));
}

void setup(){
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glDepthFunc(GL_LEQUAL);
    //glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//0.2f,0.4f,0.6f,1.0f);
  
    globs = new Globals();

    for (int i = 0; i < 16; ++i)
    {
        if (i == 4)
            globs->samplerNearest.bind(i);
        globs->samp.bind(i);
    }
    globs->samplerNearest.bind(16);
    
    globs->fbo.setAsRenderTarget(false);
    globs->fbo.unsetAsRenderTarget();
    globs->fbo2.setAsRenderTarget(false);
    globs->fbo2.unsetAsRenderTarget();

    globs->prog.use();
    
    globs->lightManager.attenuation = vec3(1,0,0.25);
    
    for(unsigned i=0;i<globs->dungeon.lightPositions.size();++i){
        globs->lightManager.setPosition(i, globs->dungeon.lightPositions[i], 1 );
        std::cout << "LIGHT " << i << ": " << globs->dungeon.lightPositions[i] << "\n";
        globs->lightManager.setColor(i, vec3(1,1,1));
    }

    addPowerUps(vec3(-3, 0, 4));
    addPowerUps(vec3(5, 0, 10));
    addPowerUps(vec3(2, 0, 5));
    addPowerUps(vec3(1, 0, 1));
}


template<typename T, typename F>
void reap(std::vector<T>& L, const F& callback )
{
    unsigned i=0;
    while(i<L.size()){
        if(L[i].isDead()){
            callback(L[i]);
            auto x = L.back();
            L.pop_back();
            if( i < L.size() )
                L[i] = x;
        } else {
            i+=1;
        }
    }
}

void mouseClick()
{
    globs->bullets.push_back(Bullet(globs->camera.eye.xyz(), globs->camera.look.xyz()));
    std::cout << "Current position:" << globs->camera.eye << "\n";
}

void mouselook(float dx, float dy )
{
    globs->camera.turn(dx);
    globs->camera.tilt(dy);
}

void keydown(int k){
    if( k == SDLK_F1 )
        globs->paused = !globs->paused;
    if (k == SDLK_F2 && globs->ambientColor > MIN_AMBIENT_COLOR)
        globs->ambientColor -= .01;
    if (k == SDLK_F3 && globs->ambientColor < MAX_AMBIENT_COLOR)
        globs->ambientColor += .01;
    if (k == SDLK_F4 && globs->shininess > MIN_SHININESS)
        globs->shininess -= .1;
    if (k == SDLK_F5 && globs->shininess < MAX_SHININESS)
        globs->shininess += .1;
    if (k == SDLK_F6 && globs->metallicity > MIN_METALLICITY)
        globs->metallicity -= .1;
    if (k == SDLK_F7 && globs->metallicity < MAX_METALLICITY)
        globs->metallicity += .1;
    if (k == SDLK_F8 && globs->roughness > MIN_ROUGHNESS)
        globs->roughness -= .1;
    if (k == SDLK_F9 )//&& globs->roughness < MAX_ROUGHNESS)
        globs->roughness += .1;
    if (k == SDLK_b && globs->blurMultiplier < MAX_BLUR_MULTIPLIER)
        globs->blurMultiplier += .1;
    else if (globs->blurMultiplier > MIN_BLUR_MULTIPLIER)
        globs->blurMultiplier -= .1;
    if (k == SDLK_g  && globs->glowMultiplier < MAX_GLOW_MULTIPLIER)
        globs->glowMultiplier += .1;
    else if (globs->glowMultiplier > MIN_GLOW_MULTIPLIER)
        globs->glowMultiplier -= .1;
    if (k == SDLK_i && globs->blurRadius < MAX_BLUR_RADIUS)
        globs->blurRadius += 1;
    if (k == SDLK_u && globs->blurRadius > MIN_BLUR_RADIUS)
        globs->blurRadius -= 1;
    if (k == SDLK_p)
        globs->outputImage = true;

    if( k >= SDLK_0 && k <= SDLK_9 ){
        int idx = k - SDLK_0;
        std::cout << "Toggle light " << idx << "\n";
        globs->lightManager.setColor(idx, vec3(1.0f) - globs->lightManager.getColor(idx) );
    }
    if (k == SDLK_LSHIFT) {
        globs->speedMultiplier = 2.0;
    } 
    if (k == SDLK_t)
    {
        std::cout << "AmbientColor: " << globs->ambientColor << " Shininess: " << globs->shininess << 
            "\nRoughness: " << globs->roughness << " Metallicity: " << globs->metallicity <<
            "\nbluryness: " << globs->blurMultiplier << " glowyness: " << globs->glowMultiplier <<
            "\nblurRadius: " << globs->blurRadius <<std::endl;
    }
}

void keyup( int k ){
    if( k == SDLK_SPACE ){
        mouseClick();
    }
    if (k == SDLK_LSHIFT) {
        globs->speedMultiplier = 1.0;
    }
}

            
void update(int elapsed){
    
    float elapsedf=float(elapsed);
    
    auto& keyset = pumpEvents( mouselook, keydown, keyup, mouseClick );

    if( globs->paused )
        return;
        
    if( keyset.count(SDLK_w) )
        globs->camera.strafeNoUpDown(0,0,0.001f * elapsedf * globs->speedMultiplier);
    if( keyset.count( SDLK_s ) )
        globs->camera.strafeNoUpDown(0,0,-0.001f * elapsedf * globs->speedMultiplier);
    if ( keyset.count( SDLK_a ) )
        globs->camera.strafeNoUpDown(-0.001f * elapsedf * globs->speedMultiplier,0,0);
    if ( keyset.count(SDLK_d) )
        globs->camera.strafeNoUpDown( 0.001f * elapsedf * globs->speedMultiplier,0,0);
    if ( keyset.count(SDLK_e) )
        globs->camera.turn(-0.001f * elapsedf * globs->speedMultiplier);
    if (keyset.count(SDLK_r))
        globs->camera.turn(0.001f * elapsedf * globs->speedMultiplier);
        
    globs->magicLantern.update(elapsed);
    globs->sBox.update(elapsed);

    for (auto& cane : globs->candyCanes) 
        cane.update(elapsed);
    
    for(auto& b : globs->bullets )
        b.update(elapsed);
        
    for(auto& ex: globs->explosions )
        ex.update(elapsed);
   
    reap( globs->bullets, [](Bullet& b) { 
        globs->explosions.push_back( ParticleSystem( b.pos ) ); 
    });
    reap(globs->explosions, [](ParticleSystem&){});
   
}

void draw(){
    //////////////////////////////////////////////////////// Set Program Using
    globs->prog.use();

    //////////////////////////////////////////////////////// Set FBO as draw target
    globs->fbo.setAsRenderTarget(true);

    ///////////////////////////////////////////////////////// Set Uniforms
    globs->camera.setUniforms();
    globs->lightManager.setUniforms();
    Program::setUniform("doRadialBlur", globs->doRadialBlur);
    Program::setUniform("glowThreshold", globs->glowThreshold);
    Program::setUniform("doGlow", 0);
    Program::setUniform("focalDistance", globs->focalDistance);
    Program::setUniform("shininess", globs->shininess);
    Program::setUniform("ambientColor", globs->ambientColor);
    Program::setUniform("roughness", globs->roughness);
    Program::setUniform("metallicity", globs->metallicity);
    Program::setUniform("worldMatrix", mat4::identity());

    //////////////////////////////////////////////////////// Bind Skybox
    globs->envMap.bind(4);

    //////////////////////////////////////////////////////// Draw to FBO

    globs->dungeon.draw();
    
    globs->magicLantern.draw();
    
    Program::setUniform("worldMatrix", globs->toothyjawsMatrix );
    globs->toothyjaws.draw();
    
    for(auto& pos : globs->torches ){
        Program::setUniform("worldMatrix", translation(pos) );
        globs->torchMesh.draw();
    }
    
    for (auto& b : globs->bullets)
        b.draw();

    for (auto& x : globs->explosions)
        x.draw();

    Program::setUniform("doGlow", globs->doGlow);
    for (auto& cane : globs->candyCanes)
        cane.draw();
    Program::setUniform("doGlow", 0);

    globs->skyBox_prog.use();
    globs->envMap.bind(0);
    globs->sBox.draw();

    //////////////////////////////////////////////////////// Stop Drawing to FBO and clear Screen
    globs->fbo.unsetAsRenderTarget();

    //Copy FBO to FBO2 and Blur FBO2
    
    globs->fbo.copyTo(globs->fbo2);
    globs->fbo.blur(0, 1, globs->glowRadius, globs->glowMultiplier);
    globs->fbo2.blur(0, 0, globs->blurRadius, globs->blurMultiplier);

    globs->fboGLOW_prog.use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (globs->outputImage)
    {
        globs->fbo.dump("fbo1");
        globs->fbo2.dump("fbo2");
        globs->outputImage = false;
    }

    //////////////////////////////////////////////////////// Use fbo program, bind texture in fbo, draw to quad
    globs->fbo.texture->bind(0);
    globs->fbo.texture->bind(1);
    globs->fbo2.texture->bind(2);
    globs->fbo.depthtexture->bind(16);
    globs->fsq.draw();
}


int main(int , char** )
{
    setupGL(SCREENWIDTH,SCREENHEIGHT);
    setup();
    mainloop(update,draw);
    return 0;
}

