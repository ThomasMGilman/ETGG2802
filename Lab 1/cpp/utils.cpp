

//utility functions that C++ really *should* have provided...
#include <stdafx.h>
#include "utils.h"
#include <vector>
#include <string>
#include <map>
#include <random>
#include <sstream>
#include <fstream>

namespace utils {
        

    bool endswith( const std::string& str, const std::string& suffix)
    {
        if( str.length() < suffix.length() )
            return false;
        return str.substr( str.length()-suffix.length() ) == suffix;
    }

    void appendFileToBuffer( const std::string& filename, std::vector<uint8_t>& buffer){
        std::ifstream in(filename,std::ios::binary);
        if(!in.good() )
            throw std::runtime_error("Cannot open file "+filename);
        in.seekg(0,std::ios::end);
        size_t sz = in.tellg();
        in.seekg(0);
        size_t i = buffer.size();
        buffer.resize(buffer.size()+sz);
        in.read( (char*) (buffer.data()+i), sz );
    }
    
} //namespace utils
