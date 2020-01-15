// jh fall 2019

#pragma once

#include <cstdint>
#include <string>
#include <vector>

class Image {
    Image(const Image&) = delete;
    void operator=(const Image&) = delete;
    
public:
    /**Load PNG or JPEG file*/
    Image(std::string filename);
 
    /** Initialize from PNG or JPEG data*/
    Image(const void* data, size_t size);

    /** Initialize from PNG or JPEG data*/
    template<typename T>
    Image(const std::vector<T>& data) : Image(data.data(), data.size()*sizeof(T) ) {}
     
    /** Create an empty image of the given size and format*/
    Image(unsigned w, unsigned h, std::string fmt);
     
    
    unsigned width() const ;
    unsigned height() const;
    unsigned bytesPerPixel() const ;
    unsigned totalBytes() const { return width()*height()*bytesPerPixel(); }
    
    void flipVertically();
    void sample(float s, float t, uint8_t& r, uint8_t& g, uint8_t b, uint8_t a);
    void getPixel(unsigned x, unsigned y, std::uint8_t& r,   
            std::uint8_t& g, std::uint8_t& b, std::uint8_t& a);
    void setPixel(unsigned x, unsigned y, std::uint8_t r,   
            std::uint8_t g, std::uint8_t b, std::uint8_t a=255);
    std::string format() const ;
    std::uint8_t* pixels();
    const std::uint8_t* pixels() const;
    void writePng(std::string fname) const ;
    void writeBmp(std::string fname) const;
    
private:
    unsigned w,h;
    unsigned Bpp;   //bytes per pixel
    std::string fmt;
    std::vector<uint8_t> pixbuff;
    unsigned pixbuffOffset;
    
    void computePixbuffOffset();
    void initialize(const void* data, size_t size);
};
