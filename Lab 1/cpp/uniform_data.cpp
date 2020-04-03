#include <stdafx.h>
#include <uniform_data.h>

uniformData::uniformData(std::string name, const std::type_info* dType, dataVariant data, bool manuallySetUniform)
{
    uniformName = name;
    dataType = dType;
    d = data;
    setManually = manuallySetUniform;
}

void uniformData::set_uniform()
{
    if (dataType->hash_code() == typeid(int).hash_code())
    {
        int val = getVal<int>();
        Program::setUniform(uniformName, val);
    }
    if (dataType->hash_code() == typeid(float).hash_code())
    {
        float val = getVal<float>();
        Program::setUniform(uniformName, val);
    }
    if (dataType->hash_code() == typeid(double).hash_code())
    {
        double val = getVal<double>();
        Program::setUniform(uniformName, val);
    }
    if (dataType->hash_code() == typeid(vec2).hash_code())
    {
        vec2 val = getVal<vec2>();
        Program::setUniform(uniformName, val);
    }
    if (dataType->hash_code() == typeid(vec3).hash_code())
    {
        vec3 val = getVal<vec3>();
        Program::setUniform(uniformName, val);
    }
    if (dataType->hash_code() == typeid(vec4).hash_code())
    {
        vec4 val = getVal<vec4>();
        Program::setUniform(uniformName, val);
    }
}

void uniformData::debug_print()
{
    std::cout << uniformName << ": ";
    if (dataType->hash_code() == typeid(int).hash_code())
        std::cout << getVal<int>();
    if (dataType->hash_code() == typeid(float).hash_code())
        std::cout << getVal<float>();
    if (dataType->hash_code() == typeid(double).hash_code())
        std::cout << getVal<double>();
    if (dataType->hash_code() == typeid(vec2).hash_code())
        std::cout << getVal<vec2>();
    if (dataType->hash_code() == typeid(vec3).hash_code())
        std::cout << getVal<vec3>();
    if (dataType->hash_code() == typeid(vec4).hash_code())
        std::cout << getVal<vec4>();
}
