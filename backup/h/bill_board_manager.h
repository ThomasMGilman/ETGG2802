#pragma once
#include <stdafx.h>

class BillBoardManager {
private:
    static GLuint vao;
    static Buffer* positionBuffer, * ubo;
    static Program* prog;
    std::shared_ptr<ImageTexture2DArray> diffuse_texture;

    static const int numObjsAllowed = 100;

    void setup_vao();

public:
    vec3 size, halfSize;
    bool dirty = false, dying, usingNoise;
    float time = 0;

    std::vector<vec4> positions, velocities;
    std::vector<int> lifetimes;
    
    BillBoardManager(std::shared_ptr<ImageTexture2DArray> diffuse_tex, vec3 size, bool dies = true, bool useNoise = false);

    ~BillBoardManager();

    void setUniforms();
    
    void cleanUniforms();

    void checkDirty();

    void add(vec3 pos, vec3 vel);

    template <typename T>
    void update(float elapsed, const T& callback)
    {
        dirty = true;
        time += elapsed;
        prog->setUniform("time", time * .00001);
        for (int i = 0; i < positions.size(); i++)
        {   
            if (dying)
                lifetimes[i] -= elapsed;
            if (dying && lifetimes[i] <= 0)
            {
                vec3 pos = positions.at(i).xyz();
                callback(pos);
                positions.erase(positions.begin() + i);
                velocities.erase(velocities.begin() + i);
                lifetimes.erase(lifetimes.begin() + i);
            }
            else
                positions[i] += velocities[i] * elapsed;
        }
    };

    void draw();
};