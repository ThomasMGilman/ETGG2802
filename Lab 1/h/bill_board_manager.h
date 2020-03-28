#pragma once
#include <stdafx.h>

class BillBoardManager {
private:
    static GLuint vao;
    static Buffer* positionBuffer, * ubo;
    std::shared_ptr<ImageTexture2DArray> diffuse_texture;

    static const int numObjsAllowed = 100;

    void setup_vao();

public:
    vec3 size, halfSize;
    bool dirty = false;

    std::vector<vec4> positions, velocities;
    std::vector<int> lifetimes;

    Program prog{ "billBoardvs.txt", "billBoardgs.txt", "billBoardfs.txt" };
    
    BillBoardManager(std::shared_ptr<ImageTexture2DArray> diffuse_tex, vec3 size);

    ~BillBoardManager();

    void setUniforms();

    void checkDirty();

    void add(vec3 pos, vec3 vel);

    template <typename T>
    void update(float elapsed, const T& callback)
    {
        dirty = true;
        for (int i = 0; i < positions.size(); i++)
        {
            lifetimes[i] -= elapsed;
            if (lifetimes[i] <= 0)
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