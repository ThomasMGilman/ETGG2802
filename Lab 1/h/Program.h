#pragma once

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

class Program{

 
  public:
    
    static Program* current;

    Program(std::string vsfname,  std::string fsfname);
    
    Program(std::string vsfname, std::string gsfname, std::string fsfname) ;
    
    Program(std::string vsfname, std::string tcsfname, std::string tesfname, std::string fsfname) ;
                
    Program(std::string vsfname, std::string tcsfname, std::string tesfname,
            std::string gsfname, std::string fsfname);

    Program(std::string csfname);
    
    Program(std::string vsfname, std::string tcsfname, std::string tesfname,
            std::string gsfname, std::string fsfname, std::string csfname);
   
    Program(const std::array<std::pair<std::string,std::string>,6>& shaderData);
 
    ~Program();
    
    void dispatch(int xs, int ys, int zs);
    
    static void setUniform(std::string name, double value );
    static void setUniform(std::string name, float value );
    static void setUniform(std::string name, int value );
    static void setUniform(std::string name, unsigned value );
    static void setUniform(std::string name, const vec2& value );
    static void setUniform(std::string name, const vec3& value );
    static void setUniform(std::string name, const vec4& value );
    static void setUniform(std::string name, const mat2& value );
    static void setUniform(std::string name, const mat3& value );
    static void setUniform(std::string name, const mat4& value );
    static void setUniform(std::string name, const std::vector<vec4>& value );
    static void setUniform(std::string name, const std::vector<vec3>& value );
    static void setUniform(std::string name, const std::vector<vec2>& value );
    static void setUniform(std::string name, const std::vector<float>& value );
    static void updateUniforms();
    void use();


    class UniformSetter;

  private:
        
    GLuint prog;
    std::string identifier;

    Program(const Program&) = delete;
    void operator=(const Program&) = delete;

    friend class UniformSetter;
    static UniformSetter* getSetter(std::string name);
    
    //key = uniform name; value = info about that uniform
    static std::map<std::string,UniformSetter*> commonUniforms;
    std::map<std::string,UniformSetter*> privateUniforms;
    
    void init( bool fromFile, const std::array<std::pair<std::string,std::string>,6>& S);
    static void setupCommonUniforms(GLuint prog);
    void setupPrivateUniforms();

};
             
