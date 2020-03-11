#include "bill_board_manager.h"

BillboardManager::BillboardManager(ImageTexture2DArray tex, vec2 size)
{
    texture = &tex;
    this->size = size;
    this->halfSize = size / 2;
    if (BillboardManager::vao == 0)
    {
        GLuint tmp[1];
        glGenVertexArrays(1, tmp);
        BillboardManager::vao = tmp[0];
        glBindVertexArray(BillboardManager::vao);
    }
    
    //create buffer
    std::vector<float> trash;
    trash.push_back(4);
    positionBuffer = new Buffer(trash);
    ubo = new Buffer(numBulletsAllowed * sizeof(vec4), GL_DYNAMIC_DRAW);
    positionBuffer->bind(GL_ARRAY_BUFFER);
    //create vao
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 1, GL_FLOAT, false, 4, 0);
    glBindVertexArray(0);
    positions.reserve(numBulletsAllowed);
}

BillboardManager::~BillboardManager()
{
    delete(positionBuffer);
    delete(ubo);
}

void BillboardManager::setUniforms()
{
    prog.setUniform("halfBoardSize", halfSize);
}

void BillboardManager::checkDirty()
{
    if (dirty)
    {
        glBufferSubData(GL_UNIFORM_BUFFER, 0, positions.size() * sizeof(positions[0]), positions.data());
        dirty = false;
    }
}

void BillboardManager::add(vec3 pos, vec3 vel)
{
    if (positions.size() < numBulletsAllowed)
    {
        this->positions.push_back(vec4(pos, 1));
        this->velocities.push_back(vec4(vel, 0));
        lifetimes.push_back(750);
        dirty = true;
    }
}

void BillboardManager::update(float elapsed, std::function<void(vec3)>& callback)
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
}

void BillboardManager::draw()
{
    if (positions.size() > 0)
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glDepthMask(false);

        auto oldprog = Program::current;
        prog.use();
        texture->bind(0);

        setUniforms();

        ubo->bindBase(GL_UNIFORM_BUFFER, 1);
        checkDirty();

        glBindVertexArray(vao);
        glDrawArraysInstanced(
            GL_POINTS,            //Mode
              0,                  //Start
              1,                  //How many verts
              positions.size());  //number of instances

        if (oldprog)
            oldprog->use();
        glDepthMask(true);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 
    }
}

GLuint BillboardManager::vao = 0;