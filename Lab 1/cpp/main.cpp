#include <stdafx.h>
#include <setupgl.h>
#include <game_state.h>

void update(int elapsed)
{
    GAME_STATE->update((float)elapsed);
}

void draw()
{
    GAME_STATE->draw();
}

int main(int , char** )
{
    setupGL(SCREENWIDTH, SCREENHEIGHT);
    GameState* MainGameState = new GameState();
    mainloop(update, draw);
    delete(MainGameState);
    return 0;
}

