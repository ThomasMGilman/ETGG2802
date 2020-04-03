#include <stdafx.h>
#include "bill_board_manager.h"

void BillBoardManager::setup_vao()
{
    if (BillBoardManager::vao == 0)
    {
        BillBoardManager::prog = new Program( "bill_board_vs.txt", "bill_board_gs.txt", "bill_board_fs.txt" );

        GLuint tmp[1];
        glGenVertexArrays(1, tmp);
        BillBoardManager::vao = tmp[0];
        glBindVertexArray(BillBoardManager::vao);

        //create buffer
        std::vector<float> trash;
        trash.push_back(4);
        positionBuffer = new Buffer(trash);
        ubo = new Buffer(numObjsAllowed * sizeof(vec4), GL_DYNAMIC_DRAW);
        positionBuffer->bind(GL_ARRAY_BUFFER);
        //create vao
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 1, GL_FLOAT, false, 4, 0);
        glBindVertexArray(0);
    }
    positions.reserve(numObjsAllowed);
}

BillBoardManager::BillBoardManager(std::shared_ptr<ImageTexture2DArray> diffuse_tex, vec3 size)
{
    this->diffuse_texture = diffuse_tex;
    this->size = size;
    this->halfSize = size / 2;
    setup_vao();
}

BillBoardManager::~BillBoardManager()
{
    delete(positionBuffer);
    delete(ubo);
}

void BillBoardManager::setUniforms()
{
    prog->setUniform("halfBoardSize", halfSize.xy());
}

void BillBoardManager::checkDirty()
{
    if (dirty)
    {
        glBufferSubData(GL_UNIFORM_BUFFER, 0, positions.size() * sizeof(positions[0]), positions.data());
        dirty = false;
    }
}

void BillBoardManager::add(vec3 pos, vec3 vel)
{
    if (positions.size() < numObjsAllowed)
    {
        this->positions.push_back(vec4(pos, 1));
        this->velocities.push_back(vec4(vel, 0));
        lifetimes.push_back(750);
        dirty = true;
    }
}

void BillBoardManager::draw()
{
    if (positions.size() > 0)
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glDepthMask(false);

        auto oldprog = Program::current;
        this->prog->use();
        this->diffuse_texture->bind(0);

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

GLuint BillBoardManager::vao = 0;

Buffer* BillBoardManager::positionBuffer = 0;

Buffer* BillBoardManager::ubo = 0;

Program* BillBoardManager::prog = nullptr;