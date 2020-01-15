
#pragma once

#include "glfuncs.h"
#include "Texture2DArray.h"
#include <initializer_list>
#include <vector>
#include <string>

class ImageTexture2DArray: public Texture2DArray 
{
    ImageTexture2DArray(const ImageTexture2DArray&) = delete;
    void operator=(const ImageTexture2DArray&) = delete;
    void init(const std::vector<std::string>& files);
    void init(std::vector<std::vector<std::uint8_t>>& inData);

  public:
    ImageTexture2DArray( std::string fname);
    ImageTexture2DArray( const std::initializer_list<std::string>& files);
    ImageTexture2DArray( const std::vector<std::string>& files);
    ImageTexture2DArray( const void* p, unsigned byteSize );
};
