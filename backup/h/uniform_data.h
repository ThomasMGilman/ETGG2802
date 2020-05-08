#pragma once
#include <stdafx.h>

using dataVariant = std::variant<int, float, double, vec2, vec3, vec4>;

class uniformData {
public:
    std::string uniformName;
    const std::type_info* dataType;
    dataVariant d;
    bool setManually;
    uniformData(std::string name, const std::type_info* dType, dataVariant data, bool manuallySetUniform);

    template<typename T>
    T getVal() { return std::get<T>(d); };

    void set_uniform();

    template<typename T>
    void set_uniform(T newVal) 
    { 
        d = newVal; 
        set_uniform(); 
    };

    void debug_print();

    void operator =(int val) { d = val; };
    void operator +=(int val) { d = getVal<int>() + val; };
    void operator -=(int val) { d = getVal<int>() - val; };
    void operator *=(int val) { d = getVal<int>() * val; };
    void operator /=(int val) { d = getVal<int>() + val; };
    bool operator ==(int val) { return getVal<int>() == val; };
    bool operator !=(int val) { return getVal<int>() != val; };
    bool operator >(int val) { return getVal<int>() > val; };
    bool operator >=(int val) { return getVal<int>() >= val; };
    bool operator <(int val) { return getVal<int>() < val; };
    bool operator <=(int val) { return getVal<int>() <= val; };

    void operator =(float val) { d = val; };
    void operator +=(float val) { d = getVal<float>() + val; };
    void operator -=(float val) { d = getVal<float>() - val; };
    void operator *=(float val) { d = getVal<float>() * val; };
    void operator /=(float val) { d = getVal<float>() + val; };
    bool operator ==(float val) { return getVal<float>() == val; };
    bool operator !=(float val) { return getVal<float>() != val; };
    bool operator >(float val) { return getVal<float>() > val; };
    bool operator >=(float val) { return getVal<float>() >= val; };
    bool operator <(float val) { return getVal<float>() < val; };
    bool operator <=(float val) { return getVal<float>() <= val; };
};