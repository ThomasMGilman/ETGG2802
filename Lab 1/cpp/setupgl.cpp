
#include <SDL.h>
#include "glfuncs.h"
#include <iostream>
#include "setupgl.h"
#include "Text.h"


namespace {
    
    std::set<int> keyset;
    bool mouseLook=false;
    SDL_Window* win;


    //source, msgtype, msgid, severity, length, message, param
    void APIENTRY debugCallback( GLenum source, GLenum msgType, GLuint msgId,
        GLenum severity, GLsizei , const GLchar* message, const void* )
    {
        std::cout << msgId << " : " ;
        switch(msgType){
            case GL_DEBUG_TYPE_ERROR:                   std::cout << "Error: "; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:     std::cout << "Deprecated: "; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:      std::cout << "Undefined behavior: "; break;
            case GL_DEBUG_TYPE_PERFORMANCE:             std::cout << "Performance: "; break;
            case GL_DEBUG_TYPE_PORTABILITY:             std::cout << "Portability: "; break;
            default:
                break;
        }
                
        std::cout << message << "\n";
        
        if(severity == GL_DEBUG_SEVERITY_HIGH && source != GL_DEBUG_SOURCE_SHADER_COMPILER ){
            throw std::runtime_error("ERROR!");
        }
    }

    unsigned winwidth, winheight;
};

    
void setupGL(int winw, int winh)
{
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    //Mix_Init(MIX_INIT_OGG|MIX_INIT_MP3)
    //Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,1,4096)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,4);
    
    win = SDL_CreateWindow( "ETGG",20,20, winw,winh, SDL_WINDOW_OPENGL);
    if(!win){
        throw std::runtime_error("Could not create window");
    }

    auto rc = SDL_GL_CreateContext(win);
    if(!rc ){
        throw std::runtime_error("Cannot create GL context");
    }
        
    glDebugMessageCallback( debugCallback, nullptr );
    
    //Source, type, severity, count, ids, enabled
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE,
        0, nullptr, true );
        
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_DEBUG_OUTPUT);
    
    SDL_SetRelativeMouseMode(SDL_TRUE);
    mouseLook = true;
  
    winwidth = (unsigned) winw;
    winheight = (unsigned) winh;
}


const std::set<int>& pumpEvents( MouseLookCallback mlcb, KeyCallback down, KeyCallback up, MouseDownCallback click )
{
    SDL_Event ev;
    while(true){
        if(!SDL_PollEvent(&ev))
            break;
        if( ev.type == SDL_QUIT ){
            SDL_Quit();
            std::exit(0);
        }
        else if( ev.type == SDL_KEYDOWN ){
            auto k = ev.key.keysym.sym;
            keyset.insert(k);
            if( k == SDLK_q ){
                SDL_Quit(); 
                std::exit(0);
            }
            if(k == SDLK_ESCAPE ){
                mouseLook = !mouseLook;
                if( mouseLook )
                    SDL_SetRelativeMouseMode(SDL_TRUE);
                else
                    SDL_SetRelativeMouseMode(SDL_FALSE);
            }
            down(k);
        } 
        else if( ev.type == SDL_KEYUP ){
            auto k = ev.key.keysym.sym;
            keyset.erase(k);
            up(k);
        }
        else if( ev.type == SDL_MOUSEBUTTONDOWN ){
            click();
        }
        else if( ev.type == SDL_MOUSEMOTION ){
            if(mouseLook){
                mlcb( -0.01f*float(ev.motion.xrel),  -0.01f*float(ev.motion.yrel) );
            }
        }
    }
    
    return keyset;
}

namespace {
    uint64_t TICKS_PER_SECOND;
    
    unsigned ticksToMilliseconds(uint64_t elapsedTicks)
    {
        return unsigned(1000 * elapsedTicks / TICKS_PER_SECOND);
    }
    uint64_t millisecondsToTicks(unsigned milliseconds)
    {
        return uint64_t( milliseconds * TICKS_PER_SECOND / 1000 ); 
    }
    
    std::string format(float num){
        std::string sign;
        if( num < 0.0f ){
        sign="-";
        num=-num;
        }
        
        if( num >= 99.5f ){
        return sign + std::to_string(unsigned(num+0.5));
        }
        
        if( num >= 9.95 ){
        unsigned ipart = unsigned(num);
        num -= float(ipart);
        unsigned fpart = unsigned(num*10+0.5);
        if( fpart == 10 ){
            fpart=0;
            ipart++;
        }
        return sign + std::to_string(ipart) + "." + std::to_string( fpart );
        }
        
        unsigned ipart = unsigned(num);
        num -= float(ipart);
        unsigned fpart = unsigned(num*100+0.5);
        if( fpart == 100 ){
        fpart=0;
        ipart++;
        }
        
        return 	sign + 
            std::to_string(ipart) + 
            "." + 
            ( (fpart < 10) ? "0" : "" ) +
            std::to_string( fpart );
    }
};


void mainloop(UpdateFunc update, DrawFunc draw)
{
    const int DESIRED_FRAMES_PER_SEC = 60;
    const float DESIRED_SEC_PER_FRAME = 1.0f/DESIRED_FRAMES_PER_SEC;
    const int DESIRED_MSEC_PER_FRAME = int(DESIRED_SEC_PER_FRAME * 1000);
    TICKS_PER_SECOND = SDL_GetPerformanceFrequency();
    int UPDATE_QUANTUM_MSEC = 5;
    Text* fpsText=nullptr;
    
    auto lastTicks = SDL_GetPerformanceCounter();
    int accumElapsedMsec = 0;
    auto fpsStartTime = lastTicks;
    auto fpsInterval = millisecondsToTicks(2000);
    auto nextFpsReport = fpsStartTime + fpsInterval;
    unsigned frameCount = 0;
    
    while(true){
        auto nowTicks = SDL_GetPerformanceCounter();
        auto elapsedTicks = nowTicks - lastTicks;
        lastTicks = nowTicks;
        auto elapsedMsec = ticksToMilliseconds(elapsedTicks);
        accumElapsedMsec += elapsedMsec;
        while( accumElapsedMsec >= UPDATE_QUANTUM_MSEC ){
            update(UPDATE_QUANTUM_MSEC);
            accumElapsedMsec -= UPDATE_QUANTUM_MSEC;
        }
        draw();
        if( fpsText ){
            fpsText->draw(winwidth,winheight);
        }
        SDL_GL_SwapWindow(win);
        auto endTicks = SDL_GetPerformanceCounter();
        ++frameCount;
        if( endTicks >= nextFpsReport ){
            float fps = (float)(frameCount * 1000.0 / ticksToMilliseconds( endTicks - fpsStartTime ));
            if( fpsText == nullptr ){
                fpsText = new Text(12, vec2(0,0), "");
            }
            fpsText->setText( "FPS: "+format(fps) );
            nextFpsReport = endTicks + fpsInterval;
            fpsStartTime = endTicks;
            frameCount = 0;
        }
        
        auto frameTicks = endTicks - nowTicks;
        auto frameMsec = int(1000*frameTicks / TICKS_PER_SECOND);
        auto leftoverMsec = DESIRED_MSEC_PER_FRAME - frameMsec;
        if(leftoverMsec > 0 )
            SDL_Delay(leftoverMsec);
    }
}
