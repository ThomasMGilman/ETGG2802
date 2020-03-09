#include "bill_board_manager.h"

BillboardManager::BillboardManager(std::shared_ptr<ImageTexture2DArray> tex, vec2 size)
{
}

std::list<vec3>::iterator BillboardManager::add(vec3 pos)
{
	return std::list<vec3>::iterator();
}

void BillboardManager::remove(std::list<vec3>::iterator it)
{
}

void BillboardManager::setPosition(std::list<vec3>::iterator it, vec3 v)
{
}

void BillboardManager::draw()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(false);

    glDepthMask(true);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
