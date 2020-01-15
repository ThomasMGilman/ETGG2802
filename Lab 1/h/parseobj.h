//jh fall 2019


#pragma once

#include <vector>
#include <tuple>
#include "parsegltf.h"


namespace obj {


//minfo = list of tuples
//      tuple first = material
//      tuple second = index in the 'indices' list where this material starts
//      tuple third = how many indices use this material
void load(std::string infname, 
        std::vector<vec3>& positions,
        std::vector<vec3>& normals, std::vector<vec2>& texcoords,
        std::vector<vec4>& tangents, std::vector<uint32_t>& indices,
        std::vector< std::tuple< std::shared_ptr<gltf::Material> , unsigned , unsigned > >& minfo);


void loadFromString(const std::string& data, std::string pfx,
        std::vector<vec3>& positions,
        std::vector<vec3>& normals, std::vector<vec2>& texcoords,
        std::vector<vec4>& tangents, std::vector<uint32_t>& indices,
        std::vector< std::tuple< std::shared_ptr<gltf::Material> , unsigned , unsigned > >& minfo);

};
