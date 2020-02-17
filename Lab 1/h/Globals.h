#pragma once

#include <set>
#include <vector>
#include "Camera.h"
#include "LightManager.h"
#include "Bullet.h"
#include "ParticleSystem.h"
#include "MagicLantern.h"
#include "PowerUp.h"
#include "Program.h"
#include "Text.h"
#include "Framebuffer.h"
#include "FullscreenQuad.h"
#include "ImageTextureCube.h"
#include "SkyBox.h"

#define MAX_FOCAL_DISTANCE 200
#define MIN_FOCAL_DISTANCE 0

#define MAX_BLUR_RADIUS 10
#define MIN_BLUR_RADIUS 1

#define MAX_BLUR_MULTIPLIER 10
#define MIN_BLUR_MULTIPLIER 1.1

#define MAX_GLOW_MULTIPLIER 10
#define MIN_GLOW_MULTIPLIER 1.1

#define MAX_AMBIENT_COLOR 1
#define MIN_AMBIENT_COLOR 0.1

#define MAX_SHININESS 16.0
#define MIN_SHININESS 2.0

#define MAX_ROUGHNESS 4.0
#define MIN_ROUGHNESS 0.0

#define MAX_METALLICITY 1.0
#define MIN_METALLICITY 0.0

#define SCREENWIDTH 512
#define SCREENHEIGHT 512

class Globals{
    public:
        Globals()
        {};

    std::vector<Bullet> bullets;
    std::vector<ParticleSystem> explosions;
    
    Camera camera{ vec3(0,0.5,0), vec3(0,0.5,1), vec3(0,1,0) };
    
    bool paused = false;

    float speedMultiplier = 1.0;

    //////////////////////////////////////////////////////// UNIFORM VARIABLES / CONTROLS
    // Graphics variables for materials, need to be set through Uniforms when drawing
    float shininess = 2.0;
    float ambientColor = 0.1;
    float roughness = 64;
    float metallicity = 0;

    // Focal Distance for DOF
    float focalDistance = 100;

    // Blur Variables
    int blurRadius = 4;
    int glowRadius = 9;
    float blurMultiplier = 1;
    float glowMultiplier = 7;
    int doRadialBlur = 0;
    bool outputImage = true;

    // Glow Variables
    int doGlow = 1;
    float glowThreshold = 2;

    LightManager lightManager;
    
    //////////////////////////////////////////////////////// PROGRAM SHADERS
    Program prog{"vs.txt","fs.txt"};
    Program fboDOF_prog{ "fboDOFvs.txt", "fboDOFfs.txt"};
    Program fboGLOW_prog{ "fboGLOWvs.txt", "fboGLOWfs.txt" };
    Program skyBox_prog{"skyBoxvs.txt", "skyBoxfs.txt"};

    
    //////////////////////////////////////////////////////// FRAME BUFFERS
    Framebuffer fbo{ SCREENWIDTH, SCREENHEIGHT, 2, GL_RGBA8 };
    Framebuffer fbo2{ SCREENWIDTH, SCREENHEIGHT, 1, GL_RGBA8 };

    //////////////////////////////////////////////////////// SAMPLERS
    Sampler samp;
    Sampler samplerNearest{ Sampler::Type::NEAREST, Sampler::Wrap::CLAMP };
    
    //////////////////////////////////////////////////////// DRAWABLE OBJECTS
    FullscreenQuad fsq;
    ImageTextureCube envMap{"skybox-%d.png"};

    std::vector <PowerUp> candyCanes;
    std::vector<vec3> torches;
    SkyBox sBox{ vec3(0,0,0) };
    Mesh torchMesh{"torch.glb"};
    Mesh dungeon{"dungeon.glb"};
    MagicLantern magicLantern{vec3(-2,0,4)};
    mat4 toothyjawsMatrix{translation(2.5,0.5,2.2)};
    Mesh toothyjaws{"toothyjaws.glb"};
};

