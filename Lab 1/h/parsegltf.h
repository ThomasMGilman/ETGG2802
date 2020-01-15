//jh fall 2019

#pragma once
 
#include <string>
#include <cstdint>
#include <memory>
#include <vector>
#include <tuple>
#include <map>
#include "math3d.h"

namespace json11 {
    class Json;
}

namespace gltf {
    

const int BYTE=5120;
const int UBYTE=5121;
const int SHORT=5122;
const int USHORT=5123;
const int UINT=5124;
const int FLOAT=5126;
const int POINTS=0;
const int LINES=1;
const int LINELOOP = 2;
const int LINESTRIP = 3;
const int TRIANGLES = 4;
const int TRISTRIP = 5;
const int TRIFAN = 6;

class Buffer {
public:
    std::shared_ptr< std::vector<std::uint8_t> > backingStore;
    size_t byteLength;    //bytes
    size_t offset;    //offset from start of backingStore to get to ptr

    Buffer(std::shared_ptr<std::vector<std::uint8_t> >v, size_t offset, size_t len);
    Buffer(std::string fname);
    const std::uint8_t* ptr();
};

class BufferView {
  public:
    std::shared_ptr<Buffer> buffer;
    size_t byteOffset;
    size_t stride;
    size_t length;
    
    BufferView( json11::Json& vinfo, std::vector<std::shared_ptr<Buffer> >& buffers);
    BufferView( std::shared_ptr<Buffer> buffer );
    //returns raw pointer to underlying storage; does not consider stride
    const std::uint8_t* ptr();
};
 

class Image {
  public:
    std::string name;
    std::string mimeType;
    std::shared_ptr<BufferView> view;
    Image(const json11::Json& J, std::vector<std::shared_ptr<BufferView>>& views);
    Image(std::shared_ptr<BufferView> view);
};

class TextureData{
  public:
    std::shared_ptr<Image> source;
    std::string name;
    TextureData(const json11::Json& J, std::vector<std::shared_ptr<Image> > images);
    TextureData(std::string name, std::shared_ptr<Image> src);
};

   
class Light{
  public:
    vec3 color;
    std::string type;
    Light( vec3 color, std::string type);
};

class Material {
  public:
    std::string name;
    std::shared_ptr<TextureData>  normalTexture;
    int normalTextureIndex=0;
    std::shared_ptr<TextureData>  occlusionTexture;
    int occlusionTextureIndex=0;
    std::shared_ptr<TextureData>  emissiveTexture;
    int emissiveTextureIndex=0;
    std::shared_ptr<TextureData>  roughnessTexture;
    int roughnessTextureIndex=0;
    std::shared_ptr<TextureData>  baseTexture;
    int baseTextureIndex=0;
    vec3 emissiveFactor{0,0,0};
    vec4 baseColorFactor{1,1,1,1};
    float metallicFactor=1.0f;
    float roughnessFactor=1.0f;
    Material(const json11::Json& J, std::vector<std::shared_ptr<TextureData> >& textures);
    Material(std::string name);
};  
    

class Accessor {
    const uint8_t* ptr();
  public:
    std::shared_ptr<BufferView> view;
    int byteOffset;
    int componentType;  //FLOAT or UINT or USHORT or UBYTE 
    int count;
    std::string type;        //"SCALAR", "VECn", "MATn"
    
    Accessor(const json11::Json& ai, std::vector<std::shared_ptr<BufferView> >& views);
    //returns the number of items added to the vector
    int getData(std::vector<vec2>& v);
    int getData(std::vector<vec3>& v);
    int getData(std::vector<vec4>& v);
    int getData(std::vector<uint32_t>& v);
};    
    
class Primitive{
    public:
    //key = name: POSITION, NORMAL, TEXCOORD_0, TANGENT, etc.
    std::map<std::string, std::shared_ptr<Accessor> > attributes;
    std::shared_ptr<Accessor>  indices;
    std::shared_ptr<Material>  material;
    int primType;   //GL_TRIANGLES, GL_LINES, etc.
    Primitive(const json11::Json& J, const std::vector<std::shared_ptr<Accessor> >& accessors, std::vector<std::shared_ptr<Material> >& materials );
};



class Mesh {
  public:
    std::vector<std::shared_ptr<Primitive> > primitives;
    std::string name;
    Mesh(const json11::Json& J1, std::vector<std::shared_ptr<Accessor> >& accessors, std::vector<std::shared_ptr<Material> >& materials);
};

           
class Node {
    std::vector<int> childIndices;
  public:
    std::vector<std::shared_ptr<Node> > children;
    mat4 matrix;    //for this node only
    mat4 inverse;   //only rotation and scale components
    std::shared_ptr<Mesh>  mesh;
    std::string name;
    std::vector<std::shared_ptr<Light> > lights;
    
    Node(const json11::Json& J, std::vector<std::shared_ptr<Mesh> >& meshes, std::vector<std::shared_ptr<Light> >& lights);
    void setChildren(std::vector<std::shared_ptr<Node> >& nodes);
};
    
    
class Scene {
  public:
    std::vector<std::shared_ptr<Node> > nodes;
    std::string name;
    Scene(const json11::Json& J, std::vector<std::shared_ptr<Node> >& nodes);
};
 
 
//minfo = list of tuples
//      tuple first = material
//      tuple second = index in the 'indices' list where this material starts
//      tuple third = how many indices use this material
void load(std::string infname, 
        std::vector<vec3>& positions,
        std::vector<vec3>& normals, 
        std::vector<vec2>& texcoords,
        std::vector<vec2>& bumpmaptexcoords,
        std::vector<vec4>& tangents, 
        std::vector<uint32_t>& indices,
        std::vector< std::tuple< std::shared_ptr<Material> , unsigned , unsigned > >& minfo,
        std::vector<vec3>& lightPositions
        );
    
std::shared_ptr<Scene> loadScene(std::string infname);

}; //namespace gltf
