#include <stdafx.h>
#include <input_manager.h>
#include <game_state.h>
#include <global_variables.h>

void InputManager::check_keys_down(float elapsed)
{
    Camera* mainCamera = &GAME_STATE->get_camera();
    if (keySet.count(SDLK_w))
        mainCamera->strafeNoUpDown(0, 0, 0.001f * elapsed * mainCamera->speedMultiplier);
    if (keySet.count(SDLK_s))
        mainCamera->strafeNoUpDown(0, 0, -0.001f * elapsed * mainCamera->speedMultiplier);
    if (keySet.count(SDLK_a))
        mainCamera->strafeNoUpDown(-0.001f * elapsed * mainCamera->speedMultiplier, 0, 0);
    if (keySet.count(SDLK_d))
        mainCamera->strafeNoUpDown(0.001f * elapsed * mainCamera->speedMultiplier, 0, 0);
    if (keySet.count(SDLK_e))
        mainCamera->turn(-0.001f * elapsed * mainCamera->speedMultiplier);
    if (keySet.count(SDLK_r))
        mainCamera->turn(0.001f * elapsed * mainCamera->speedMultiplier);

    /*/// Move lights around
    if (keySet.count(SDLK_UP))
        GAME_STATE->move_selected_light(0, 0, 0.001 * elapsed * mainCamera->speedMultiplier);
    if (keySet.count(SDLK_DOWN))
        GAME_STATE->move_selected_light(0, 0, -0.001 * elapsed * mainCamera->speedMultiplier);
    if (keySet.count(SDLK_LEFT))
        GAME_STATE->move_selected_light(-0.001 * elapsed * mainCamera->speedMultiplier, 0, 0);
    if (keySet.count(SDLK_RIGHT))
        GAME_STATE->move_selected_light(0.001 * elapsed * mainCamera->speedMultiplier, 0, 0);*/
}

void InputManager::key_down(int k, float elapsed)
{
    keySet.insert(k);
    if (k == SDLK_q)
    {
        SDL_Quit();
        std::exit(0);
    }
    if (k == SDLK_ESCAPE)
    {
        mouseLook = !mouseLook;
        if (mouseLook)
            SDL_SetRelativeMouseMode(SDL_TRUE);
        else
            SDL_SetRelativeMouseMode(SDL_FALSE);
    }

    if (k == SDLK_F1)
        GAME_STATE->set_pause_state(!GAME_STATE->is_paused());

    uniformData* dataToManipulate;
    if (GAME_STATE->contains_uniform("ambientColor"))
    {
        dataToManipulate = GAME_STATE->get_uniform("ambientColor");
        if (k == SDLK_F2 && *dataToManipulate > (float)MIN_AMBIENT_COLOR)
            *dataToManipulate -= .01f;
        if (k == SDLK_F3 && *dataToManipulate < (float)MAX_AMBIENT_COLOR)
            *dataToManipulate += .01f;
    }

    if (k == SDLK_F4)
        GAME_STATE->turn_all_lights(0.1f);
    if (k == SDLK_F5)
        GAME_STATE->turn_selected_light(-0.1f);
    if (k == SDLK_F6)
        GAME_STATE->turn_selected_light(0.1f);
    if (k == SDLK_F7)
        GAME_STATE->cycle_light_bkwrd();
    if (k == SDLK_F8)
        GAME_STATE->cycle_light_fwrd();
    if (k == SDLK_F9)
        GAME_STATE->debup_print_light_info();

    if (GAME_STATE->contains_uniform("blurMultiplier"))
    {
        dataToManipulate = GAME_STATE->get_uniform("blurMultiplier");
        if (k == SDLK_b && *dataToManipulate < (float)MAX_BLUR_MULTIPLIER)
            *dataToManipulate += .1f;
    }
    if (GAME_STATE->contains_uniform("glowMultiplier"))
    {
        dataToManipulate = GAME_STATE->get_uniform("glowMultiplier");
        if (k == SDLK_g && *dataToManipulate < (float)MAX_GLOW_MULTIPLIER)
            *dataToManipulate += .1f;
    }
    if (GAME_STATE->contains_uniform("blurRadius"))
    {
        dataToManipulate = GAME_STATE->get_uniform("blurRadius");
        if (k == SDLK_i && *dataToManipulate < (int)MAX_BLUR_RAD)
            *dataToManipulate += 1;
        if (k == SDLK_u && *dataToManipulate > (int)MIN_BLUR_RAD)
            *dataToManipulate -= 1;
    }
    if (GAME_STATE->contains_uniform("isHDR"))
    {
        dataToManipulate = GAME_STATE->get_uniform("isHDR");
        if (k == SDLK_x && *dataToManipulate == 0)
            *dataToManipulate = 1;
        else if (k == SDLK_x && *dataToManipulate == 1)
            *dataToManipulate = 0;
    }
    if (k == SDLK_p)
        GAME_STATE->set_print_scene();

    if (k >= SDLK_0 && k <= SDLK_9) {
        int idx = k - SDLK_0;
        std::cout << "Toggle light " << idx << "\n";
        GAME_STATE->toggle_light(idx);
    }
    
    if (k == SDLK_LSHIFT) {
        GAME_STATE->get_camera().speedMultiplier = 2.0;
    }
    if (k == SDLK_t)
        GAME_STATE->debug_print_uniforms();
    if (k == SDLK_z)
        GAME_STATE->set_print_scene();
}

void InputManager::key_up(int k, float elapsed)
{
    keySet.erase(k);
    if (k == SDLK_SPACE)
        mouse_button_down(elapsed);
    if (k == SDLK_LSHIFT)
        GAME_STATE->get_camera().speedMultiplier = 1.0;
}

void InputManager::mouse_button_down(float elapsed)
{
    GAME_STATE->fire_bullet();
}

void InputManager::mouse_button_up(float elapsed)
{
}

void InputManager::mouse_motion(SDL_MouseMotionEvent motion, float elapsed)
{
    if (mouseLook)
    {
        float dx = -0.01f * float(motion.xrel);
        float dy = -0.01f * float(motion.yrel);
        GAME_STATE->get_camera().turn(dx);
        GAME_STATE->get_camera().tilt(dy);
    }
}

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::update(float elapsed)
{
    SDL_Event ev;
    while (true)
    {
        if (!SDL_PollEvent(&ev))
            break;
        if (ev.type == SDL_QUIT)
        {
            SDL_Quit();
            std::exit(0);
        }
        else if (ev.type == SDL_KEYDOWN)
            key_down(ev.key.keysym.sym, elapsed);
        else if (ev.type == SDL_KEYUP)
            key_up(ev.key.keysym.sym, elapsed);
        else if (ev.type == SDL_MOUSEBUTTONDOWN)
            mouse_button_down(elapsed);
        else if (ev.type == SDL_MOUSEMOTION)
            mouse_motion(ev.motion, elapsed);
    }
    if(!GAME_STATE->is_paused())
        check_keys_down(elapsed);
}