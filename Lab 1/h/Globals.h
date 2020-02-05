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

#define MAX_AMBIENT_COLOR 1
#define MIN_AMBIENT_COLOR 0.1

#define MAX_SHININESS 16.0
#define MIN_SHININESS 2.0

#define MAX_ROUGHNESS 1.0
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

    // Graphics variables for materials, need to be set through Uniforms when drawing
    float shininess = 2.0;
    float ambientColor = 0.1;
    float roughness = 64;
    float metallicity = 0;

    // Focal Distance for DOF
    float focalDistance = 100;
    
    LightManager lightManager;
    
    Program prog{"vs.txt","fs.txt"};
    Program fboprog{ "fboDOFvs.txt", "fboDOFfs.txt"};
    
    Framebuffer fbo{ SCREENWIDTH, SCREENHEIGHT, 1, GL_RGBA8 };
    Framebuffer fbo2{ SCREENWIDTH, SCREENHEIGHT, 1, GL_RGBA8 };
    Sampler samp;
    Sampler samplerNearest{ Sampler::Type::NEAREST, Sampler::Wrap::CLAMP };
    
    FullscreenQuad fsq;

    std::vector <PowerUp> candyCanes;
    std::vector<vec3> torches;
    Mesh torchMesh{"torch.glb"};
    Mesh dungeon{"dungeon.glb"};
    MagicLantern magicLantern{vec3(-2,0,4)};
    mat4 toothyjawsMatrix{translation(2.5,0.5,2.2)};
    Mesh toothyjaws{"toothyjaws.glb"};
};

