#include <stdafx.h>
#include <game_state.h>

GameState* GameState::msSingleton = nullptr;

void GameState::setup_gl_attributes()
{
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glEnable(GL_STENCIL_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void GameState::setup_sample_buffers()
{
    for (int i = 0; i < 16; i++)
    {
        if (i == 4)
            samplerNearest.bind(i);
        samp.bind(i);
    }
    samplerNearest.bind(16);
}

void GameState::setup_frame_buffers()
{
    fbo.setAsRenderTarget(false);
    fbo.unsetAsRenderTarget();
    fbo1.setAsRenderTarget(false);
    fbo1.unsetAsRenderTarget();
}

void GameState::setup_uniform_map()
{
    make_uniform("shininess", 2.0f);
    make_uniform("ambientColor", 0.09f);
    make_uniform("roughness", 64.0f);
    make_uniform("metallicity", 0.0f);
    make_uniform("gamma", 2.2f);
    make_uniform("exposure", 2.0f);
    make_uniform("focalDistance", 1.0f);
    make_uniform("isHDR", 0);
    make_uniform("focalSmoothStepMin", 0.0f);
    make_uniform("focalSmoothStepMax", 10.0f);
    make_uniform("doRadialBlur", 0);
    make_uniform("blurRadius", 4, true);
    make_uniform("blurMultiplier", 2.0f, true);
    make_uniform("doGlow", 0);
    make_uniform("glowRadius", 9, true);
    make_uniform("glowMultiplier", 7.0f, true);
    make_uniform("glowThreshold", 2.0f);
    make_uniform("toneMinValue", 0.5f);
    make_uniform("toneMaxValue", 5.0f);
    make_uniform("a_lum_val", 0.2f);
    make_uniform("lum_white", 2.0f);
}

void GameState::debug_output_scene()
{
    fbo.dump("fbo0");
    fbo1.dump("fbo1");
    this->outputImage = false;
}

void GameState::debug_print_uniforms()
{
    std::map<std::string, uniformData*>::iterator uniformEntryPointer = uniformInfo.begin();
    std::cout << "Uniforms:";
    while (uniformEntryPointer != uniformInfo.end())
    {
        std::cout << "\n\t";
        uniformEntryPointer->second->debug_print();
        uniformEntryPointer++;
    }
    std::cout << std::endl;
}

GameState::GameState()
{
    setup_gl_attributes();
    setup_sample_buffers();
    setup_frame_buffers();
    setup_uniform_map();
    mainProg.use();

    inputManager = new InputManager();
    lightManager = new LightManager();
    lightManager->attenuation = vec3(1, 0, 0.25);

    // Initialize Bullet Class to get mesh and create BillBoard_BulletManager;
    Bullet b_init = Bullet(vec3(0), vec3(0));   
    BB_BulletManager = new BillBoardManager(b_init.get_diffuse_texture(), vec3(1));//b_init.get_scale());

    for (unsigned i = 0; i < dungeon.get_num_light_positions(); i++)
    {
        set_light_position(i, dungeon.get_light_position(i), true);
        std::cout << "LIGHT " << i << ": " << dungeon.get_light_position(i) << "\n";
        set_light_color(i, vec3(1, 1, 1));
    }
}

GameState::~GameState()
{
    delete(inputManager);
    delete(lightManager);
    delete(BB_BulletManager);
    std::map<std::string, uniformData*>::iterator ud;
    while (ud != uniformInfo.end())
    {
        delete(ud->second);
        ud++;
    }
}

uniformData* GameState::get_uniform(std::string uniform_name)
{
    std::map<std::string, uniformData*>::iterator uniformEntryPointer = uniformInfo.find(uniform_name);
    if (uniformEntryPointer == uniformInfo.end())
        throw new std::runtime_error(("Uniform specified does not exist!! uniform: "+uniform_name).c_str());
    return uniformEntryPointer->second;
}

void GameState::set_uniform(std::string uniform_name)
{
    std::map<std::string, uniformData*>::iterator uniformEntryPointer = uniformInfo.find(uniform_name);
    if (uniformEntryPointer == uniformInfo.end())
        throw new std::runtime_error(("Unfiorm: " + uniform_name + " Does not exist in uniformInfo!!").c_str());
    uniformEntryPointer->second->set_uniform();
}

void GameState::set_uniforms()
{
    camera.setUniforms();
    lightManager->setUniforms();
    std::map<std::string, uniformData*>::iterator uniformEntryPointer = uniformInfo.begin();
    while (uniformEntryPointer != uniformInfo.end())
    { 
        if(!(uniformEntryPointer->second->setManually))
            uniformEntryPointer->second->set_uniform();
        uniformEntryPointer++;
    }
    Program::setUniform("worldMatrix", mat4::identity());
}

void GameState::fire_bullet()
{
    Camera* cam = &(get_camera());
    BB_BulletManager->add(cam->eye.xyz(), cam->look.xyz() * BULLET_SPEED_MODIFIER);
}

void GameState::update_variables(float elapsed)
{
    uniformData* dataToManipulate;
    dataToManipulate = get_uniform("blurMultiplier");
    if (*dataToManipulate > (float)MAX_BLUR_MULTIPLIER)
        *dataToManipulate += .1f;

    dataToManipulate = get_uniform("glowMultiplier");
    if (*dataToManipulate > (float)MIN_GLOW_MULTIPLIER)
        *dataToManipulate -= .1f;
}

void GameState::update(float elapsed)
{
    inputManager->update(elapsed);
    if (!paused)
    {
        update_variables(elapsed);
        magicLantern.update(elapsed);
        sBox.update(elapsed);

        for (auto& cane : candyCanes)
            cane.update(elapsed);

        BB_BulletManager->update(elapsed, [this](vec3 p) {
            this->explosions.push_back(ParticleSystem(p));
        });

        for (auto& explosion : explosions)
            explosion.update(elapsed);

        reap(explosions, [](ParticleSystem&) {});
    }
}

void GameState::draw()
{
    mainProg.use();                   // Set main Program Shader
    fbo.setAsRenderTarget(true);      // Set FBO as draw target
    set_uniforms();                   // Set Uniforms
    envMap.bind(4);                   // Bind SkyBox

    dungeon.draw();
    magicLantern.draw();
    toothyjaws.draw();

    for (auto& pos : torches)
    {
        Program::setUniform("worldMatrix", translation(pos));
        torchMesh.draw();
    }

    BB_BulletManager->draw();

    for (auto& x : explosions)
        x.draw();

    set_uniform<int>("doGlow", 1);
    for (auto& cane : candyCanes)
        cane.draw();
    set_uniform<int>("doGlow", 0);

    envMap.bind(0);
    skyBoxProg.use();               // Set SkyBox Program Shader      
    sBox.draw();                    // draw SkyBox Box

    fbo.unsetAsRenderTarget();      // Stop drawing to FBO
    fbo.copyTo(fbo1);
    fbo.blur(0, 1, get_uniform_data<int>("glowRadius"), get_uniform_data<float>("glowMultiplier"), get_uniform_data<int>("isHDR"));
    fbo1.blur(0, 0, get_uniform_data<int>("blurRadius"), get_uniform_data<float>("blurMultiplier"), get_uniform_data<int>("isHDR"));

    if (outputImage)
        debug_output_scene();

    postProcessProg.use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    fbo.texture->bind(0);           // bind texture to Image2DArray 0
    fbo.texture->bind(1);           // bind texture to Image2DArray 1
    fbo1.texture->bind(2);          // bind texture to Image2DArray 2
    fbo.depthtexture->bind(16);     // bind texture to Image2DArray 16
    fsq.draw();                     // draw scene to screen
}
