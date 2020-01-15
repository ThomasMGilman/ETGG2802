
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

class Globals{
    public:
    
    std::vector<Bullet> bullets;
    std::vector<ParticleSystem> explosions;
    
    Camera camera{ vec3(0,0.5,0), vec3(0,0.5,1), vec3(0,1,0) };
    
    bool paused = false;
    
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

