
#include <stdafx.h>
#include "ImageTexture2DArray.h"
#include "Image.h"
#include <iostream>
#include <fstream>
#include "Zip.h"
#include <algorithm>
#include "glfuncs.h"
#include "utils.h"

using namespace std;

ImageTexture2DArray::ImageTexture2DArray(string fname) : ImageTexture2DArray({fname})
{
}

ImageTexture2DArray::ImageTexture2DArray( const std::initializer_list<string>& files): Texture2DArray(0,0,0) {
    std::vector<string> v;
    for(auto x: files ){
        v.push_back(x);
    }
    init(v);
}

ImageTexture2DArray::ImageTexture2DArray( const std::vector<string>& files): Texture2DArray(0,0,0) {
    init(files);
}
 
ImageTexture2DArray::ImageTexture2DArray( const void* p, unsigned byteSize ): Texture2DArray(0,0,0)
{
    std::vector<std::vector<std::uint8_t>> membuf(1);
    membuf.back().insert(membuf.back().end(), (uint8_t*)p, ((uint8_t*)p)+byteSize);
    init(membuf);
}

void ImageTexture2DArray::init(const std::vector<string>& files){
    std::vector<std::vector<std::uint8_t>> membuf(1);
    membuf.resize(files.size());
    for(unsigned i=0;i<files.size();++i){
        utils::appendFileToBuffer("assets/"+files[i],membuf[i]);
    }
    init(membuf);
}

void ImageTexture2DArray::init(std::vector<std::vector<std::uint8_t>>& inData)
{
    std::vector<std::uint8_t> membuf;
    
    for(unsigned i=0;i<inData.size();++i){
        if( inData[i].size() > 2 && inData[i][0] == 'P' && inData[i][1] == 'K' ){
            //zip file or ORA file
            Zip z(inData[i]);
            auto namelist = z.namelist();
            std::sort(namelist.begin(),namelist.end());
            for(auto n : namelist ){
                if(n.find("thumbnail") != string::npos )
                    continue;
                if( utils::endswith(n,".png") || utils::endswith(n,".jpg") ){
                    auto ze = z.open(n);
                    auto tmp = ze.read();
                    Image img(tmp);
                    if( this->w == 0 ){
                        this->w=img.width();
                        this->h=img.height();
                    } else {
                        if( this->w != img.width() or this->h != img.height() ){
                            throw std::runtime_error("Size mismatch: "+std::to_string(w) + "," + std::to_string(h) +
                                " " + std::to_string(img.width()) + "," + std::to_string(img.height() ));
                        }
                    }
                    slices+=1;
                    membuf.insert( membuf.end(), img.pixels(), img.pixels()+img.totalBytes() );
                }
            }
        } else {
            //png or jpg?
            Image img(inData[i]);
            img.flipVertically();
            if( this->w == 0 ){
                this->w=img.width();
                this->h=img.height();
            } else {
                if( this->w != img.width() or this->h != img.height() ){
                    throw std::runtime_error("Size mismatch: "+std::to_string(w) + "," + std::to_string(h) +
                        " " + std::to_string(img.width()) + "," + std::to_string(img.height() ));
                }
            }
            this->slices += 1;
            membuf.insert( membuf.end(), img.pixels(), img.pixels()+img.totalBytes() );
        }
       
        GLuint tmp[1];
        glGenTextures(1,tmp);
        this->tex = tmp[0];
        this->bind(0);
        glTexImage3D( GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, 
            w,h,slices, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
            membuf.data() );
        this->generateMipmap();
        this->unbind(0);
    }
};

