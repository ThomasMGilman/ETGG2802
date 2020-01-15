#pragma once

#include <SDL.h>
#include <set>

void setupGL(int winw, int winh);
typedef void (*UpdateFunc)(int);
typedef void (*DrawFunc)(void);
typedef void (*KeyCallback)(int);
typedef void (*MouseLookCallback)(float,float);
typedef void (*MouseDownCallback)();
void mainloop(UpdateFunc update, DrawFunc draw);
const std::set<int>& pumpEvents( MouseLookCallback, KeyCallback down, KeyCallback up, MouseDownCallback click );
