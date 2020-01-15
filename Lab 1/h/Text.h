#pragma once

#include "math3d.h"
#include "DataTexture2DArray.h"
#include <string>
#include <memory>

class Text{
    Text(const Text&) = delete;
    void operator=(const Text&) = delete;
    std::string data;
    bool dirty=true;
    int size;
    vec2 pos;
    vec3 color;
    std::shared_ptr<DataTexture2DArray> datatex;
  public:
  
    /** Initialize the Text.
        @param size Size in points. Only a limited number of sizes are valid.
        @param position The position, in pixel coordinates.
        @param s The initial text.
    */
    Text(int size, vec2 position, std::string s);
    Text(int size): Text(size,vec2(0,0),""){}
    void setColor(vec3 c);
    void setPosition(vec2 position);
    void setText(std::string s);
    void draw(int screenWidth, int screenHeight, float depth=-1.0);
};
