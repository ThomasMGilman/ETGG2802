#pragma once
#include "Program.h"
#include "ImageTexture2DArray.h"
#include <list>

class BillboardManager {
public:
    std::list<vec3> positions;
    Program prog{ "billBoardvs.txt", "billBoardgs.txt", "billBoardfs.txt" };
    bool dirty = false;
    std::shared_ptr<Buffer> positionBuffer;
    GLuint vao;
    std::shared_ptr<ImageTexture2DArray> texture;
    vec2 size;

    BillboardManager(std::shared_ptr<ImageTexture2DArray> tex, vec2 size);
    std::list<vec3>::iterator add(vec3 pos);
    void remove(std::list<vec3>::iterator it);
    void setPosition(std::list<vec3>::iterator it, vec3 v);
    void draw();
};