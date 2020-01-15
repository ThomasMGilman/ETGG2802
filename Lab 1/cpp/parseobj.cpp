

#include "parseobj.h"
#include <iostream>
#include <fstream>
#include <tuple>
#include <memory>

using namespace std;

namespace obj {
    
    
 

namespace {


    typedef std::array<int,3> Vertex;
    typedef std::array< Vertex, 3> Triangle;    

    
    
    std::string replace( const std::string& s, const std::string& search, const std::string& repl)
    {
        std::string ret;
        size_t start = 0;
        while(start < s.length() ){
            auto pos = s.find(search, start );
            if( pos == std::string::npos ){
                ret += s.substr(start);
                break;
            } else {
                ret += s.substr(start,pos);
                ret += repl;
                if( search.empty() )
                    start = pos + 1;
                else
                    start = pos + search.length();
            }
        }
        return ret;
    }

    bool equalany(const char* s, char c){
        for(unsigned i=0;s[i];++i){
            if( s[i] == c )
                return true;
        }
        return false;
    }

    std::vector<std::string> split(std::string line, const char* delims){
        std::vector<std::string> v;
        const char* p = line.c_str();
        
        size_t i=0;
        while(i<line.length()){
            
            //skip over any delimiters
            while( i < line.length() && equalany(delims, line[i] ) )
                i++;
            if( i == line.length() )
                break;
                
            //i points to first non-delim character
            
            size_t j=i+1;
            while( j < line.length() && !equalany(delims, line[j]) )
                j++;
                
            //j points to first delimiter after i or else end of string
            std::string s(p+i, j-i);
            v.push_back(s);
            i = j;
        }
        return v;
    }
    
    
    std::vector<std::string> split(std::string line){
        return split( line, " \t\r\n");
    }

    std::vector<std::string> split(std::string line, char delim){
        char x[2];
        x[0]=delim;
        x[1]=0;
        return split(line,x);
    }

    std::shared_ptr<gltf::TextureData> loadTexture(string pfx, string fname){
        std::string fn;
        if( !pfx.empty() )
            fn = pfx+fname;
        else
            fn = fname;
        
        auto buffer = std::make_shared<gltf::Buffer>(fn);
        auto view = std::make_shared<gltf::BufferView>(buffer);
        auto img = std::make_shared<gltf::Image>(view);
        auto tex = std::make_shared<gltf::TextureData>(fname,img);
        return tex;
    }

    void readMaterials(std::string pfx, std::string fname, 
                       std::map<std::string, std::shared_ptr<gltf::Material> >& materials)
    {
        std::string fn;
        if( !pfx.empty() )
            fn = pfx+fname;
        else
            fn = fname;
            
        std::ifstream in(fn);
        if(!in.good())
            throw std::runtime_error("Could not open "+fn);
         
        std::shared_ptr<gltf::Material> currmtl;
        
        while(true){
            std::string line;
            //use stream I/O here -- not too many lines, so not too costly
            std::getline(in,line);
            if( in.fail() )
                break;
            auto lst = split(line);
            if( lst.empty() || lst[0][0] == '#' )
                continue;
            if(lst[0] == "newmtl"){
                currmtl = std::make_shared<gltf::Material>(lst[1]);
                materials[lst[1]] = currmtl;
            } 
            else if(lst[0] == "map_Kd") 
                currmtl->baseTexture = loadTexture(pfx,lst[1]);
            else if(lst[0] == "map_Ke")
                currmtl->emissiveTexture = loadTexture(pfx,lst[1]);
            else if(lst[0] == "map_bump" || lst[0] == "map_Bump")
                currmtl->normalTexture = loadTexture(pfx,lst[1]);
            else if(lst[0] == "map_Ka")
                currmtl->occlusionTexture = loadTexture(pfx,lst[1]);
            else if(lst[0] == "map_Ns")
                currmtl->roughnessTexture = loadTexture(pfx,lst[1]);
            else if( lst[0] == "Ke" )
                currmtl->emissiveFactor = vec3( stof(lst[1]),stof(lst[2]),stof(lst[3]));
            else if( lst[0] == "Kd" )
                currmtl->baseColorFactor = vec4( stof(lst[1]),stof(lst[2]),stof(lst[3]),1.0f);
            else if( lst[0] == "Ns" )
                currmtl->roughnessFactor = (1.0f-stof(lst[1])) * 128.0f;        //arbitrary constant
        }
    }
    
} //anonymous namespace

void load(std::string infname, 
        std::vector<vec3>& positions,
        std::vector<vec3>& normals, std::vector<vec2>& texcoords,
        std::vector<vec4>& tangents, std::vector<uint32_t>& indices,
        std::vector< std::tuple< std::shared_ptr<gltf::Material> , unsigned , unsigned > >& minfo)
{

    std::string pfx;
    infname = replace( infname, "\\", "/");
    auto idx = infname.rfind("/");
    if( idx != std::string::npos ){
        pfx = infname.substr(0,idx+1);
    }
    std::ifstream fin(infname);
    if( !fin.good() )
        throw std::runtime_error("Could not open file "+infname);
        
    std::string fileData;
    std::getline( fin, fileData, '\0' );
    
    loadFromString( fileData,  pfx, positions,
         normals,  texcoords,
         tangents,  indices,
         minfo);
}


void loadFromString(const std::string& fileData, 
        string pfx,
        std::vector<vec3>& positions,
        std::vector<vec3>& normals, std::vector<vec2>& texcoords,
        std::vector<vec4>& tangents, std::vector<uint32_t>& indices,
        std::vector< std::tuple< std::shared_ptr<gltf::Material> , unsigned , unsigned > >& minfo)
{
    
    std::vector<string> lines = split(fileData, "\n" );
    
    std::string currMaterial;
    std::vector<vec3> filePos;
    std::vector<vec2> fileTex;
    std::vector<vec3> fileNorms;
    std::map<std::string, std::vector<Triangle> > fileTris ;
    std::map<std::string, std::shared_ptr<gltf::Material> > materials;
    
    for(auto& line : lines ){
        std::vector<std::string> lst = split(line);
        if( lst.empty() )
            continue;
        if( lst[0][0] == '#' )
            continue;

        if( lst[0] == "mtllib" )
            readMaterials(pfx,lst[1],materials);
        else if( lst[0] == "v" )
            filePos.push_back( vec3(stof(lst[1]),stof(lst[2]),stof(lst[3])) );
        else if(lst[0] == "vn")
            fileNorms.push_back( vec3(stof(lst[1]),stof(lst[2]),stof(lst[3])) );
        else if( lst[0] == "vt" )
            fileTex.push_back( vec2(stof(lst[1]),stof(lst[2])));
        else if( lst[0] == "f" ){
            if(lst.size() != 4 )
                throw std::runtime_error("Mesh has non-triangles");
            Triangle triangle;
            for(int i=1;i<4;++i){
                auto spec = split( lst[i], '/' );
                //vi  vi/ti  vi/ti/ni   vi//ni
                if( spec.size() != 3 )
                    throw std::runtime_error("Mesh lacks texture coordinates or normals");
                int vi = std::stoi(spec[0])-1;
                int ti = std::stoi(spec[1])-1;
                int ni = std::stoi(spec[2])-1;
                triangle[i-1] = Vertex{vi,ti,ni};
            }
            fileTris[currMaterial].push_back(triangle);
        }
        else if( lst[0] == "usemtl" ){
            currMaterial = lst[1] ;
        }
    }
    
    
            
    std::map< Vertex, int> vmap;
    int numVerts=0;
    
    
    for(auto& it : fileTris ){
        std::string materialName = it.first;
        auto& tris = it.second;
        auto material = materials[materialName];
        std::tuple< std::shared_ptr<gltf::Material> , unsigned , unsigned > tpl(
            material,
            (unsigned)indices.size(),
            (unsigned)tris.size()*3
        );

        minfo.push_back( tpl );
        
        for( Triangle triangle : tris ){
            for( auto key : triangle ){
                if( vmap.find(key) == vmap.end() ){
                    vmap[key] = numVerts++;
                    int vi = key[0];
                    int ti = key[1];
                    int ni = key[2];
                    vec3 pos = filePos[vi];
                    vec2 tex = fileTex[ti];
                    vec3 norm = fileNorms[ni];
                    positions.push_back( pos );
                    normals.push_back( norm );
                    texcoords.push_back( tex );
                    tangents.push_back(vec4(0,0,0,1));
                }
                indices.push_back( vmap[key] );
            }
        }
    }
       
    //compute tangents
    bool warned=false;
    
    for(unsigned i=0;i<indices.size();i+=3){
        int i0 = indices[i];
        int i1 = indices[i+1];
        int i2 = indices[i+2];
        
        vec3 qq[3] = {  positions[i0], positions[i1], positions[i2] };
        float ss[3] = {  texcoords[i0].x, texcoords[i1].x, texcoords[i2].x };
        float tt[3] = {  texcoords[i0].y, texcoords[i1].y, texcoords[i2].y };
        float q1x = qq[1].x-qq[0].x;
        float q1y = qq[1].y-qq[0].y;
        float q1z = qq[1].z-qq[0].z;
        float q2x = qq[2].x-qq[0].x;
        float q2y = qq[2].y-qq[0].y;
        float q2z = qq[2].z-qq[0].z;
        float s1 = ss[1] - ss[0];
        float s2 = ss[2] - ss[0];
        float t1 = tt[1] - tt[0];
        float t2 = tt[2] - tt[0];
        
        float denom = (s1*t2-s2*t1);
        if( fabs(denom) < 0.00001 ){
            if(!warned){
                std::cout << "Warning: Bad texture coordinates: Tangents will be incorrect\n";
                warned=true;
            }
        } else {
            float tmp = 1.0f / denom;
            float Tx = tmp * (t2*q1x-t1*q2x);
            float Ty = tmp * (t2*q1y-t1*q2y);
            float Tz = tmp * (t2*q1z-t1*q2z);
            vec4 tangent( normalize(vec3(Tx,Ty,Tz)), 0.0f );
            tangents[i0] += tangent;
            tangents[i1] += tangent;
            tangents[i2] += tangent;
        }
    }
    
    for(unsigned i=0;i<tangents.size();++i){
        if( tangents[i].isZero() )
            tangents[i].setXYZ( vec3(1,0,0) );
        else
            tangents[i].setXYZ( normalize(tangents[i].xyz() ) );
    }
}





}; //namespace obj
