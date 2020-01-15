#pragma once


//utility functions that C++ really *should* have provided...


#include <vector>
#include <string>
#include <map>
#include <cmath>

namespace utils {
        
    bool endswith( const std::string& str, const std::string& suffix);
    std::string replace( const std::string& s, const std::string& search, const std::string& repl);
    void appendFileToBuffer( const std::string& filename, std::vector<uint8_t>& buffer);
    template<typename V>
    V get(std::map<std::string,V>& m, std::string key, const V& defl){
        auto it = m.find(key);
        if( it == m.end() )
            return defl;
        else
            return it->second;
    }

} //namespace utils
