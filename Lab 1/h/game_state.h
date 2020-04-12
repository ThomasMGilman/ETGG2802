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
#include <torch.h>
#include <SkyBox.h>

// Managers
#include <LightManager.h>
#include <ParticleSystem.h>
#include <input_manager.h>
#include <bill_board_manager.h>
#include <uniform_data.h>
#include <global_variables.h>

#define GAME_STATE GameState::getSingletonPtr()

float get_random_float(float max = 1.0f, float min = 0.0f);

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
    Framebuffer fbo{ SCREENWIDTH, SCREENHEIGHT, 2,  GL_RGBA32F };
    Framebuffer fbo1{ SCREENWIDTH, SCREENHEIGHT, 1, GL_RGBA32F };
    Framebuffer shadowBuffer{ SCREENWIDTH, SCREENHEIGHT, 1, GL_R32F };

    // Samplers
    Sampler samp;
    Sampler samplerNearest{ Sampler::Type::NEAREST, Sampler::Wrap::CLAMP };

    // Quad to draw to
    FullscreenQuad fsq;

    // Permutation and Gradient tables for noise
    DataTexture2DArray* permutationTex, *gradientTex;

protected:
    // Managers
    InputManager* inputManager;
    LightManager* lightManager;
    BillBoardManager* BB_BulletManager;
    BillBoardManager* BB_TorchManager;

    // Skybox
    ImageTextureCube envMap{ "skybox-%d.png" };
    SkyBox sBox{ vec3(0,0,0) };

    // StandAlone GameObjects
    Dungeon dungeon{ vec3(0,0,0) };
    MagicLantern magicLantern{ vec3(-2,0,4) };
    Enemy toothyjaws{ vec3(2.5f, 0.5f, 2.2f) };

    // GameObject Containers
    std::vector<ParticleSystem> explosions;
    std::vector<Torch> torches;
    std::vector <PowerUp> candyCanes{
        PowerUp(vec3(-3, 0, 4)), PowerUp(vec3(5, 0, 10)), 
        PowerUp(vec3(2, 0, 5)), PowerUp(vec3(1, 0, 1)) };

    //FOV 90, fade 60
    std::vector <vec3> lightDirs{
        vec3(0, 0.5, 1),            // 0
        vec3(1, 0.5, 0),            // 1
        vec3(-1, 0.5, 0),           // 2
        vec3(-1, 0.5, 0),           // 3
        vec3(1, 0.5, 0),            // 4
        vec3(0, 0.5, -1),           // 5
        vec3(0, 0.5, 1),            // 6
        vec3(1, 0.5, 0)             // 7
    };

    // Main Camera
    Camera* camera;// { vec3(0, 0.5, 0), vec3(0, 0.5, 1), vec3(0, 1, 0) };
    
    // Debug Variables
    bool outputImage = false;

    // Game State Variables
    bool paused = false;

    // light selected
    unsigned lightSelected = 0;

    static void bullet_death_callback(Bullet b, std::vector<ParticleSystem>* exp) { exp->push_back(ParticleSystem(b.get_pos())); };

    static void particle_system_death_callback(ParticleSystem p, std::vector<ParticleSystem>* exp) {};

    void setup_gl_attributes();

    void setup_sample_buffers();

    void setup_frame_buffers();

    void setup_uniform_map();

    void setup_noise_tables();

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

    /// Print all the uniforms stored by this game state
    void debug_print_uniforms();

    /// Print all basic light info
    void debup_print_light_info() { lightManager->print_light_info(); };

	GameState();

	~GameState();

    /// retrieve the games main camera
    Camera& get_camera() { return *camera; };

    /// Turns the current selected light by the angle amount given
    void turn_selected_light(float amt) { lightManager->turn(lightSelected, amt); };

    /// Turns all lights by the angle amount given
    void turn_all_lights(float amt) { for (int i = 0; i < LightManager::MAX_LIGHTS; i++) lightManager->turn(i, amt); };

    /// cycles and selects the next light to the right in the list
    void cycle_light_fwrd() { this->lightSelected++; if (this->lightSelected >= LightManager::MAX_LIGHTS) this->lightSelected = 0; };

    /// cycles and selects the next light to the left in the list
    void cycle_light_bkwrd() { this->lightSelected--; if (this->lightSelected < 0) this->lightSelected = LightManager::MAX_LIGHTS - 1; };

    /// Move the position of the light
    void move_selected_light(float dr, float du, float dl) { this->lightManager->strafeNoUpDown(lightSelected, dr, du, dl); };

    /// sets the specified lights position
    void set_light_position(int index, vec3 pos, bool positional) { lightManager->setPosition(index, pos, positional); };
    
    /// sets the specified lights color
    void set_light_color(int index, vec3 color) { lightManager->setColor(index, color); };

    /// gets the specified lights color
    vec3 get_light_color(int index) { return lightManager->getColor(index); };

    /// turn the specified light on/off
    void toggle_light(int index) { lightManager->setColor(index, vec3(1.0f) - lightManager->getColor(index)); };

    /// check if game is paused
    bool is_paused() { return paused; };

    /// set game paused/playing
    void set_pause_state(bool state) { paused = state; };

    /// debug scene by telling the game to output the framebuffer images to the project file
    void set_print_scene() { outputImage = true; };

    /// check if uniform specified exists
    bool contains_uniform(std::string uniform_name) { return uniformInfo.count(uniform_name); };

    /// get pointer to the uniform specified
    uniformData* get_uniform(std::string uniform_name);

    template<typename T>
    T get_uniform_data(std::string uniform_name)
    {
        return std::get<T>(get_uniform(uniform_name)->d);
    }

    /// Sets the specified uniform 
    void set_uniform(std::string uniform_name);

    /// Sets the specified uniform with the new value and type
    template<typename T>
    void set_uniform(std::string uniform_name, T newVal)
    {
        std::map<std::string, uniformData*>::iterator uniformEntryPointer = uniformInfo.find(uniform_name);
        if (uniformEntryPointer == uniformInfo.end())
            throw new std::runtime_error(("Unfiorm: " + uniform_name + " Does not exist in uniformInfo!!").c_str());
        uniformEntryPointer->second->set_uniform(newVal);
    };

    /// Sets all uniforms
	void set_uniforms();

    /// collect dead gameobjects and delete them
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

    /// fire and add bullet to exsisting bullets in game
    void fire_bullet();

    /// update all variables and uniform variables that adjust over time
    void update_variables(float elapsed);

    /// update all gameobjects
	void update(float elapsed);

    /// draw the objects in the scene, either to shadowbuffer or to framebuffer
    void draw_objects(bool shadowPass = false);

    /// draw the scene
	void draw();
};