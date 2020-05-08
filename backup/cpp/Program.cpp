#include <stdafx.h>
#include "Program.h"

#include "glfuncs.h"
#include "utils.h"
#include "math3d.h"
#include "Buffer.h"
#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <memory>
#include <array>
#include <cmath>
#include <cassert>
#include <variant>

using namespace std;



namespace {  
    Buffer* uboBuffer = nullptr;
    std::vector<uint8_t> uboBackingMemory;
    bool bufferDirty = true;
};


class Program::UniformSetter{
  public:
    std::string name;
    unsigned offset,numBytes;
    unsigned location;
    GLenum type;
    unsigned arraySize;
    
    UniformSetter( std::string name, unsigned offset, unsigned location, unsigned numBytes, GLenum type, unsigned arraySize){
        this->name = name;
        this->offset = offset;
        this->location = location;
        this->numBytes = numBytes;
        this->type = type;
        this->arraySize = arraySize;
        assert( location == (unsigned) -1  || offset == (unsigned) -1 );
        assert( location != (unsigned) -1 || offset != (unsigned) -1 );
    }
    virtual std::string getTypeName() = 0;
    void set(double f){ 
        set( (float) f );
    }
    virtual void set(float ){ 
        throw std::runtime_error("Bad type when setting uniform "+name+": got float; expected "+getTypeName() );
    }
    virtual void set(std::int32_t ){ 
        throw std::runtime_error("Bad type when setting uniform "+name+": got int; expected "+getTypeName() );
    }
    virtual void set(std::uint32_t ){ 
        throw std::runtime_error("Bad type when setting uniform "+name+": got uint; expected "+getTypeName() );
    }
    virtual void set(const vec2& ){ 
        throw std::runtime_error("Bad type when setting uniform "+name+": got vec2; expected "+getTypeName() );
    }
    virtual void set(const vec3& ){ 
        throw std::runtime_error("Bad type when setting uniform "+name+": got vec3; expected "+getTypeName() );
    }
    virtual void set(const vec4& ){ 
        throw std::runtime_error("Bad type when setting uniform "+name+": got vec4; expected "+getTypeName() );
    }
    virtual void set(const std::vector<float>& ){ 
        throw std::runtime_error("Bad type when setting uniform "+name+": got array of float; expected "+getTypeName() );
    }
    virtual void set(const std::vector<vec2>& ){ 
        throw std::runtime_error("Bad type when setting uniform "+name+": got array of vec4; expected "+getTypeName() );
    }
    virtual void set(const std::vector<vec3>& ){ 
        throw std::runtime_error("Bad type when setting uniform "+name+": got array of vec4; expected "+getTypeName() );
    }
    virtual void set(const std::vector<vec4>& ){ 
        throw std::runtime_error("Bad type when setting uniform "+name+": got array of vec4; expected "+getTypeName() );
    }
    virtual void set(const mat2& ){ 
        throw std::runtime_error("Bad type when setting uniform "+name+": got mat2; expected "+getTypeName() );
    }
    virtual void set(const mat3& ){ 
        throw std::runtime_error("Bad type when setting uniform "+name+": got mat3; expected "+getTypeName() );
    }
    virtual void set(const mat4& ){ 
        throw std::runtime_error("Bad type when setting uniform "+name+": got mat4; expected "+getTypeName() );
    }
    virtual void set(const std::vector<mat2>&) {
        throw std::runtime_error("Bad type when setting uniform " + name + ": got array of mat2; expected " + getTypeName());
    }
    virtual void set(const std::vector<mat3>&) {
        throw std::runtime_error("Bad type when setting uniform " + name + ": got array of mat3; expected " + getTypeName());
    }
    virtual void set(const std::vector<mat4>&) {
        throw std::runtime_error("Bad type when setting uniform " + name + ": got array of mat4; expected " + getTypeName());
    }

    void doSet(const void* p, size_t count){
        if( this->numBytes != count ){
            throw std::runtime_error("Expected "+std::to_string(this->numBytes)+" but got "+std::to_string(count)+" for "+name);
        }
        void* dst = uboBackingMemory.data() + offset;
        memmove( dst, p, numBytes );
        bufferDirty = true;
    }
};


namespace {

    class FloatSetter : public Program::UniformSetter{
      public:
        FloatSetter( std::string name, unsigned offset, unsigned location, GLenum type, unsigned size):
            UniformSetter( name,offset,location,4,type,size)
        {
        }

        virtual std::string getTypeName() override {
            return "float";
        }
        virtual void set(float f) override { 
            if( location != (unsigned) -1 )
                glUniform1f( location, f );
            else
                doSet( &f, 4 );
        }
        virtual void set(std::int32_t f) override { 
            set((float)f);
        }
        virtual void set(std::uint32_t f) override { 
            set((float)f);
        }
    };

    class IntSetter : public Program::UniformSetter{
      public:
        IntSetter( std::string name, unsigned offset, unsigned location,   GLenum type, unsigned size):
            UniformSetter( name,offset,location,4,type,size)
        {
        }
        virtual std::string getTypeName() override {
            return "int";
        }
        virtual void set(float f) override { 
            set( (std::int32_t) f );
        }
        virtual void set(std::int32_t f) override { 
            if( location != (unsigned) -1 )
                glUniform1i( location, f );
            else
                doSet( &f, 4 );
        }
        virtual void set(std::uint32_t f) override { 
            set((std::int32_t)f);
        }
    };

    class UintSetter : public Program::UniformSetter{
      public:
        UintSetter( std::string name, unsigned offset, unsigned location,   GLenum type, unsigned size):
            UniformSetter( name,offset,location,4,type,size)
        {
        }
        virtual std::string getTypeName() override {
            return "uint";
        }
        virtual void set(float f) override { 
            set( (std::uint32_t) f );
        }
        virtual void set(std::int32_t f) override { 
            set((std::uint32_t)f);
        }
        virtual void set(std::uint32_t f) override { 
           if( location != (unsigned) -1 )
                glUniform1ui( location, f );
            else
                doSet( &f, 4 );
        }
    };

    class Vec2Setter : public Program::UniformSetter{
      public:
        Vec2Setter( std::string name, unsigned offset, unsigned location,   GLenum type, unsigned size):
            UniformSetter( name,offset,location,8,type,size) 
        {
        }
        virtual std::string getTypeName() override {
            return "vec2";
        }
        virtual void set(const vec2& v) override {
           if( location != (unsigned) -1 )
                glUniform2fv( location, 1, (float*) &v );
            else
                doSet( &v, sizeof(v) );
        }
    };

    class Vec3Setter : public Program::UniformSetter{
      public:
        Vec3Setter( std::string name, unsigned offset,  unsigned location, GLenum type, unsigned size):
            UniformSetter( name,offset,location,12,type,size)    
        {
        }
        virtual std::string getTypeName() override {
            return "vec3";
        }
        virtual void set(const vec3& v) override {
           if( location != (unsigned) -1 )
                glUniform3fv( location, 1, (float*) &v );
            else
                doSet( &v, sizeof(v) );
        }
    };

    class Vec4Setter : public Program::UniformSetter{
      public:
        Vec4Setter( std::string name, unsigned offset,  unsigned location,  GLenum type, unsigned size):
            UniformSetter( name,offset,location,16,type,size)    
        {
        }
        virtual std::string getTypeName() override {
            return "vec4";
        }
        virtual void set(const vec4& v) override {
           if( location != (unsigned) -1 )
                glUniform4fv( location, 1, (float*)  &v );
            else
                doSet( &v, sizeof(v) );
        }
    };

    class Mat2Setter : public Program::UniformSetter{
      public:
        Mat2Setter( std::string name, unsigned offset, unsigned location, GLenum type, unsigned size):
            UniformSetter( name,offset,location,32,type,size)    //2*16 = 32 because one vec4 for 2 rows
        {
        }
        virtual std::string getTypeName() override {
            return "mat2";
        }
        virtual void set(const mat2& v) override {
           if( location != (unsigned) -1 )
                glUniformMatrix2fv( location, 1, true, v.data().data() );
            else
                doSet( &v, sizeof(v) );
        }
    };

    class Mat3Setter : public Program::UniformSetter{
      public:
        Mat3Setter( std::string name, unsigned offset,  unsigned location,   GLenum type, unsigned size):
            UniformSetter( name,offset,location,48,type,size)    //3*16 = 48 because one vec4 for 3 rows
        {
        }
        virtual std::string getTypeName() override {
            return "mat3";
        }
        virtual void set(const mat3& v) override {
           if( location != (unsigned) -1 )
                glUniformMatrix3fv( location, 1, true, v.data().data() );
            else
                doSet( &v, sizeof(v) );
        }
    };

    class Mat4Setter : public Program::UniformSetter{
      public:
        Mat4Setter( std::string name, unsigned offset, unsigned location,  GLenum type, unsigned size):
            UniformSetter( name,offset,location,64,type,size)       // 4*16 = 64 because one vec4 for 4 rows 
        {
        }
        virtual std::string getTypeName() override {
            return "mat4";
        }
        virtual void set(const mat4& v) override {
           if( location != (unsigned) -1 )
                glUniformMatrix4fv( location, 1, true, v.data().data() );
            else
                doSet( &v, sizeof(v) );
        }
    };

    class Mat4ArraySetter : public Program::UniformSetter {
    public:
        Mat4ArraySetter(std::string name, unsigned offset, unsigned location, GLenum type, unsigned size) :
            UniformSetter(name, offset, location, size * 64, type, size) // 4 vec4 per row *  size of array
        {
        }
        virtual std::string getTypeName() override {
            return "array of mat4";
        }
        virtual void set(const std::vector<mat4>& v) override {
            if (v.size() != arraySize) {
                throw std::runtime_error("Array size mismatch: Got " + std::to_string(v.size()) + " but shader expects " +
                    std::to_string(arraySize));
            }
            if (location != (unsigned)-1)
                glUniformMatrix4fv(location, arraySize, true, (float*)v.data());
            else
                doSet(v.data(), v.size() * sizeof(v[0]));
        }
    };

    class Mat3ArraySetter : public Program::UniformSetter {
    public:
        Mat3ArraySetter(std::string name, unsigned offset, unsigned location, GLenum type, unsigned size) :
            UniformSetter(name, offset, location, size * 48, type, size) // 3 vec3 per row *  size of array contains padding
        {
        }
        virtual std::string getTypeName() override {
            return "array of mat3";
        }
        virtual void set(const std::vector<mat3>& v) override {
            if (v.size() != arraySize) {
                throw std::runtime_error("Array size mismatch: Got " + std::to_string(v.size()) + " but shader expects " +
                    std::to_string(arraySize));
            }
            if (location != (unsigned)-1)
                glUniformMatrix3fv(location, arraySize, true, (float*)v.data());
            else
                doSet(v.data(), v.size() * sizeof(v[0]));
        }
    };

    class Mat2ArraySetter : public Program::UniformSetter {
    public:
        Mat2ArraySetter(std::string name, unsigned offset, unsigned location, GLenum type, unsigned size) :
            UniformSetter(name, offset, location, size*32, type, size) // 2 vec2 per row *  size of array
        {
        }
        virtual std::string getTypeName() override {
            return "array of mat2";
        }
        virtual void set(const std::vector<mat2>& v) override {
            if (v.size() != arraySize) {
                throw std::runtime_error("Array size mismatch: Got " + std::to_string(v.size()) + " but shader expects " +
                    std::to_string(arraySize));
            }
            if (location != (unsigned)-1)
                glUniformMatrix2fv(location, arraySize, true, (float*) v.data());
            else
                doSet(v.data(), v.size() * sizeof(v[0]));
        }
    };

    class Vec4ArraySetter : public Program::UniformSetter{
      public:
        Vec4ArraySetter( std::string name, unsigned offset, unsigned location,  GLenum type, unsigned size):
            UniformSetter( name,offset,location,size*16,type,size)    
        {
        }
        virtual std::string getTypeName() override {
            return "array of vec4";
        }
        virtual void set(const std::vector<vec4>& v) override {
            if( v.size() != arraySize ){
                throw std::runtime_error("Array size mismatch: Got "+std::to_string(v.size())+" but shader expects "+
                    std::to_string(arraySize));
            }
            if( location != (unsigned) -1 )
                glUniform4fv( location, arraySize, (float*) v.data() );
            else
                doSet( v.data(), v.size()*sizeof(v[0]) );
        }
    };
    
    class Vec3ArraySetter : public Program::UniformSetter{
      public:
        Vec3ArraySetter( std::string name, unsigned offset, unsigned location,   GLenum type, unsigned size):
            UniformSetter( name,offset,location,size*16,type,size)      //16 because arrays are padded 
        {
        }
        virtual std::string getTypeName() override {
            return "array of vec3";
        }
        virtual void set(const std::vector<vec3>& v) override {
            if( v.size() != arraySize ){
                throw std::runtime_error("Array size mismatch: Got "+std::to_string(v.size())+" but shader expects "+
                    std::to_string(arraySize));
            }
            if( location != (unsigned) -1 )
                glUniform3fv( location, arraySize, (float*) v.data() );
            else{
                //arrays of uniforms are always sent as vec4's
                std::vector<vec4> tmp(arraySize);
                for(unsigned i=0;i<arraySize;++i){
                    tmp[i].x = v[i].x;
                    tmp[i].y = v[i].y;
                    tmp[i].z = v[i].z;
                }
                doSet( tmp.data(), tmp.size()*sizeof(tmp[0]) );        }
            }
    };
    
    class Vec2ArraySetter : public Program::UniformSetter{
      public:
        Vec2ArraySetter( std::string name, unsigned offset, unsigned location, GLenum type, unsigned size):
            UniformSetter( name,offset,location,size*16,type,size)    //16 because arrays are padded 
        {
        }
        virtual std::string getTypeName() override {
            return "array of vec2";
        }
        virtual void set(const std::vector<vec2>& v) override {
            if( v.size() != arraySize ){
                throw std::runtime_error("Array size mismatch: Got "+std::to_string(v.size())+" but shader expects "+
                    std::to_string(arraySize));
            }
            if( location != (unsigned) -1 )
                glUniform2fv( location, arraySize, (float*) v.data() );
            else{
                //arrays of uniforms are always sent as vec4's
                std::vector<vec4> tmp(arraySize);
                for(unsigned i=0;i<arraySize;++i){
                    tmp[i].x = v[i].x;
                    tmp[i].y = v[i].y;
                }
                doSet( tmp.data(), tmp.size()*sizeof(tmp[0]) );        }
            }
    };
    
    class FloatArraySetter : public Program::UniformSetter{
      public:
        FloatArraySetter( std::string name, unsigned offset, unsigned location, GLenum type, unsigned size):
            UniformSetter( name,offset,location,size*16,type,size)    
        {
        }
        virtual std::string getTypeName() override {
            return "array of floats";
        }
        virtual void set(const std::vector<float>& v) override {
            if( v.size() != arraySize ){
                throw std::runtime_error("Array size mismatch: Got "+std::to_string(v.size())+" but shader expects "+
                    std::to_string(arraySize));
            }
            if( location != (unsigned) -1 ){
                glUniform1fv( location, arraySize,  v.data() );
            } else{
                //arrays of uniforms are always sent as vec4's
                std::vector<vec4> tmp(arraySize);
                for(unsigned i=0;i<arraySize;++i)
                    tmp[i].x = v[i];
                doSet( tmp.data(), tmp.size()*sizeof(tmp[0]) );
            }
        }
    };



 
    template<typename T>
    void getLog( std::string msg, const T& func, GLuint identifier )
    {
        std::vector<char> blob(4096);
        GLint tmp[1];
        func( identifier, (unsigned)blob.size(), tmp, blob.data());
        auto length = tmp[0];
        if(length > 0){
            std::cout << msg << "\n";
            std::string str(blob.data(),length);
            std::cout << str << "\n";
        }
    }

    string readFile(string fname)
    {
        if( fname.empty() )
            return "";
            
        std::string shaderdata;
        fname = "shaders/"+fname;
        std::ifstream in( fname );
        if(!in.good() )
            throw std::runtime_error("Cannot open "+fname);
        std::getline(in,shaderdata,'\0');
      
        auto verpos = shaderdata.find("#version");
        if( verpos == std::string::npos ){
            shaderdata = "#version 430\n" + shaderdata;
            verpos = 0;
        }
        
        auto lineEnd = shaderdata.find("\n",verpos);
        if( lineEnd == std::string::npos){
            throw std::runtime_error("?");
        }
        
        std::string uniformdata;
        std::ifstream in2("shaders/uniforms.txt");
        if( !in2.good() ){
            //no uniforms?
            throw std::runtime_error("Cannot find shaders/uniforms.txt");
        } else {
            std::getline(in2,uniformdata,'\0');
        }

        shaderdata = shaderdata.substr(0,lineEnd+1) + "\n//begin uniform data\n#line 1\n" + uniformdata + "\n//end uniform data\n#line 1\n" + shaderdata.substr(lineEnd+1);
                 
        if( shaderdata.empty() )
            throw std::runtime_error("Empty file: "+fname );
            
        return shaderdata;
    }
        
          
    void compile( std::string shadername, std::string shaderdata, GLenum shaderType, GLint prog)
    {

        if( shaderdata.empty() )
            throw std::runtime_error("Empty shader!");
            
        if(0)
            std::cout << shaderdata << "\n";
 
        GLuint s = glCreateShader(shaderType);
        const char* X[1];
        X[0] = shaderdata.c_str();
        glShaderSource(s, 1, X, nullptr );
        glCompileShader(s);
        getLog( "Compiling "+shadername+": ", glGetShaderInfoLog, s);
        GLint tmp[1];
        glGetShaderiv( s, GL_COMPILE_STATUS, tmp );
        if(!tmp[0] )
            throw std::runtime_error("Cannot compile!");
        glAttachShader(prog,s);
        glDeleteShader(s);
    }
    





    unsigned queryUniforms( GLuint prog, std::map<std::string,Program::UniformSetter*>& uniforms, bool privateOnly)
    {
        GLint tmp[1];
        glGetProgramiv(prog,GL_ACTIVE_UNIFORMS,tmp);
        GLint numuniforms = tmp[0];
        std::vector<GLuint> uniformsToQuery(numuniforms);
        for(GLint i=0;i<numuniforms;++i)
            uniformsToQuery[i]=i;
        std::vector<GLint> offsets(numuniforms);
        std::vector<GLint> sizes(numuniforms);
        std::vector<GLint> types(numuniforms);
        glGetActiveUniformsiv(prog, numuniforms,
            uniformsToQuery.data(), GL_UNIFORM_OFFSET, offsets.data());
        glGetActiveUniformsiv(prog, numuniforms,
            uniformsToQuery.data(), GL_UNIFORM_SIZE, sizes.data());
        glGetActiveUniformsiv(prog, numuniforms,
            uniformsToQuery.data(), GL_UNIFORM_TYPE, types.data());

        std::vector<char> nameBytes(256);
        int totalUniformBytes = 0;
        for(int i=0;i<numuniforms;++i){
            
            bool isPrivate = (offsets[i] == GLint(-1) );
            if( privateOnly && !isPrivate )
                continue;
            if( !privateOnly && isPrivate )
                continue;
            
            glGetActiveUniformName(prog, i, (unsigned)nameBytes.size(),
                tmp, nameBytes.data());
            auto nameLen = tmp[0];
            std::string name(nameBytes.data(),nameLen);
            
            
            //std::cout << "Found " << (isPrivate ? "private " : "common ") << "uniform " << name << "\n";
            
            unsigned location;
            
            if( isPrivate ){
                location = glGetUniformLocation(prog,name.c_str());
            } else {
                location = (unsigned) -1;
            }
            
            Program::UniformSetter* u;
            if( sizes[i] == 1 ){
                switch(types[i]){
                    case GL_FLOAT_VEC4:         u = new Vec4Setter {name, (unsigned)offsets[i], location,  (unsigned)types[i], (unsigned)sizes[i] }; break;
                    case GL_FLOAT_VEC3:         u = new Vec3Setter {name, (unsigned)offsets[i], location,  (unsigned)types[i], (unsigned)sizes[i] }; break;
                    case GL_FLOAT_VEC2:         u = new Vec2Setter {name, (unsigned)offsets[i], location,  (unsigned)types[i], (unsigned)sizes[i] }; break;
                    case GL_FLOAT_MAT4:         u = new Mat4Setter {name, (unsigned)offsets[i], location,  (unsigned)types[i], (unsigned)sizes[i] }; break;
                    case GL_FLOAT_MAT3:         u = new Mat3Setter {name, (unsigned)offsets[i], location,  (unsigned)types[i], (unsigned)sizes[i] }; break;
                    case GL_FLOAT_MAT2:         u = new Mat2Setter {name, (unsigned)offsets[i], location,  (unsigned)types[i], (unsigned)sizes[i] }; break;
                    case GL_FLOAT:              u = new FloatSetter{name, (unsigned)offsets[i], location,  (unsigned)types[i], (unsigned)sizes[i] }; break;
                    case GL_INT:                u = new IntSetter  {name, (unsigned)offsets[i], location,  (unsigned)types[i], (unsigned)sizes[i] }; break;
                    case GL_UNSIGNED_INT:       u = new UintSetter {name, (unsigned)offsets[i], location,  (unsigned)types[i], (unsigned)sizes[i] }; break;
                    case GL_SAMPLER_1D:
                    case GL_SAMPLER_1D_ARRAY:
                    case GL_SAMPLER_2D:
                    case GL_SAMPLER_2D_ARRAY:
                    case GL_SAMPLER_3D:
                    case GL_SAMPLER_CUBE:
                    case GL_INT_SAMPLER_1D:
                    case GL_INT_SAMPLER_1D_ARRAY:
                    case GL_INT_SAMPLER_2D:
                    case GL_INT_SAMPLER_2D_ARRAY:
                    case GL_INT_SAMPLER_3D:
                    case GL_INT_SAMPLER_CUBE:
                    case GL_UNSIGNED_INT_SAMPLER_1D:
                    case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
                    case GL_UNSIGNED_INT_SAMPLER_2D:
                    case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
                    case GL_UNSIGNED_INT_SAMPLER_3D:
                    case GL_UNSIGNED_INT_SAMPLER_CUBE:
                    case GL_SAMPLER_1D_SHADOW:
                    case GL_SAMPLER_2D_SHADOW:
                    case GL_SAMPLER_2D_ARRAY_SHADOW:
                    case GL_SAMPLER_CUBE_SHADOW:
                    case GL_SAMPLER_BUFFER:
                    case GL_SAMPLER_2D_MULTISAMPLE:
                    case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
                    case GL_SAMPLER_CUBE_MAP_ARRAY:
                    case GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW:
                                                u = new IntSetter  {name, (unsigned)offsets[i], location,  (unsigned)types[i], (unsigned)sizes[i] }; break;
                    default:
                        throw std::runtime_error("Uniform "+name+": Unsupported type");
                }
            } else {
                switch(types[i]){
                    case GL_FLOAT_MAT4:  u = new Mat4ArraySetter{ name, (unsigned)offsets[i], location,  (unsigned)types[i], (unsigned)sizes[i] }; break;
                    case GL_FLOAT_MAT3:  u = new Mat3ArraySetter{ name, (unsigned)offsets[i], location,  (unsigned)types[i], (unsigned)sizes[i] }; break;
                    case GL_FLOAT_MAT2:  u = new Mat2ArraySetter  {name, (unsigned)offsets[i], location,  (unsigned)types[i], (unsigned)sizes[i] }; break;
                    case GL_FLOAT_VEC4:  u = new Vec4ArraySetter  {name, (unsigned)offsets[i], location,  (unsigned)types[i], (unsigned)sizes[i] }; break;
                    case GL_FLOAT_VEC3:  u = new Vec3ArraySetter  {name, (unsigned)offsets[i], location,  (unsigned)types[i], (unsigned)sizes[i] }; break;
                    case GL_FLOAT_VEC2:  u = new Vec2ArraySetter  {name, (unsigned)offsets[i], location,  (unsigned)types[i], (unsigned)sizes[i] }; break;
                    case GL_FLOAT:       u = new FloatArraySetter {name, (unsigned)offsets[i], location,  (unsigned)types[i], (unsigned)sizes[i] }; break;
                    default:
                        throw std::runtime_error("Uniform "+name+": Unsupported type (arrays must be vecN or float)");
                }
            }
            
            uniforms[name] = u;
            
            if( offsets[i] != -1 ){
                int end = offsets[i] + u->numBytes;
                if(end > totalUniformBytes){
                    totalUniformBytes = end;
                }
            }
        }
        
        {
            GLint rv[1];
            glGetProgramiv( prog, GL_ACTIVE_UNIFORM_BLOCKS, rv );
            if( rv[0] != 0 ){
                glGetActiveUniformBlockiv( prog, 0, GL_UNIFORM_BLOCK_DATA_SIZE, rv );
                return rv[0];
            } else
                return 0;
                
            //~ if( rv[0] != totalUniformBytes ){
                //~ std::cout << "Uniform block size mismatch: GL says it should be " << rv[0] << " but we calculated " << totalUniformBytes << "\n";
            //~ }
        }
        
        //~ return totalUniformBytes;
    }
    
        
       
    
    void autoUpdateUniforms( const char* , const std::vector<uint64_t>& ){
        Program::updateUniforms(); 
    }

    std::map<GLenum, int> sizeForType{ {
        {GL_FLOAT_VEC4, 4*4},
        {GL_FLOAT_VEC3, 3*4},
        {GL_FLOAT_VEC2, 2*4},
        {GL_FLOAT,      1*4},
        {GL_INT,        1*4},
        {GL_BOOL,       1},
        {GL_FLOAT_MAT2, 2*16},
        {GL_FLOAT_MAT3, 3*16},
        {GL_FLOAT_MAT4, 4*16}
    }};
    
    std::map<GLenum, std::string> M{ {
        {GL_FLOAT_VEC4, "GL_FLOAT_VEC4"},
        {GL_FLOAT_VEC3, "GL_FLOAT_VEC3"},
        {GL_FLOAT_VEC2, "GL_FLOAT_VEC2"},
        {GL_FLOAT,      "GL_FLOAT"},
        {GL_INT,        "GL_INT"},
        {GL_BOOL,       "GL_BOOL"},
        {GL_FLOAT_MAT2, "GL_FLOAT_MAT2"},
        {GL_FLOAT_MAT3, "GL_FLOAT_MAT3"},
        {GL_FLOAT_MAT4, "GL_FLOAT_MAT4"}
    }};
};   //namespace
         

    
Program::Program(std::string vsfname,  std::string fsfname) : 
        Program(vsfname,"","","",fsfname,"") {}

Program::Program(std::string vsfname, std::string gsfname, std::string fsfname) : 
        Program(vsfname, "", "", gsfname, fsfname,"")  {}

Program::Program(std::string vsfname, std::string tcsfname, std::string tesfname, std::string fsfname) :
    Program(vsfname,tcsfname,tesfname,"",fsfname,"") {}

Program::Program(std::string vsfname, std::string tcsfname, std::string tesfname, std::string gsfname, std::string fsfname):
    Program(vsfname,tcsfname,tesfname,gsfname,fsfname,"") {}

Program::Program(std::string csfname) :
    Program("","","","","",csfname) {}
            
Program::Program(std::string vsfname, std::string tcsfname, std::string tesfname, std::string gsfname, std::string fsfname, std::string csfname)
{
    std::array< std::pair<std::string,std::string>, 6 > v;
    v[0] = ( std::make_pair(vsfname, readFile(vsfname) ) );
    v[1] = ( std::make_pair(tcsfname, readFile(tcsfname) ) );
    v[2] = ( std::make_pair(tesfname, readFile(tesfname) ) );
    v[3] = ( std::make_pair(gsfname, readFile(gsfname) ) );
    v[4] = ( std::make_pair(fsfname, readFile(fsfname) ) );
    v[5] = ( std::make_pair(csfname, readFile(csfname) ) );
    init(true,v);
}

Program::Program( const std::array<std::pair<std::string,std::string>,6>& shaderData) 
{
    init(false,shaderData);
}

Program::~Program(){
    glDeleteProgram(this->prog);
}

//first = filename, second = data
void Program::init( bool fromFile, const std::array<std::pair<std::string,std::string>,6>& S)
{
    this->prog = glCreateProgram();
    
    GLenum shty[] = {GL_VERTEX_SHADER,GL_TESS_CONTROL_SHADER,GL_TESS_EVALUATION_SHADER,
            GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER, GL_COMPUTE_SHADER };

    for(int i=0;i<6;++i){
        if( !S[i].second.empty() ){
            if( !identifier.empty() )
                identifier += "+";
            identifier += S[i].first;
            compile(S[i].first, S[i].second, shty[i], this->prog) ;
        }
    }
   
    glLinkProgram(this->prog);
    getLog( "Linking "+identifier+":", glGetProgramInfoLog, this->prog);
    GLint tmp[1];
    glGetProgramiv( this->prog, GL_LINK_STATUS, tmp );
    if(! tmp[0] )
        throw std::runtime_error("Cannot link");
    this->setupPrivateUniforms();
    if( fromFile )
        setupCommonUniforms(this->prog);
}

void Program::dispatch(int xs, int ys, int zs){
    if( current != this )
        this->use();
    glDispatchCompute(xs,ys,zs);
}


void Program::setupCommonUniforms(GLuint prog)
{
    if( uboBuffer != nullptr )
        return;
    unsigned totalUniformBytes = queryUniforms( prog, commonUniforms, false );
    std::cout << __FILE__ << "(" << __LINE__ << "): " << totalUniformBytes << " bytes (=" << totalUniformBytes/4/4 << " vec4 slots) for common uniforms\n";
    uboBackingMemory.resize(totalUniformBytes);
    uboBuffer = new Buffer(uboBackingMemory.size(), GL_DYNAMIC_DRAW);
    uboBuffer->bindBase(GL_UNIFORM_BUFFER,0);
    gl::addDrawCallHook( autoUpdateUniforms );
}

void Program::setupPrivateUniforms()
{
    queryUniforms( this->prog, this->privateUniforms, true  );
}

Program::UniformSetter* Program::getSetter(std::string name){
    
    if( Program::current ){
        auto it = Program::current->privateUniforms.find(name);
        if( it != Program::current->privateUniforms.end() )
            return it->second;
    }
        
    auto it = Program::commonUniforms.find(name);
    if( it != Program::commonUniforms.end() )
        return it->second;
    
    if( Program::commonUniforms.count( name+"[0]" ) || 
        (Program::current && Program::current->privateUniforms.count( name+"[0]") )){
        string msg = "No such uniform "+name+" (maybe you meant '"+name+"[0]' ?)";
        cout << msg << "\n";
        throw std::runtime_error(msg);
    } else {
        string msg = "No such uniform "+name;
        cout << msg << "\n";
        throw std::runtime_error(msg);
    }
}

void Program::setUniform(std::string name, double value ){
    auto u = getSetter(name);
    u->set(value);
}
void Program::setUniform(std::string name, float value ){
    auto u = getSetter(name);
    u->set(value);
}
void Program::setUniform(std::string name, int value ){
    auto u = getSetter(name);
    u->set(value);
}
void Program::setUniform(std::string name, unsigned value ){
    auto u = getSetter(name);
    u->set(value);
}
void Program::setUniform(std::string name, const vec2& value ){
    auto u = getSetter(name);
    u->set(value);
}
void Program::setUniform(std::string name, const vec3& value ){
    auto u = getSetter(name);
    u->set(value);
}
void Program::setUniform(std::string name, const vec4& value ){
    auto u = getSetter(name);
    u->set(value);
}
void Program::setUniform(std::string name, const mat2& value ){
    auto u = getSetter(name);
    u->set(value);
}
void Program::setUniform(std::string name, const mat3& value ){
    auto u = getSetter(name);
    u->set(value);
}
void Program::setUniform(std::string name, const mat4& value ){
    auto u = getSetter(name);
    u->set(value);
}
void Program::setUniform(std::string name, const std::vector<mat4>& value) {
    auto u = getSetter(name);
    u->set(value);
}
void Program::setUniform(std::string name, const std::vector<mat3>& value) {
    auto u = getSetter(name);
    u->set(value);
}
void Program::setUniform(std::string name, const std::vector<mat2>& value) {
    auto u = getSetter(name);
    u->set(value);
}
void Program::setUniform(std::string name, const std::vector<vec4>& value ){
    auto u = getSetter(name);
    u->set(value);
}
void Program::setUniform(std::string name, const std::vector<vec3>& value ){
    auto u = getSetter(name);
    u->set(value);
}
void Program::setUniform(std::string name, const std::vector<vec2>& value ){
    auto u = getSetter(name);
    u->set(value);
}
void Program::setUniform(std::string name, const std::vector<float>& value ){
    auto u = getSetter(name);
    u->set(value);
}
    
void Program::updateUniforms(){
    if( bufferDirty ){
        uboBuffer->bindBase(GL_UNIFORM_BUFFER, 0);
        glBufferSubData(GL_UNIFORM_BUFFER,0,
            uboBackingMemory.size(),
            uboBackingMemory.data()
        );
        bufferDirty = false;
    }
}

void Program::use(){
//    Program::setupCommonUniforms(this->prog);
    glUseProgram(this->prog);
    Program::current = this;
}

std::map<std::string,Program::UniformSetter* > Program::commonUniforms;
Program* Program::current;
