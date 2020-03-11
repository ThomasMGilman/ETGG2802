#pragma once
#include "Program.h"
#include <functional>
#include "ImageTexture2DArray.h"

class BillboardManager {
public:
    vec2 size, halfSize;
    int numBulletsAllowed = 100;
    bool dirty = false;

    std::vector<vec4> positions, velocities;
    std::vector<int> lifetimes;

    Program prog{ "billBoardvs.txt", "billBoardgs.txt", "billBoardfs.txt" };
    ImageTexture2DArray* texture;
    Buffer* positionBuffer, *ubo;
    static GLuint vao;
    
    BillboardManager(ImageTexture2DArray tex, vec2 size);
    ~BillboardManager();
    void setUniforms();
    void checkDirty();
    void add(vec3 pos, vec3 vel);
    void update(float elapsed, std::function<void(vec3)>& callback);
    void draw();
};