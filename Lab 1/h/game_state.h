#pragma once
#include <stdafx.h>

// GameObject Includes
#include <Camera.h>
#include <enemy.h>
#include <dungeon.h>
#include <Bullet.h>
#include <ParticleSystem.h>
#include <MagicLantern.h>
#include <PowerUp.h>
#include <SkyBox.h>

// Managers
#include <LightManager.h>
#include <ParticleSystem.h>
#include <input_manager.h>
#include <bill_board_manager.h>
#include <uniform_data.h>
#include <global_variables.h>

#define GAME_STATE GameState::getSingletonPtr()

class GameState : public Singleton<GameState>
{
private:
    // Uniform Variables
    std::map<std::string, uniformData*> uniformInfo;

    // GL Program shaders
    Program mainProg = { "main_vs.txt", "main_fs.txt" };
    Program postProcessProg = { "post_process_vs.txt", "post_process_fs.txt" };
    Program skyBoxProg = { "sky_box_vs.txt", "sky_box_fs.txt" };

    // Frame Buffers
    Framebuffer fbo{ SCREENWIDTH, SCREENHEIGHT, 2, GL_RGBA32F };
    Framebuffer fbo1{ SCREENWIDTH, SCREENHEIGHT, 1, GL_RGBA32F };

    // Samplers
    Sampler samp;
    Sampler samplerNearest{ Sampler::Type::NEAREST, Sampler::Wrap::CLAMP };

    // Quad to draw to
    FullscreenQuad fsq;

protected:
    // Managers
    InputManager* inputManager;
    LightManager* lightManager;
    BillBoardManager* BB_BulletManager;

    // Skybox
    ImageTextureCube envMap{ "skybox-%d.png" };
    SkyBox sBox{ vec3(0,0,0) };

    // StandAlone GameObjects
    Mesh torchMesh{ "torch.glb" };
    Dungeon dungeon{ vec3(0,0,0) };
    MagicLantern magicLantern{ vec3(-2,0,4) };
    Enemy toothyjaws{ vec3(2.5f, 0.5f, 2.2f) };

    // GameObject Containers
    std::vector<ParticleSystem> explosions;
    std::vector<vec3> torches;
    std::vector <PowerUp> candyCanes{
        PowerUp(vec3(-3, 0, 4)), PowerUp(vec3(5, 0, 10)), 
        PowerUp(vec3(2, 0, 5)), PowerUp(vec3(1, 0, 1)) };

    // Main Camera
    Camera camera{ vec3(0,0.5,0), vec3(0,0.5,1), vec3(0,1,0) };

    // Debug Variables
    bool outputImage = false;

    // Game State Variables
    bool paused = false;

    static void bullet_death_callback(Bullet b, std::vector<ParticleSystem>* exp) { exp->push_back(ParticleSystem(b.get_pos())); };

    static void particle_system_death_callback(ParticleSystem p, std::vector<ParticleSystem>* exp) {};

    void setup_gl_attributes();

    void setup_sample_buffers();

    void setup_frame_buffers();

    void setup_uniform_map();

    void debug_output_scene();

    template<typename T>
    void make_uniform(std::string uniformName, T value, bool setManually = false)
    {
        if (uniformInfo.find(uniformName) == uniformInfo.end())
            uniformInfo[uniformName] = new uniformData(uniformName, &typeid(T), value, setManually);
        else
            throw new std::runtime_error(("Cannot have duplicate uniforms!! " + uniformName).c_str());
    }

public:

    void debug_print_uniforms();

	GameState();

	~GameState();

    Camera& get_camera() { return camera; };

    void set_light_position(int index, vec3 pos, bool positional) { lightManager->setPosition(index, pos, positional); };

    void set_light_color(int index, vec3 color) { lightManager->setColor(index, color); };

    vec3 get_light_color(int index) { return lightManager->getColor(index); };

    void toggle_light(int index) { lightManager->setColor(index, vec3(1.0f) - lightManager->getColor(index)); };

    bool is_paused() { return paused; };

    void set_pause_state(bool state) { paused = state; };

    void set_print_scene() { outputImage = true; };

    bool contains_uniform(std::string uniform_name) { return uniformInfo.count(uniform_name); };

    uniformData* get_uniform(std::string uniform_name);

    template<typename T>
    T get_uniform_data(std::string uniform_name)
    {
        return std::get<T>(get_uniform(uniform_name)->d);
    }

    void set_uniform(std::string uniform_name);

    template<typename T>
    void set_uniform(std::string uniform_name, T newVal)
    {
        std::map<std::string, uniformData*>::iterator uniformEntryPointer = uniformInfo.find(uniform_name);
        if (uniformEntryPointer == uniformInfo.end())
            throw new std::runtime_error(("Unfiorm: " + uniform_name + " Does not exist in uniformInfo!!").c_str());
        uniformEntryPointer->second->set_uniform(newVal);
    };

	void set_uniforms();

	template<typename T, typename F>
    void reap(std::vector<T>& L, const F &callback)
	{
        unsigned i = 0;
        while (i < L.size()) {
            if (L[i].is_dead()) {
                callback(L[i]);
                auto x = L.back();
                L.pop_back();
                if (i < L.size())
                    L[i] = x;
            }
            else {
                i += 1;
            }
        }
	};

    void fire_bullet();

    void update_variables(float elapsed);

	void update(float elapsed);

	void draw();
};