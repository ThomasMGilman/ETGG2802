//jh fall 2019

#include "parsegltf.h"
#include <cstring>
#include "json11.h"
#include <vector>
#include <cassert>
#include "math3d.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <tuple>

using namespace std;
using json11::Json;

namespace gltf {


class Binaries {
    std::vector< std::shared_ptr<vector<uint8_t> > >binaryDatas;
    int currentBinary=0;
    int offsetInCurrentBinary=0;
public:    
    Binaries(){
    }
    
    void addBinary(shared_ptr<vector<uint8_t> > x){
        binaryDatas.push_back(x);
    }
    void next( int size, std::shared_ptr<vector<uint8_t> >& v, int& offset )
    {
        if( offsetInCurrentBinary + size > (int)binaryDatas[currentBinary]->size() )
            throw std::runtime_error("Buffer underflow");
        v = binaryDatas[currentBinary];
        offset = offsetInCurrentBinary;
        offsetInCurrentBinary += size;
        if( offsetInCurrentBinary == (int)binaryDatas[currentBinary]->size() ){
            ++currentBinary;
            offsetInCurrentBinary = 0;
        }
    }
};

Buffer::Buffer(std::shared_ptr<std::vector<uint8_t> >v, size_t offset, size_t len){
    this->backingStore = v;
    this->offset = offset;
    this->byteLength=len;
}

Buffer::Buffer(string fname){
    ifstream in(fname,ios::binary);
    if(!in.good())
        throw std::runtime_error("Cannot load "+fname);
    in.seekg(0,ios::end);
    this->backingStore = std::make_shared<std::vector<uint8_t> >(in.tellg());
    in.seekg(0);
    in.read( (char*) backingStore->data(), backingStore->size() );
    this->offset = 0;
    this->byteLength=backingStore->size();
}

const uint8_t* Buffer::ptr(){
    return backingStore->data()+offset;
}

vector<shared_ptr<Buffer>> parseBuffers(const Json& J, Binaries& bin)
{
    auto& L = J["buffers"].array_items();
    vector<std::shared_ptr<Buffer>> v;
    for(unsigned i=0;i<L.size();++i){
        if( L[i].hasKey("uri") ){
            string fname = L[i]["uri"];
            v.push_back(std::make_shared<Buffer>( fname ) );
        } else {
            int byteLength = L[i]["byteLength"];
            int offset2;
            std::shared_ptr<vector<uint8_t> > backingStore;
            bin.next(byteLength,backingStore,offset2);
            v.push_back( std::make_shared<Buffer>( backingStore, offset2, byteLength ) );
        }
    }
    return v;
}

BufferView::BufferView( Json& vinfo, vector<std::shared_ptr<Buffer> >& buffers)
{
    int bufferIndex = vinfo["buffer"];
    this->buffer = buffers[bufferIndex];
    this->byteOffset = vinfo.get("byteOffset",0);
    this->length = (size_t)(vinfo["byteLength"].int_value());
    this->stride = (size_t)(vinfo.get("byteStride",0));
}

//view of entire buffer
BufferView::BufferView( std::shared_ptr<Buffer> buffer ){
    this->buffer = buffer;
    byteOffset = 0;
    stride = 0;
    length = buffer->byteLength;
}

const uint8_t* BufferView::ptr(){
    return buffer->ptr() + byteOffset;
}

vector<std::shared_ptr<BufferView> > parseBufferViews(const Json& J, vector<std::shared_ptr<Buffer> >& buffers)
{
    vector<std::shared_ptr<BufferView> > v;
    auto views = J["bufferViews"].array_items();
    for(unsigned i=0;i<views.size();++i){
        v.push_back(std::make_shared<BufferView>( views[i], buffers ) );
    }
    return v;
}


namespace {  
    //T1 = type of thing in memory (vecN, uint{8,16,32}_t
    //T2 = output type (vecN or uint32_t)
    template<typename T1, typename T2>
    int extract(Accessor* accessor, const T1* src, vector<T2>& result)
    {
        size_t incr;
        if( accessor->view->stride == 0 )
            incr = sizeof(T1);
        else
            incr = accessor->view->stride;
        
        const uint8_t* q = (const uint8_t*) src;
        for(int i=0;i<accessor->count;++i,q+=incr){
            const T1* tmp = (T1*)q;
            result.push_back( *tmp );
        }
        return accessor->count;
    }

};
 
Accessor::Accessor(const Json& ai, vector<std::shared_ptr<BufferView> >& views)
{
    int bufferViewIndex = ai["bufferView"];
    view = views[bufferViewIndex];
    byteOffset = ai.get("byteOffset",0);
    componentType = ai["componentType"];
    count = ai["count"];
    type = ai["type"].string_value();  
}
    
const uint8_t* Accessor::ptr() {
    return view->ptr() + byteOffset;
}
    
int Accessor::getData(std::vector<vec2>& v){
    assert( componentType == FLOAT );
    assert( type == "VEC2" );
    return extract(this,(vec2*)ptr(), v);
}
int Accessor::getData(std::vector<vec3>& v){
    assert( componentType == FLOAT );
    assert( type == "VEC3" );
    return extract(this,(vec3*)ptr(), v);
}
int Accessor::getData(std::vector<vec4>& v){
    assert( componentType == FLOAT );
    assert( type == "VEC4" );
    return extract(this,(vec4*)ptr(), v);
}
int Accessor::getData(std::vector<uint32_t>& v){
     assert( type == "SCALAR" );
     switch(componentType){
         case UBYTE:
            return extract(this,(uint8_t*) ptr(), v );
            break;
         case USHORT:
            return extract(this,(uint16_t*) ptr(), v );
            break;
         case UINT:
            return extract(this,(uint32_t*) ptr(), v );
            break;
        default:
            throw std::runtime_error("Not a scalar type?");
    }
}

namespace {
    vector<std::shared_ptr<Accessor> > parseAccessors(const Json& J, vector<std::shared_ptr<BufferView> >& views)
    {
        vector<std::shared_ptr<Accessor> > A;
        auto L = J["accessors"].array_items();
        for(unsigned i=0;i<L.size();++i){
            A.push_back(std::make_shared<Accessor>(L[i],views));
        }
        return A;
    }
}

Image::Image(const Json& J, vector<std::shared_ptr<BufferView>>& views){
   name = J.get("name","");
   if( J.hasKey("bufferView" )){
       int i = J["bufferView"];
       view = views[i];
   } else if( J.hasKey("uri")){
       std::string fname = J["uri"].string_value();
       auto buff = std::make_shared<Buffer>(fname);
       view = std::make_shared<BufferView>(buff);
       mimeType = J["mimeType"].string_value();
   } else {
       throw std::runtime_error("Bad image specification");
   }
}
    
Image::Image(std::shared_ptr<BufferView> view){
    this->view = view;
}

vector<std::shared_ptr<Image> > parseImages(const Json& J, vector<std::shared_ptr<BufferView> >& views)
{
    vector<std::shared_ptr<Image> > v;
    if( !J.hasKey("images") )
        return v;
    auto L = J["images"].array_items();
    for(unsigned i=0;i<L.size();++i){
        v.push_back(std::make_shared<Image>( L[i], views ) );
    }
    return v;
}

TextureData::TextureData(const Json& J, vector<std::shared_ptr<Image> > images)
{
    int ii = J["source"];
    source = images[ ii ];
    name = J.get("name","");
}
    
TextureData::TextureData(std::string name, std::shared_ptr<Image> src)
{
    this->name=name;
    this->source=src;
}

vector<std::shared_ptr<TextureData> > parseTextures(const Json& J,vector<std::shared_ptr<Image> > images){
    vector<std::shared_ptr<TextureData> > v;
    if(!J.hasKey("textures"))
        return v;
    auto L = J["textures"].array_items();
    for(unsigned i=0;i<L.size();++i){
        v.push_back(std::make_shared<TextureData>(L[i],images));
    }
    return v;
}

namespace {
    void setTexture( const Json& P, const char* key, 
            std::vector<std::shared_ptr<TextureData> > & textures, 
            std::shared_ptr<TextureData>& texture, int& tc)
    {
        if( P.hasKey(key)){
            int idx = P[key]["index"];
            texture = textures[ idx ];
            if( P[key].hasKey("texCoord") )
                tc = P[key]["texCoord"];
        }
    }
};

Material::Material(const Json& J, vector<std::shared_ptr<TextureData> >& textures)
{
    name = J.get("name","");
    if( J.hasKey("pbrMetallicRoughness" )){
        auto P = J["pbrMetallicRoughness"];
        if( P.hasKey("baseColorFactor") ){
            for(int i=0;i<4;++i)
                baseColorFactor[i] = (float)P["baseColorFactor"][i].number_value();
        }
        setTexture( P, "baseColorTexture", textures, baseTexture, baseTextureIndex );
        setTexture( P, "metallicRoughnessTexture", textures, roughnessTexture, roughnessTextureIndex );
        
        metallicFactor = float(P.get("metallicFactor",1.0));
        roughnessFactor = float(P.get("roughnessFactor",1.0));
    }
    setTexture(J, "normalTexture", textures, normalTexture, normalTextureIndex );
    setTexture(J, "occlusionTexture", textures, occlusionTexture, occlusionTextureIndex );
    setTexture(J, "emissiveTexture", textures, emissiveTexture, emissiveTextureIndex );

    if( J.hasKey("emissiveFactor") ){
        for(int i=0;i<3;++i)
            emissiveFactor[i] = float(J["emissiveFactor"][i].number_value());
    }
}
    
Material::Material(std::string name)
{
    this->name = name;
}

vector<std::shared_ptr<Material> > parseMaterials(const Json& J, vector<std::shared_ptr<TextureData> >& textures)
{
    vector<std::shared_ptr<Material> > v;
    if(!J.hasKey("materials"))
        return v;
    auto& L = J["materials"].array_items();
    
    for(unsigned i=0;i<L.size();++i){
        v.push_back(std::make_shared<Material>( L[i], textures ) );
    }
    return v;
}
        
   
class DataBundle {
  public:
    vector<vec3> position;
    vector<vec3> normal;
    vector<vec2> baseTexCoord;
    vector<vec2> bumpTexCoord;
    vector<vec4> tangents;
    vector<uint32_t> indices;
};

Primitive::Primitive(const Json& J, const vector<std::shared_ptr<Accessor> >& accessors, vector<std::shared_ptr<Material> >& materials ){
    const Json& dict = J["attributes"];
    for(string key : dict ){
        int ai = dict[key];
        attributes[key] = accessors[ai];
    }
    int ii = J.get("indices",-1 );
    if( ii == -1 )
        indices = nullptr;
    else
        indices = accessors[ii];
    
    int mi = J.get("material",-1);
    if( mi == -1 )
        material = nullptr;
    else
        material = materials[mi];
        
    primType = J.get("mode",TRIANGLES);
}
    
namespace {
    void getTextureCoordinates( std::shared_ptr<Primitive> primitive, int expectedCount, bool isThereATextureAtAll, 
            int texcoordIndex, vector<vec2>& destination  )
    {
        vector<vec2> tmp;
        int nt = 0;
        
        //if no such texture exists, don't bother looking for tex coords
        if( isThereATextureAtAll ){
            if( texcoordIndex > 9 )
                throw std::runtime_error("Too many texture coordinates");

            string attributeName = "TEXCOORD_x";
            attributeName[9] = (char)('0' + texcoordIndex);
            
            if( primitive->attributes.find(attributeName) != primitive->attributes.end() )
                nt = primitive->attributes[attributeName]->getData( tmp );
        }
        
        assert(nt == 0 || nt == expectedCount );
       
        if( nt == 0 ){
            destination.resize(destination.size()+expectedCount , vec2(0,0) );
        } else {
            //FIXME: If we don't flip the images, we don't need to flip the texcoords...
            //But blender uses GL's texcoords, not GLTF's texcoords
            for(unsigned i=0;i<tmp.size();++i){
                tmp[i].y = 1.0f - tmp[i].y;
            }
            destination.insert( destination.end(), tmp.begin(), tmp.end() );
        }   
    }


    void addDataToBundle(std::shared_ptr<Primitive> primitive, DataBundle& D, const mat4& matrix, const mat3& inverseTranspose)
    {
        auto numV = D.position.size();
        
        auto& attributes = primitive->attributes;
        auto& material = primitive->material;
        
        auto i = D.position.size();
        int np = attributes["POSITION"]->getData( D.position );
        for( ; i<D.position.size();++i){
            D.position[i] = (vec4( D.position[i],1.0) * matrix ).xyz();
        }
        
        i = D.normal.size();
        int nn = 0;
        if( attributes.find("NORMAL") != attributes.end() )
            nn = attributes["NORMAL"]->getData( D.normal );
            
        assert( nn == 0 || nn == np );
        
        if( nn == 0 ){
            cout << "Warning: No normals\n";
            D.normal.resize( D.position.size(), vec3(0,1,0) );
        }

        assert( D.normal.size() == D.position.size() );
        
        for( ; i<D.normal.size();++i){
            D.normal[i] = D.normal[i] * inverseTranspose;
        }
        
        if( !material->baseTexture )
            throw std::runtime_error("No base texture");
            
        if( material->baseTextureIndex != material->emissiveTextureIndex && material->emissiveTexture)
            throw std::runtime_error("Base texcoord index != emissive texcoord index");

        if( material->baseTextureIndex != material->occlusionTextureIndex && material->occlusionTexture )
            throw std::runtime_error("Base texcoord index != occlusion texcoord index");
            
        if( material->baseTextureIndex != material->roughnessTextureIndex && material->roughnessTexture)
            throw std::runtime_error("Base texcoord index != roughness texcoord index");
        
       
        getTextureCoordinates( primitive, np, material->baseTexture != nullptr, material->baseTextureIndex,
                D.baseTexCoord );
                
        assert( D.position.size() == D.baseTexCoord.size() );
        
        getTextureCoordinates( primitive, np, material->normalTexture != nullptr, material->normalTextureIndex,
                D.bumpTexCoord );
 
        assert( D.position.size() == D.bumpTexCoord.size() );
        
        
        i = D.tangents.size();
        int nT=0;
        if( attributes.find("TANGENT") != attributes.end() )
            nT = attributes["TANGENT"]->getData( D.tangents );
        assert( nT == 0 || nT == np );
        if( nT == 0 ){
            cout << "Warning: No tangents\n";
            D.tangents.resize(D.position.size(), vec4(1,0,0,0) );
        }
        //FIXME: Should we look at w component here?
        for( ; i<D.tangents.size();++i){
            D.tangents[i].setXYZ(  D.tangents[i].xyz() * inverseTranspose );
        }
        vector<uint32_t> I;
        primitive->indices->getData(I);
        for(unsigned i=0;i<I.size();++i){
            I[i] += (uint32_t)numV;
        }
        D.indices.insert(D.indices.end(), I.begin(), I.end() );
    }
    
};

Mesh::Mesh(const Json& J1, vector<std::shared_ptr<Accessor> >& accessors, vector<std::shared_ptr<Material> >& materials){
    this->name = J1.get("name","");
    
    auto PL = J1["primitives"].array_items();
    for(auto& J: PL ){
        this->primitives.push_back( std::make_shared<Primitive>(J, accessors, materials ) );
    }
}

namespace {
    void addDataToBundle( shared_ptr<Mesh> mesh, const mat4& matrix, const mat3& inverseTranspose, map<std::shared_ptr<Material>,DataBundle> &M )
    {
        for(auto& p : mesh->primitives ){
            if( p->primType != TRIANGLES )
                continue;
            DataBundle& D = M[p->material];
            addDataToBundle( p, D, matrix, inverseTranspose );
        }
    }
};

vector<std::shared_ptr<Mesh> > parseMeshes(const Json& J, 
    vector<std::shared_ptr<Accessor> >& accessors, vector<std::shared_ptr<Material> >& materials)
{
    vector<std::shared_ptr<Mesh> > v;
    auto L = J["meshes"].array_items();
    for(unsigned i=0;i<L.size();++i){
        v.push_back(std::make_shared<Mesh>( L[i], accessors, materials ) );
    }
    return v;
}
 

Light::Light( vec3 color, string type ){
    this->color=color;
    this->type=type;
}    

namespace {
    vector<std::shared_ptr<Light> > parseLights(const Json& J )
    {
        vector<std::shared_ptr<Light> > R;
        if( J.hasKey("extensions" )){
            auto& E = J["extensions"];
            if( E.hasKey("KHR_lights_punctual" )){
                auto& L = E["KHR_lights_punctual"]["lights"].array_items();
                for(auto& x : L ){
                    auto& tmp = x["color"].array_items();
                    float r = float(tmp[0].number_value());
                    float g = float(tmp[1].number_value());
                    float b = float(tmp[2].number_value());
                    string ty = x["type"].string_value();
                    R.push_back( std::make_shared<Light>(vec3(r,g,b), ty ) );
                }
            }
        }
        return R;
    }
    
        
        
    void getMeshAndLights( std::shared_ptr<Node> N, map<std::shared_ptr<Material> ,DataBundle>& M, 
        vector<vec3>& lightPositions, mat4 cmatrix, mat4 cimatrix )
    {
        //cmatrix = cumulative matrix
        //cimatrix = inverse of cmatrix
        
        cmatrix = N->matrix * cmatrix;
        cimatrix = cimatrix * N->inverse;
        mat3 inverseTranspose = transpose(cimatrix.upper3x3());

        if( N->mesh ){
            addDataToBundle( N->mesh, cmatrix, inverseTranspose, M );
        }

        //either zero or one lights per node
        for(unsigned i=0;i<N->lights.size();++i){
            lightPositions.push_back( (vec4(0,0,0,1) * cmatrix).xyz()  );
            //light direction, if spotlight, is (0,0,-1,0) * cmatrix
        }
        
        for(auto& c : N->children ){
            getMeshAndLights( c, M, lightPositions, cmatrix, cimatrix );
        }
    }

}; //namespace
    
Node::Node(const Json& J, vector<std::shared_ptr<Mesh> >& meshes, vector<std::shared_ptr<Light> >& lights)
{
    name = J.get("name","");
    
    
    if( J.hasKey("mesh") )
        mesh = meshes[J["mesh"].int_value()];
    else
        mesh = nullptr;
    
    matrix = mat4::identity();
    inverse = mat4::identity();
    
    mat4 S = mat4::identity();
    mat4 R = mat4::identity();
    mat4 T = mat4::identity();
    
    if( J.hasKey("translation") ){
        auto lst = J["translation"].array_items();
        double tx = lst[0].number_value();
        double ty = lst[1].number_value();
        double tz = lst[2].number_value();
        T = translation( tx,ty,tz );
    }
    if( J.hasKey("rotation")){
        auto lst = J["rotation"].array_items();
        double qx = -lst[0].number_value();
        double qy = -lst[1].number_value();
        double qz = -lst[2].number_value();
        double qw = lst[3].number_value();
        
        //to rotate by angle t around (x,y,z): Create quaternion [x*s, y*s, z*s, c]
        //where s=sin(t) and c=cos(t)
        //qw = c
        //sin**2 + cos**2 = 1
        //so sin**2 = 1-cos**2 = 1-qw*qw
        //so s = sqrt( 1-qw*qw)
        //thus, we know axis of rotation
        
        //~ float c = qw;
        //~ float s = std::sqrt( 1.0 - qw*qw );
        //~ float x = qx/s;
        //~ float y = qy/s;
        //~ float z = qz/s;
        
        
        //https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
        R = transpose(
            mat4(    qw, qz,-qy, qx,
                    -qz, qw, qx, qy,
                     qy,-qx, qw, qz,
                    -qx,-qy,-qz, qw ) *
            mat4(    qw, qz,-qy,-qx,
                    -qz, qw, qx,-qy,
                     qy,-qx, qw,-qz,
                     qx, qy, qz, qw )
            );
        
    }
    if( J.hasKey("scale") ){
        auto lst = J["scale"].array_items();
        double sx = lst[0].number_value();
        double sy = lst[1].number_value();
        double sz = lst[2].number_value();
        S = scaling( sx,sy,sz ) ;
    }
    
    if( J.hasKey("matrix") ){
        auto jlst = J["matrix"].array_items();
        array<double,16> lst;
        for(unsigned i=0;i<16;++i)
            lst[i] = jlst[i];
        matrix = transpose( mat4(
            lst[0],lst[1],lst[2],lst[3],
            lst[4],lst[5],lst[6],lst[7],
            lst[8],lst[9],lst[10],lst[11],
            lst[12],lst[13],lst[14],lst[15]
        )) * matrix;
    } else {
        matrix = S*R*T;
    }
    
    inverse = ::inverse(matrix);
    
    if( J.hasKey("children" ) ){
        for(int i :  J["children"].array_items() ){
            childIndices.push_back( i );
        }
    }
    
    if( J.hasKey("extensions" ) ){
        auto& e = J["extensions"];
        if( e.hasKey("KHR_lights_punctual") ){
            //standard says only one light per node
            int lightIndex = e["KHR_lights_punctual"].int_value();
            this->lights.push_back( lights[lightIndex] );
        }
    }
}

void Node::setChildren(vector<std::shared_ptr<Node> >& nodes)
{
    for(auto i : childIndices )
        this->children.push_back( nodes[i] );
}

namespace {
        
    vector<std::shared_ptr<Node> > parseNodes(const Json& J, vector<std::shared_ptr<Mesh> >& meshes,
            vector<std::shared_ptr<Light> >& lights)
    {
        vector<std::shared_ptr<Node> > v;
        auto& L = J["nodes"].array_items();
        for(unsigned i=0;i<L.size();++i){
            v.push_back(std::make_shared<Node>( L[i], meshes, lights ) );
        }
        for(unsigned i=0;i<v.size();++i)
            v[i]->setChildren(v);
        return v;
    }
    
};

Scene::Scene(const Json& J, vector<std::shared_ptr<Node> >& nodes){
    if(J.hasKey("nodes")){
        for(int i : J["nodes"].array_items()){
            this->nodes.push_back( nodes[i] );
        }
    }
    name = J.get("name","");
}
 
namespace {
    typedef std::shared_ptr<Material> MPtr;
    typedef std::tuple< MPtr , unsigned , unsigned >  TupleType ;
    
    void getAllMeshData( shared_ptr<Scene> scene, vector<vec3>& positions,
        vector<vec3>& normals, vector<vec2>& texcoords,
        vector<vec2>& bumpmaptexcoords,
        vector<vec4>& tangents, vector<uint32_t>& indices,
        vector< TupleType >& minfo,
        vector<vec3>& lightPositions)
    {
        map<std::shared_ptr<Material> ,DataBundle> M;
        for(auto& n : scene->nodes ){
            getMeshAndLights(n,  M , lightPositions, mat4::identity(), mat4::identity() );
        }
        unsigned vcount = 0;
        unsigned icount = 0;
        for(auto& it : M ){
            std::shared_ptr<Material>  mtl = it.first;
            DataBundle& B = it.second;
            positions.insert( positions.end(), B.position.begin(), B.position.end() );
            normals.insert( normals.end(), B.normal.begin(), B.normal.end() );
            texcoords.insert( texcoords.end(), B.baseTexCoord.begin(), B.baseTexCoord.end() );
            bumpmaptexcoords.insert( bumpmaptexcoords.end(), B.bumpTexCoord.begin(), B.bumpTexCoord.end() );
            tangents.insert( tangents.end(), B.tangents.begin(), B.tangents.end() );
            for(auto I : B.indices ){
                indices.push_back( I+vcount );
            }
            tuple<std::shared_ptr<Material> , unsigned, unsigned> mi( mtl, (unsigned) icount, (unsigned) B.indices.size() );
            minfo.push_back(mi);
            vcount += unsigned(B.position.size());
            icount += unsigned(B.indices.size());
        }
    }
     
    vector<std::shared_ptr<Scene> > parseScenes(const Json& J, vector<std::shared_ptr<Node> >& nodes){
        vector<std::shared_ptr<Scene> > v;
        auto L = J["scenes"].array_items();
        for(unsigned i=0;i<L.size();++i){
            v.push_back(std::make_shared<Scene>(L[i],nodes));
        }
        return v;
    }

}; //namespace
    
void parseGLTFBinary( std::string infname, std::string& json, Binaries& binaries)
{
    std::ifstream in(infname, ios::binary );
    assert(in.good());
    char magic[4];
    in.read(magic,4);
    if( 0 != std::memcmp( magic,"glTF",4) )
        throw std::runtime_error( string("Not glTF binary file: got ")+magic[0]+magic[1]+magic[2]+magic[3]);
    uint32_t version, filesize;
    in.read( (char*)&version, 4 );
    in.read( (char*)&filesize, 4 );
    
    while(true){
        uint32_t chunksize;
        in.read( (char*)&chunksize, 4 );
        if( in.fail() )
            break;
        in.read( magic, 4 );
        if( 0 == std::memcmp( magic, "JSON" ,4 ) ){
            std::vector<char> D(chunksize);
            in.read( D.data(), D.size() );
            D.push_back(0);
            json = D.data();
        } else if( 0 == std::memcmp( magic, "BIN\0" ,4)){
            auto v = std::make_shared<vector<uint8_t> >(chunksize);
            in.read( (char*) (v->data()), v->size() );
            binaries.addBinary(v);
        } else {
            throw std::runtime_error("Unexpected chunk");
        }
    }
    return;
}

void parseGLTFText(std::string , std::string& , Binaries& )
{
    //FIXME: TODO: Finish this
    assert(0);
}

void load(std::string infname, 
        vector<vec3>& positions,
        vector<vec3>& normals, 
        vector<vec2>& texcoords,
        vector<vec2>& bumpmaptexcoords,
        vector<vec4>& tangents, vector<uint32_t>& indices,
        vector< tuple< std::shared_ptr<Material> , unsigned , unsigned > >& minfo,
        vector<vec3>& lightPositions)
{
    std::ifstream in(infname, std::ios::binary);
    if(!in.good())
        throw std::runtime_error("Cannot open file "+infname);
    std::string s;
    char magic[4];
    in.read(magic,4);
    
    std::string jsonData;
 
    Binaries binaries;
       
    if( 0 == std::memcmp( "glTF", magic,4 ) ){
        parseGLTFBinary(infname,jsonData,binaries);
    } else {
        parseGLTFText(infname,jsonData,binaries);
    }
    
    std::string err;
    auto J = json11::Json::parse(jsonData,err);
    if( !err.empty() )
        throw std::runtime_error(err);

   
    auto buffers = parseBuffers( J, binaries );
    auto views = parseBufferViews(J,buffers);
    auto accessors = parseAccessors( J, views);
    auto images = parseImages(J,views);
    auto textures = parseTextures(J,images);
    auto materials = parseMaterials(J,textures);
    auto meshes = parseMeshes(J,accessors,materials);
    auto lights = parseLights(J);
    auto nodes = parseNodes(J,meshes,lights);
    auto scenes = parseScenes(J,nodes);
    
        
    if( scenes.size() > 1 ){
        std::cout << "Warning: More than one scene; only processing first one\n";
    }
    auto scene = scenes[0];
    getAllMeshData(scene, positions, normals, texcoords, bumpmaptexcoords,
        tangents, indices, minfo, lightPositions );
    assert( !minfo.empty() );
}
  
  
  
std::shared_ptr<Scene> loadScene(std::string infname)
{
    std::ifstream in(infname, std::ios::binary);
    if(!in.good())
        throw std::runtime_error("Cannot open file "+infname);
    std::string s;
    char magic[4];
    in.read(magic,4);
    
    std::string jsonData;
 
    Binaries binaries;
       
    if( 0 == std::memcmp( "glTF", magic,4 ) ){
        parseGLTFBinary(infname,jsonData,binaries);
    } else {
        parseGLTFText(infname,jsonData,binaries);
    }
    
    std::string err;
    auto J = json11::Json::parse(jsonData,err);
    if( !err.empty() )
        throw std::runtime_error(err);

   
    auto buffers = parseBuffers( J, binaries );
    auto views = parseBufferViews(J,buffers);
    auto accessors = parseAccessors( J, views);
    auto images = parseImages(J,views);
    auto textures = parseTextures(J,images);
    auto materials = parseMaterials(J,textures);
    auto meshes = parseMeshes(J,accessors,materials);
    auto lights = parseLights(J);
    auto nodes = parseNodes(J,meshes,lights);
    auto scenes = parseScenes(J,nodes);
    
        
    if( scenes.size() > 1 ){
        std::cout << "Warning: More than one scene; only processing first one\n";
    }
    return scenes[0];
}
  
  
  
       
}; //namespace gltf

