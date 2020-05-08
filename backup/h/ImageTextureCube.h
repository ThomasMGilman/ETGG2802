#pragma once
#include "TextureCube.h"
#include <vector>


class ImageTextureCube : public TextureCube {
  public:
    //namepattern should have a %d in it; this will be replaced
    //with a number from 0 to 5
    ImageTextureCube( std::string namepattern );
};
