
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

#define MAX_AMBIENT_COLOR 1
#define MIN_AMBIENT_COLOR 0.1

#define MAX_SHININESS 16.0
#define MIN_SHININESS 2.0

class Globals{
    public:
    
    std::vector<Bullet> bullets;
    std::vector<ParticleSystem> explosions;
    
    Camera camera{ vec3(0,0.5,0), vec3(0,0.5,1), vec3(0,1,0) };
    
    bool paused = false;

    float speedMultiplier = 1.0;

    // Graphics variables for materials, need to be set through Uniforms when drawing
    float shininess = 2.0;
    float ambientColor = 0.1;
    
    LightManager lightManager;
    
    Program prog{"vs.txt","fs.txt"};
    
    Sampler samp;
    
    std::vector <PowerUp> candyCanes;
    std::vector<vec3> torches;
    Mesh torchMesh{"torch.glb"};
    Mesh dungeon{"dungeon.glb"};
    MagicLantern magicLantern{vec3(-2,0,4)};
    mat4 toothyjawsMatrix{translation(2.5,0.5,2.2)};
    Mesh toothyjaws{"toothyjaws.glb"};
};

