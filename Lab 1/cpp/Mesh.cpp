
#include "Mesh.h"
#include "glfuncs.h"
#include <iostream>
#include <fstream>
#include "ImageTexture2DArray.h"
#include "math3d.h"
#include "Buffer.h"
#include "utils.h"
#include <sstream>
#include <vector>
#include "parsegltf.h"
#include "parseobj.h"

namespace {
    std::shared_ptr<ImageTexture2DArray> black=nullptr;
    std::shared_ptr<ImageTexture2DArray> white=nullptr;
    std::shared_ptr<ImageTexture2DArray> periwinkle=nullptr;
    
    namespace blackNS {
        #include "black_png.h"
    };
    namespace whiteNS {
        #include "white_png.h"
    };
    namespace periwinkleNS {
        #include "periwinkle_png.h"
    };
   

    void setupVao(GLuint vao, Buffer& pbuff, Buffer& tbuff, Buffer& nbuff, Buffer& Tbuff, Buffer& t2buff, Buffer& ibuff)
    {
    
        glBindVertexArray(vao);
        
        ibuff.bind(GL_ELEMENT_ARRAY_BUFFER);
        
        pbuff.bind(GL_ARRAY_BUFFER);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer( 0, 3, GL_FLOAT, false, 3*4, 0 );
        
        tbuff.bind(GL_ARRAY_BUFFER);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer( 1, 2, GL_FLOAT, false, 2*4, 0 );
        
        nbuff.bind(GL_ARRAY_BUFFER);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer( 2, 3, GL_FLOAT, false, 3*4, 0 );
        
        Tbuff.bind(GL_ARRAY_BUFFER);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer( 3, 4, GL_FLOAT, false, 4*4, 0 ); 
        
        t2buff.bind(GL_ARRAY_BUFFER);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer( 4, 2, GL_FLOAT, false, 2*4, 0 );
        
        glBindVertexArray(0);
    }

    class Edge {
        public:
        unsigned i1,i2;
        Edge(unsigned a, unsigned b) : i1(a), i2(b) {}
        bool operator<(const Edge& e2) const {
            unsigned selfMin = std::min(i1,i2);
            unsigned selfMax = std::max(i1,i2);
            unsigned otherMin = std::min(e2.i1,e2.i2);
            unsigned otherMax = std::max(e2.i1,e2.i2);
            if( selfMin < otherMin )
                return true;
            if( selfMin > otherMin )
                return false;
            return selfMax < otherMax;
        }
        bool operator==(const Edge& e2) const {
            return !( *this < e2 || e2 < *this );
        }
    };

    #if 0
    void computeAdjacency( const std::vector<uint32_t>& indices, std::vector<uint32_t>& aindices)
    {
        std::map<Edge,std::vector<unsigned> > M;
        for(uint32_t i=0;i<indices.size(); i+=3 ){
            uint32_t i0 = i, i1=i+1, i2=i+2;
            uint32_t v[3] = { indices[i0], indices[i1], indices[i2] };
            for(uint32_t j=0;j<3;++j){
                uint32_t a = v[j];
                uint32_t b = v[(j+1)%3];
                uint32_t c = v[(j+2)%3];
                M[Edge(a,b)].push_back(c);
            }
        }
        
        for(unsigned i=0;i<indices.size(); i+=3 ){
            uint32_t i0 = i, i1=i+1, i2=i+2;
            uint32_t v[3] = { indices[i0], indices[i1], indices[i2] };
            for(uint32_t j=0;j<3;++j){
                uint32_t a = v[j];
                uint32_t b = v[(j+1)%3];
                uint32_t c = v[(j+2)%3];
                
                Edge e(a,b);
                assert( !M[e].empty() );
                
                aindices.push_back( a );
                
                if( M[e].size() == 1 ){
                    //bare edge
                    aindices.push_back(c);
                } else if( M[e].size() == 2 ){
                    //manifold edge
                    if( M[e][0] == c )
                        aindices.push_back( M[e][1] );
                    else
                        aindices.push_back( M[e][0] );
                } else {
                    //nonmanifold edge
                    if( M[e][0] == c )
                        aindices.push_back( M[e][1] );
                    else
                        aindices.push_back( M[e][0] );
                }
            }
        }
        return;
    }
    #endif

};  //anonymous namespace




class Mesh::Material {
  public:
    std::string name;
    std::shared_ptr<ImageTexture2DArray> baseTexture;
    std::shared_ptr<ImageTexture2DArray> emissiveTexture;
    std::shared_ptr<ImageTexture2DArray> normalTexture;
    std::shared_ptr<ImageTexture2DArray> roughnessTexture;
    float metallicFactor=0.0f;
    float roughnessFactor=0.0f;
    Material(){ metallicFactor=0.0f; roughnessFactor=0.0f; }
};  

class Mesh::MaterialInfo{
  public:
    MaterialInfo(){ byteStart=count=0; }
    Mesh::Material mtl;
    unsigned byteStart=0;
    unsigned count=0;
};


class Mesh::Helpers {
public:
   static void initialize( Mesh* M, const std::vector<vec3>& positions,
        const std::vector<vec3>& normals, const std::vector<vec2>& texcoords,
        const std::vector<vec2>& texcoords2, const std::vector<vec4>&tangents,
        const std::vector<uint32_t>& indices,
        const std::vector< std::tuple< std::shared_ptr<gltf::Material> , unsigned , unsigned > >& mlist,
        const std::vector<vec3>& lightPositions)
    {
        for(auto& tpl : mlist ){
            auto& m = std::get<0>(tpl);
            Material m2;
            if( !m )
                m2.name="(null)";
            else
                m2.name = m->name;
            if( m && m->baseTexture )
                m2.baseTexture =  std::make_shared<ImageTexture2DArray>( m->baseTexture->source->view->ptr(), (uint32_t) m->baseTexture->source->view->length );
            else
                m2.baseTexture = white;
            if( m && m->emissiveTexture ){
                m2.emissiveTexture = std::make_shared<ImageTexture2DArray>( m->emissiveTexture->source->view->ptr(), (uint32_t)  m->emissiveTexture->source->view->length );
            } else{
                m2.emissiveTexture = black;
            }
            if( m && m->roughnessTexture )
                m2.roughnessTexture = std::make_shared<ImageTexture2DArray>( m->roughnessTexture->source->view->ptr(),(uint32_t)  m->roughnessTexture->source->view->length );
            else
                m2.roughnessTexture = white;
            if( m && m->normalTexture )
                m2.normalTexture =  std::make_shared<ImageTexture2DArray>( m->normalTexture->source->view->ptr(), (uint32_t)    m->normalTexture->source->view->length );
            else
                m2.normalTexture = periwinkle;
            if(m){
                m2.metallicFactor = m->metallicFactor;
                m2.roughnessFactor = m->roughnessFactor;
            } else {
                m2.metallicFactor = 1.0;
                m2.roughnessFactor = 1.0;
            }
            
            MaterialInfo mi;
            mi.mtl = m2;
            mi.byteStart = std::get<1>(tpl)*4;
            mi.count = std::get<2>(tpl);
            M->materialList.push_back( mi );
        }


        std::vector<uint32_t> aindices;
        //computeAdjacency( indices, aindices );


        Buffer pbuff(positions );
        Buffer tbuff(texcoords);
        Buffer t2buff(texcoords2);
        Buffer nbuff(normals);
        Buffer Tbuff(tangents);
        Buffer ibuff(indices);
        Buffer aibuff(aindices);
        
        
        GLuint tmp[2];
        glGenVertexArrays(2,tmp);
        M->vao = tmp[0];
        M->avao = tmp[1];

        setupVao( M->vao, pbuff, tbuff, nbuff, Tbuff, t2buff, ibuff );
        setupVao( M->avao, pbuff, tbuff, nbuff, Tbuff, t2buff, aibuff );
        
        M->lightPositions = lightPositions;
    }   
            
};
   
   
Mesh::Mesh(std::string fname)
{
    this->vao = 0;
    this->avao = 0;  //bogus; only to quiet VS

    if(black == nullptr){
        black = std::make_shared<ImageTexture2DArray>(blackNS::data, (uint32_t) sizeof(blackNS::data));
        white = std::make_shared<ImageTexture2DArray>(whiteNS::data, (uint32_t) sizeof(whiteNS::data));
        periwinkle = std::make_shared<ImageTexture2DArray>(periwinkleNS::data, (uint32_t) sizeof(periwinkleNS::data));
    }
       
    std::vector<vec3> positions;
    std::vector<vec3> normals;
    std::vector<vec2> texcoords;
    std::vector<vec2> texcoords2;
    std::vector<vec4> tangents;
    std::vector<uint32_t> indices;
    std::vector< std::tuple< std::shared_ptr<gltf::Material> , unsigned , unsigned > > mlist;
    std::vector<vec3> lightPositions;
    
    fname = "assets/"+fname;
    
    if( utils::endswith(fname,".glb") )
        gltf::load(fname,positions,normals,texcoords,texcoords2,tangents,indices,mlist,lightPositions);
    else if( utils::endswith(fname,".obj"))
        obj::load(fname,positions,normals,texcoords,tangents,indices,mlist);
    else
        throw std::runtime_error("Unknown mesh format");
        
    Helpers::initialize(this,positions,normals,texcoords,texcoords2,tangents,indices,mlist,lightPositions);
}

Mesh* Mesh::fromObjData(const char* data)
{
    std::vector<vec3> positions;
    std::vector<vec3> normals;
    std::vector<vec2> texcoords;
    std::vector<vec2> texcoords2;
    std::vector<vec4> tangents;
    std::vector<uint32_t> indices;
    std::vector< std::tuple< std::shared_ptr<gltf::Material> , unsigned , unsigned > > mlist;
    std::vector<vec3> lightPositions;

    std::string sdata( data );
    obj::loadFromString(data,"",positions,normals,texcoords,tangents,indices,mlist);
    Mesh* M = new Mesh();
    Helpers::initialize(M,positions,normals,texcoords,texcoords2,tangents,indices,mlist,lightPositions);
    return M;
}



void Mesh::draw()
{
    drawHelper( this->vao, 1 );
}

void Mesh::drawAdjacency()
{
    drawHelper( this->avao, 2 );
}

void Mesh::drawHelper( GLuint vao, unsigned multiplier)
{
    glBindVertexArray(vao);
    for(auto& q : this->materialList){
        q.mtl.baseTexture->bind(0);
        q.mtl.emissiveTexture->bind(1);
        q.mtl.roughnessTexture->bind(2);
        q.mtl.normalTexture->bind(3);
        uint32_t bs = (uint32_t) q.byteStart;
        glDrawElements( GL_TRIANGLES, q.count*multiplier, GL_UNSIGNED_INT, (char*)0 + bs * multiplier );
    }
}

Mesh::~Mesh(){
    glDeleteVertexArrays(1,&vao);
}

Mesh::Mesh(){
    this->vao = 0; this->avao = 0; //bogus, for vs
}
    
 
