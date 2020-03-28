#pragma once
#include <stdafx.h>
#include <game_state.h>

void setupGL(int winw, int winh);
typedef void (*UpdateFunc)(int);
typedef void (*DrawFunc)(void);
typedef void (*KeyCallback)(int);
typedef void (*MouseLookCallback)(float,float);
typedef void (*MouseDownCallback)();
void mainloop(UpdateFunc update, DrawFunc draw);
//void mainloop(GameState* gs);