
//jh ssu fall 2019

// ~ Some of these functions (individually noted) are based on code from TDL.
// ~ The TDL copyright is as follows:
 
// ~ Copyright 2009, Google Inc.
// ~ All rights reserved.

// ~ Redistribution and use in source and binary forms, with or without
// ~ modification, are permitted provided that the following conditions are
// ~ met:

// ~ *  Redistributions of source code must retain the above copyright
// ~ notice, this list of conditions and the following disclaimer.
// ~ *  Redistributions in binary form must reproduce the above
// ~ copyright notice, this list of conditions and the following disclaimer
// ~ in the documentation and/or other materials provided with the
// ~ distribution.
// ~ *  Neither the name of Google Inc. nor the names of its
// ~ contributors may be used to endorse or promote products derived from
// ~ this software without specific prior written permission.

// ~ THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// ~ "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// ~ LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// ~ A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// ~ OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// ~ SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// ~ LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// ~ DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// ~ THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// ~ (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// ~ OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#pragma once



#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <cassert>
#include <memory>
#include <array>
#include <cstring>


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 26495)
#endif
/* Copyright (C) 1991-2019 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */
//this is the toplevel include to kick off the header generation process
//https://stackoverflow.com/questions/1489932/how-to-concatenate-twice-with-the-c-preprocessor-and-expand-a-macro-as-in-arg
//Generate each type of vecN
//included from gen.h
//instantiates matN and vecN for one type (float, int, uint) 
//macro trickery to expand macros into real symbols.
//we need to do two levels of expansion to get
//the behavior we want; see
//https://stackoverflow.com/questions/1489932/how-to-concatenate-twice-with-the-c-preprocessor-and-expand-a-macro-as-in-arg
//float, uint32_t, etc.
//~ #define VEC2TYPE concat(vec2_ , SCALARTYPE1)
//~ #define VEC3TYPE concat(vec3_ , SCALARTYPE1)
//~ #define VEC4TYPE concat(vec4_ , SCALARTYPE1)
//~ #define MAT2TYPE concat( mat2_ , SCALARTYPE1 )
//~ #define MAT3TYPE concat( mat3_ , SCALARTYPE1 )
//~ #define MAT4TYPE concat( mat4_ , SCALARTYPE1 )
class vec4;
class vec3;
class vec2;
class mat4;
class mat3;
class mat2;
/////////////////////////////////////////////////////////////////////
class mat2 {
    public:
    //suitable for using in uniform buffer
    float _M[2][4];
    mat2() {
        _M[0][0] = 0; _M[0][1] = 0; _M[0][2] = 0; _M[0][3] = 0;
        _M[1][0] = 0; _M[1][1] = 0; _M[1][2] = 0; _M[1][3] = 0;
    }
    mat2( float m0, float m1,
                float m2, float m3)
    {
        _M[0][0] = m0; _M[0][1] = m1; _M[0][2] = 0; _M[0][3] = 0;
        _M[1][0] = m2; _M[1][1] = m3; _M[1][2] = 0; _M[1][3] = 0;
    }
//included from instantiate.h
//matrix operators that work for all matN types
//Symbols defined in instantiate.h:
// MATSIZE: 2, 3, or 4
// MatType: type of the matrix that contains these
// VecType: The type of vector that goes with this matrix
    /** Initialize matrix from an initializer_list. */
    mat2(const std::initializer_list<float>& L){
        if( L.size() != 2*2 )
            throw std::runtime_error("Bad number of arguments");
        //quiet bogus VS warnings
            _M[0][0] = _M[0][1] = _M[0][2] = _M[0][3] = 0;
            _M[1][0] = _M[1][1] = _M[1][2] = _M[1][3] = 0;
        auto x = L.begin();
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                _M[i][j] = *x++;
            }
        }
    }
    /** Matrix-matrix addition*/
    mat2 operator+(const mat2& m2) const {
        mat2 r;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                r[i][j] = (*this)[i][j] + m2[i][j];
            }
        }
        return r;
    }
    /** Matrix-matrix subtraction*/
    mat2 operator-(const mat2& m2) const {
        mat2 r;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                r[i][j] = (*this)[i][j] - m2[i][j];
            }
        }
        return r;
    }
    /** Matrix-matrix multiplication*/
    mat2 operator*(const mat2& o) const {
        //mat * mat
        mat2 R;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                float total=0;
                for(unsigned k=0;k<2;++k){
                    total += (*this)[i][k] * o[k][j];
                }
                R[i][j] = total;
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    mat2 operator*(float o) const {
        //mat * scalar
        mat2 R;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                R[i][j] = (*this)[i][j] * float(o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    mat2 operator*(double o) const {
        //mat * scalar
        mat2 R;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                R[i][j] = (*this)[i][j] * float(o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    mat2 operator*(int o) const {
        //mat * scalar
        mat2 R;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                R[i][j] = (*this)[i][j] *float( o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    mat2 operator*(unsigned o) const {
        //mat * scalar
        mat2 R;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                R[i][j] = (*this)[i][j] * float( o );
            }
        }
        return R;
    }
    //Vector<MATSIZE> operator*(const Vector<MATSIZE>& v) const ;
    /** Unary + */
    mat2 operator+() const {
        return *this;
    }
    /** Negation */
    mat2 operator-() const {
        return *this * -1.0f;
    }
    const void* tobytes() const {
        return _M;
    }
  private:
    class MatRow{
      public:
        mat2& m;
        unsigned i;
        MatRow(mat2& mm,int ii) : m(mm), i(ii){ }
        float& operator[](unsigned j){
            //each row is padded out to be a vec4
            if( j >= 2 )
                throw std::runtime_error("Bad index");
            return this->m._M[i][j];
        }
    };
    class CMatRow{
      public:
        const mat2& m;
        unsigned i;
        CMatRow(const mat2& mm,int ii) : m(mm), i(ii){ }
        float operator[](unsigned j) const {
            //each row is padded out to be a vec4
            if( j >= 2 )
                throw std::runtime_error("Bad index");
            //~ std::cout << "[" << i << "][" << j << "] = index " << i*4+j << "\n";
            return this->m._M[i][j];
        }
    };
  public:
    MatRow operator[](unsigned i) {
        return MatRow(*this,i);
    }
    CMatRow operator[](unsigned i) const {
        return CMatRow(*this,i);
    }
    bool operator==(const mat2& o) const {
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                if( (*this)[i][j] != o[i][j] )
                    return false;
            }
        }
        return true;
    }
    bool operator!=(const mat2& o) const {
        return ! (*this==o);
    }
    operator std::string() const {
        std::ostringstream oss;
        for(unsigned i=0;i<2;++i){
            oss << "[";
            for(unsigned j=0;j<2;++j){
                oss << (*this)[i][j] << "   ";
            }
            oss << "]\n";
        }
        return oss.str();
    }
    mat2 transpose() const {
        mat2 R;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                R[i][j] = (*this)[j][i];
            }
        }
        return R;
    }
    static mat2 identity(){
        mat2 R;
        for(unsigned i=0;i<2;++i){
            R[i][i]=1;
        }
        return R;
    }
    //no padding; not suitable for use in array or uniform buffer
    const std::array<float,2*2> data() const {
        std::array<float,2*2> rv;
        int k=0;
        for(int i=0;i<2;++i){
            for(int j=0;j<2;++j){
                rv[k++] = (*this)[i][j];
            }
        }
        return rv;
    }
};
class vec2{
  public:
    vec2(const vec3&) = delete;
    vec2(const vec4&) = delete;
    public:
    float x,y;
    vec2(){
        this->x=0;
        this->y=0;
    }
    template<typename T1>
    vec2(T1 x){
        this->x = this->y = (float) x;
    }
    template<typename T1, typename T2>
    vec2(T1 x, T2 y){
        this->x = (float) x;
        this->y = (float) y;
    }
    const vec2 xy() const {
        return *this;
    }
    static vec2 from(const float* fp){
        vec2 v;
        for(unsigned i=0;i<2;++i)
            v[i] = float(fp[i]);
        return v;
    }
    vec2(const std::initializer_list<float>& L){
        if( L.size() != 2 )
            throw std::runtime_error("Bad size for vector initializer");
        auto F = L.begin();
            this->x=*F++;
            this->y=*F++;
    }
    float& operator[](unsigned key){
        switch(key){
            case 0: return x;
            case 1: return y;
            default: throw std::runtime_error("Bad index");
        }
    }
    float operator[](unsigned key) const{
        switch(key){
            case 0: return x;
            case 1: return y;
            default: throw std::runtime_error("Bad index");
        }
    }
    void operator+=(const vec2& o){
        for(unsigned i=0;i<2;++i)
            (*this)[i] += o[i];
    }
    void operator-=(const vec2& o){
        for(unsigned i=0;i<2;++i)
            (*this)[i] -= o[i];
    }
    void operator*=(const vec2& o){
        for(unsigned i=0;i<2;++i)
            (*this)[i] *= o[i];
    }
    void operator*=(const float o){
        for(unsigned i=0;i<2;++i)
            (*this)[i] *= o;
    }
    void operator/=(const vec2& o){
        for(unsigned i=0;i<2;++i)
            (*this)[i] /= o[i];
    }
    vec2 operator-()const {
        vec2 res;
        for(unsigned i=0;i<2;++i){
            double tmp = (*this)[i];
            tmp = -tmp;
            res[i] = (float) tmp;
        }
        return res;
    }
    vec2 operator+() const {
        vec2 res = *this;
        return res;
    }
    vec2 operator+(const vec2& other) const {
        vec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] + other[i];
        return res;
    }
    vec2 operator-(const vec2& other) const {
        vec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] - other[i];
        return res;
    }
    vec2 operator*(const vec2& other) const {
        vec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] * other[i];
        return res;
    }
    vec2 operator/(const vec2& other) const {
        vec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] / other[i];
        return res;
    }
    vec2 operator*(float other) const {
        vec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] * float(other );
        return res;
    }
    vec2 operator*(double other) const {
        vec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] * float(other );
        return res;
    }
    vec2 operator*(int other) const {
        vec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] * float(other );
        return res;
    }
    vec2 operator*(unsigned other) const {
        vec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] * float( other );
        return res;
    }
    vec2 operator/(float other) const {
        vec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] / float(other);
        return res;
    }
    vec2 operator/(double other) const {
        vec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] / float(other);
        return res;
    }
    vec2 operator/(int other) const {
        vec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] / float(other);
        return res;
    }
    vec2 operator/(unsigned other) const {
        vec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] / float(other);
        return res;
    }
    bool operator<(const vec2& o) const {
        for(unsigned i=0;i<2;++i){
            if( (*this)[i] != o[i] )
                return (*this)[i] < o[i];
        }
        return false;
    }
    bool operator==(const vec2& o) const {
        for(unsigned i=0;i<2;++i){
            if((*this)[i] != o[i])
                return false;
        }
        return true;
    }
    bool operator!=(const vec2& o)const {
        return ! (*this == o);
    }
    operator std::string() const {
        std::ostringstream oss;
        oss << "vec" << 2 << "(";
        for(unsigned i=0;i<2;++i){
            oss << " " << (*this)[i];
        }
        oss << ")";
        return oss.str();
    }
    bool isZero() const {
        for(unsigned i=0;i<2;++i){
            if( (*this)[i] != 0 )
                return false;
        }
        return true;
    }
    unsigned size() const {
        return 2;
    }
};
inline
mat2 operator*( float f, const mat2& m ){
    return m*f;
}
inline
mat2 operator*( double f, const mat2& m ){
    return m*f;
}
inline
mat2 operator*( int f, const mat2& m ){
    return m*f;
}
inline
mat2 operator*( unsigned f, const mat2& m ){
    return m*f;
}
inline
vec2 operator*( float f, const vec2& m ){
    return m*f;
}
inline
vec2 operator*( double f, const vec2& m ){
    return m*f;
}
inline
vec2 operator*( int f, const vec2& m ){
    return m*f;
}
inline
vec2 operator*( unsigned f, const vec2& m ){
    return m*f;
}
inline
vec2 operator*(const mat2& m, const vec2& o) {
    //mat * vector
    vec2 R;
    for(unsigned i=0;i<2;++i){
        float total = (float) 0;
        for(unsigned j=0;j<2;++j){
            total += m[i][j] * o[j];
        }
        R[i] = total;
    }
    return R;
}
//vector-matrix multiplication
inline
vec2 operator*(const vec2& v, const mat2& o){
    vec2 R;
    for(unsigned i=0;i<2;++i){
        float total=0;
        for(unsigned j=0;j<2;++j){
            total += float(v[j])*float(o[j][i]);
        }
        R[i]=float(total);
    }
    return R;
}
inline
mat2 transpose(const mat2& m){
    return m.transpose();
}
inline
float dot(const vec2& v, const vec2& w){
    float total =0;
    for(unsigned i=0;i<2;++i){
        total += v[i] * w[i];
    }
    return total;
}
inline
float length(const vec2& v){
    return (float)std::sqrt( dot(v,v) );
}
inline
std::ostream& operator<<(std::ostream& o, const vec2& v){
    o << (std::string)v;
    return o;
}
inline
std::ostream& operator<<(std::ostream& o, const mat2& v){
    o << (std::string)v;
    return o;
}
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
class mat3 {
    public:
    //suitable for using in uniform buffer
    float _M[3][4];
    mat3() {
        for(int i=0;i<3;++i){
            for(int j=0;j<4;++j){
                _M[i][j]= (float) 0;
            }
        }
    }
    mat3( float m0, float m1, float m2,
                float m3, float m4, float m5,
                float m6, float m7, float m8) :
                    mat3({m0,m1,m2,m3,m4,m5,m6,m7,m8})
    {
        _M[0][0] = m0; _M[0][1] = m1; _M[0][2] = m2; _M[0][3] = 0;
        _M[1][0] = m3; _M[1][1] = m4; _M[1][2] = m5; _M[1][3] = 0;
        _M[2][0] = m6; _M[2][1] = m7; _M[2][2] = m8; _M[2][3] = 0;
    }
//included from instantiate.h
//matrix operators that work for all matN types
//Symbols defined in instantiate.h:
// MATSIZE: 2, 3, or 4
// MatType: type of the matrix that contains these
// VecType: The type of vector that goes with this matrix
    /** Initialize matrix from an initializer_list. */
    mat3(const std::initializer_list<float>& L){
        if( L.size() != 3*3 )
            throw std::runtime_error("Bad number of arguments");
        //quiet bogus VS warnings
            _M[0][0] = _M[0][1] = _M[0][2] = _M[0][3] = 0;
            _M[1][0] = _M[1][1] = _M[1][2] = _M[1][3] = 0;
            _M[2][0] = _M[2][1] = _M[2][2] = _M[2][3] = 0;
        auto x = L.begin();
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                _M[i][j] = *x++;
            }
        }
    }
    /** Matrix-matrix addition*/
    mat3 operator+(const mat3& m2) const {
        mat3 r;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                r[i][j] = (*this)[i][j] + m2[i][j];
            }
        }
        return r;
    }
    /** Matrix-matrix subtraction*/
    mat3 operator-(const mat3& m2) const {
        mat3 r;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                r[i][j] = (*this)[i][j] - m2[i][j];
            }
        }
        return r;
    }
    /** Matrix-matrix multiplication*/
    mat3 operator*(const mat3& o) const {
        //mat * mat
        mat3 R;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                float total=0;
                for(unsigned k=0;k<3;++k){
                    total += (*this)[i][k] * o[k][j];
                }
                R[i][j] = total;
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    mat3 operator*(float o) const {
        //mat * scalar
        mat3 R;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                R[i][j] = (*this)[i][j] * float(o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    mat3 operator*(double o) const {
        //mat * scalar
        mat3 R;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                R[i][j] = (*this)[i][j] * float(o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    mat3 operator*(int o) const {
        //mat * scalar
        mat3 R;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                R[i][j] = (*this)[i][j] *float( o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    mat3 operator*(unsigned o) const {
        //mat * scalar
        mat3 R;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                R[i][j] = (*this)[i][j] * float( o );
            }
        }
        return R;
    }
    //Vector<MATSIZE> operator*(const Vector<MATSIZE>& v) const ;
    /** Unary + */
    mat3 operator+() const {
        return *this;
    }
    /** Negation */
    mat3 operator-() const {
        return *this * -1.0f;
    }
    const void* tobytes() const {
        return _M;
    }
  private:
    class MatRow{
      public:
        mat3& m;
        unsigned i;
        MatRow(mat3& mm,int ii) : m(mm), i(ii){ }
        float& operator[](unsigned j){
            //each row is padded out to be a vec4
            if( j >= 3 )
                throw std::runtime_error("Bad index");
            return this->m._M[i][j];
        }
    };
    class CMatRow{
      public:
        const mat3& m;
        unsigned i;
        CMatRow(const mat3& mm,int ii) : m(mm), i(ii){ }
        float operator[](unsigned j) const {
            //each row is padded out to be a vec4
            if( j >= 3 )
                throw std::runtime_error("Bad index");
            //~ std::cout << "[" << i << "][" << j << "] = index " << i*4+j << "\n";
            return this->m._M[i][j];
        }
    };
  public:
    MatRow operator[](unsigned i) {
        return MatRow(*this,i);
    }
    CMatRow operator[](unsigned i) const {
        return CMatRow(*this,i);
    }
    bool operator==(const mat3& o) const {
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                if( (*this)[i][j] != o[i][j] )
                    return false;
            }
        }
        return true;
    }
    bool operator!=(const mat3& o) const {
        return ! (*this==o);
    }
    operator std::string() const {
        std::ostringstream oss;
        for(unsigned i=0;i<3;++i){
            oss << "[";
            for(unsigned j=0;j<3;++j){
                oss << (*this)[i][j] << "   ";
            }
            oss << "]\n";
        }
        return oss.str();
    }
    mat3 transpose() const {
        mat3 R;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                R[i][j] = (*this)[j][i];
            }
        }
        return R;
    }
    static mat3 identity(){
        mat3 R;
        for(unsigned i=0;i<3;++i){
            R[i][i]=1;
        }
        return R;
    }
    //no padding; not suitable for use in array or uniform buffer
    const std::array<float,3*3> data() const {
        std::array<float,3*3> rv;
        int k=0;
        for(int i=0;i<3;++i){
            for(int j=0;j<3;++j){
                rv[k++] = (*this)[i][j];
            }
        }
        return rv;
    }
};
class vec3{
  public:
    vec3(const vec2&) = delete;
    vec3(const vec4&) = delete;
    public:
    float x=0,y=0,z=0;
    vec3(){
        x=y=z=0;
    }
    template<typename T>
    vec3(T q) {
        x=y=z=(float)q;
    }
    template<typename T>
    vec3( vec2 v, T z){
        this->x = (float) v.x;
        this->y = (float) v.y;
        this->z = (float) z;
    }
    vec3(float x, float y, float z){
        this->x=x;
        this->y=y;
        this->z=z;
    }
    void setXY(vec2 v){
        x=v.x;
        y=v.y;
    }
    const vec2 xy() const{
        return vec2(x,y);
    }
    const vec3 xyz() const {
        return *this;
    }
    static vec3 from(const float* fp){
        vec3 v;
        for(unsigned i=0;i<3;++i)
            v[i] = float(fp[i]);
        return v;
    }
    vec3(const std::initializer_list<float>& L){
        if( L.size() != 3 )
            throw std::runtime_error("Bad size for vector initializer");
        auto F = L.begin();
            this->x=*F++;
            this->y=*F++;
            this->z=*F++;
    }
    float& operator[](unsigned key){
        switch(key){
            case 0: return x;
            case 1: return y;
                case 2: return z;
            default: throw std::runtime_error("Bad index");
        }
    }
    float operator[](unsigned key) const{
        switch(key){
            case 0: return x;
            case 1: return y;
                case 2: return z;
            default: throw std::runtime_error("Bad index");
        }
    }
    void operator+=(const vec3& o){
        for(unsigned i=0;i<3;++i)
            (*this)[i] += o[i];
    }
    void operator-=(const vec3& o){
        for(unsigned i=0;i<3;++i)
            (*this)[i] -= o[i];
    }
    void operator*=(const vec3& o){
        for(unsigned i=0;i<3;++i)
            (*this)[i] *= o[i];
    }
    void operator*=(const float o){
        for(unsigned i=0;i<3;++i)
            (*this)[i] *= o;
    }
    void operator/=(const vec3& o){
        for(unsigned i=0;i<3;++i)
            (*this)[i] /= o[i];
    }
    vec3 operator-()const {
        vec3 res;
        for(unsigned i=0;i<3;++i){
            double tmp = (*this)[i];
            tmp = -tmp;
            res[i] = (float) tmp;
        }
        return res;
    }
    vec3 operator+() const {
        vec3 res = *this;
        return res;
    }
    vec3 operator+(const vec3& other) const {
        vec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] + other[i];
        return res;
    }
    vec3 operator-(const vec3& other) const {
        vec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] - other[i];
        return res;
    }
    vec3 operator*(const vec3& other) const {
        vec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] * other[i];
        return res;
    }
    vec3 operator/(const vec3& other) const {
        vec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] / other[i];
        return res;
    }
    vec3 operator*(float other) const {
        vec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] * float(other );
        return res;
    }
    vec3 operator*(double other) const {
        vec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] * float(other );
        return res;
    }
    vec3 operator*(int other) const {
        vec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] * float(other );
        return res;
    }
    vec3 operator*(unsigned other) const {
        vec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] * float( other );
        return res;
    }
    vec3 operator/(float other) const {
        vec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] / float(other);
        return res;
    }
    vec3 operator/(double other) const {
        vec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] / float(other);
        return res;
    }
    vec3 operator/(int other) const {
        vec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] / float(other);
        return res;
    }
    vec3 operator/(unsigned other) const {
        vec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] / float(other);
        return res;
    }
    bool operator<(const vec3& o) const {
        for(unsigned i=0;i<3;++i){
            if( (*this)[i] != o[i] )
                return (*this)[i] < o[i];
        }
        return false;
    }
    bool operator==(const vec3& o) const {
        for(unsigned i=0;i<3;++i){
            if((*this)[i] != o[i])
                return false;
        }
        return true;
    }
    bool operator!=(const vec3& o)const {
        return ! (*this == o);
    }
    operator std::string() const {
        std::ostringstream oss;
        oss << "vec" << 3 << "(";
        for(unsigned i=0;i<3;++i){
            oss << " " << (*this)[i];
        }
        oss << ")";
        return oss.str();
    }
    bool isZero() const {
        for(unsigned i=0;i<3;++i){
            if( (*this)[i] != 0 )
                return false;
        }
        return true;
    }
    unsigned size() const {
        return 3;
    }
};
inline
mat3 operator*( float f, const mat3& m ){
    return m*f;
}
inline
mat3 operator*( double f, const mat3& m ){
    return m*f;
}
inline
mat3 operator*( int f, const mat3& m ){
    return m*f;
}
inline
mat3 operator*( unsigned f, const mat3& m ){
    return m*f;
}
inline
vec3 operator*( float f, const vec3& m ){
    return m*f;
}
inline
vec3 operator*( double f, const vec3& m ){
    return m*f;
}
inline
vec3 operator*( int f, const vec3& m ){
    return m*f;
}
inline
vec3 operator*( unsigned f, const vec3& m ){
    return m*f;
}
inline
vec3 operator*(const mat3& m, const vec3& o) {
    //mat * vector
    vec3 R;
    for(unsigned i=0;i<3;++i){
        float total = (float) 0;
        for(unsigned j=0;j<3;++j){
            total += m[i][j] * o[j];
        }
        R[i] = total;
    }
    return R;
}
//vector-matrix multiplication
inline
vec3 operator*(const vec3& v, const mat3& o){
    vec3 R;
    for(unsigned i=0;i<3;++i){
        float total=0;
        for(unsigned j=0;j<3;++j){
            total += float(v[j])*float(o[j][i]);
        }
        R[i]=float(total);
    }
    return R;
}
inline
mat3 transpose(const mat3& m){
    return m.transpose();
}
inline
float dot(const vec3& v, const vec3& w){
    float total =0;
    for(unsigned i=0;i<3;++i){
        total += v[i] * w[i];
    }
    return total;
}
inline
float length(const vec3& v){
    return (float)std::sqrt( dot(v,v) );
}
inline
std::ostream& operator<<(std::ostream& o, const vec3& v){
    o << (std::string)v;
    return o;
}
inline
std::ostream& operator<<(std::ostream& o, const mat3& v){
    o << (std::string)v;
    return o;
}
///////////////////////////////////////////////////////////////
class mat4 {
    public:
    //suitable for using in uniform buffer
    float _M[4][4];
    mat4() {
        _M[0][0] = 0; _M[0][1] = 0; _M[0][2] = 0; _M[0][3] = 0;
        _M[1][0] = 0; _M[1][1] = 0; _M[1][2] = 0; _M[1][3] = 0;
        _M[2][0] = 0; _M[2][1] = 0; _M[2][2] = 0; _M[2][3] = 0;
        _M[3][0] = 0; _M[3][1] = 0; _M[3][2] = 0; _M[3][3] = 0;
    }
    mat4( float m0, float m1, float m2, float m3,
                float m4, float m5, float m6, float m7,
                float m8, float m9, float ma, float mb,
                float mc, float md, float me, float mf)
    {
        _M[0][0] = m0; _M[0][1] = m1; _M[0][2] = m2; _M[0][3] = m3;
        _M[1][0] = m4; _M[1][1] = m5; _M[1][2] = m6; _M[1][3] = m7;
        _M[2][0] = m8; _M[2][1] = m9; _M[2][2] = ma; _M[2][3] = mb;
        _M[3][0] = mc; _M[3][1] = md; _M[3][2] = me; _M[3][3] = mf;
    }
    mat4( double m0, double m1, double m2, double m3,
                double m4, double m5, double m6, double m7,
                double m8, double m9, double ma, double mb,
                double mc, double md, double me, double mf)
    {
        _M[0][0] = (float) m0; _M[0][1] = (float) m1; _M[0][2] = (float) m2; _M[0][3] = (float) m3;
        _M[1][0] = (float) m4; _M[1][1] = (float) m5; _M[1][2] = (float) m6; _M[1][3] = (float) m7;
        _M[2][0] = (float) m8; _M[2][1] = (float) m9; _M[2][2] = (float) ma; _M[2][3] = (float) mb;
        _M[3][0] = (float) mc; _M[3][1] = (float) md; _M[3][2] = (float) me; _M[3][3] = (float) mf;
    }
    mat3 upper3x3() const
    {
       return mat3(
            (*this)[0][0], (*this)[0][1], (*this)[0][2],
            (*this)[1][0], (*this)[1][1], (*this)[1][2],
            (*this)[2][0], (*this)[2][1], (*this)[2][2]
        );
    }
//included from instantiate.h
//matrix operators that work for all matN types
//Symbols defined in instantiate.h:
// MATSIZE: 2, 3, or 4
// MatType: type of the matrix that contains these
// VecType: The type of vector that goes with this matrix
    /** Initialize matrix from an initializer_list. */
    mat4(const std::initializer_list<float>& L){
        if( L.size() != 4*4 )
            throw std::runtime_error("Bad number of arguments");
        //quiet bogus VS warnings
            _M[0][0] = _M[0][1] = _M[0][2] = _M[0][3] = 0;
            _M[1][0] = _M[1][1] = _M[1][2] = _M[1][3] = 0;
            _M[2][0] = _M[2][1] = _M[2][2] = _M[2][3] = 0;
            _M[3][0] = _M[3][1] = _M[3][2] = _M[3][3] = 0;
        auto x = L.begin();
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                _M[i][j] = *x++;
            }
        }
    }
    /** Matrix-matrix addition*/
    mat4 operator+(const mat4& m2) const {
        mat4 r;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                r[i][j] = (*this)[i][j] + m2[i][j];
            }
        }
        return r;
    }
    /** Matrix-matrix subtraction*/
    mat4 operator-(const mat4& m2) const {
        mat4 r;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                r[i][j] = (*this)[i][j] - m2[i][j];
            }
        }
        return r;
    }
    /** Matrix-matrix multiplication*/
    mat4 operator*(const mat4& o) const {
        //mat * mat
        mat4 R;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                float total=0;
                for(unsigned k=0;k<4;++k){
                    total += (*this)[i][k] * o[k][j];
                }
                R[i][j] = total;
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    mat4 operator*(float o) const {
        //mat * scalar
        mat4 R;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                R[i][j] = (*this)[i][j] * float(o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    mat4 operator*(double o) const {
        //mat * scalar
        mat4 R;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                R[i][j] = (*this)[i][j] * float(o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    mat4 operator*(int o) const {
        //mat * scalar
        mat4 R;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                R[i][j] = (*this)[i][j] *float( o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    mat4 operator*(unsigned o) const {
        //mat * scalar
        mat4 R;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                R[i][j] = (*this)[i][j] * float( o );
            }
        }
        return R;
    }
    //Vector<MATSIZE> operator*(const Vector<MATSIZE>& v) const ;
    /** Unary + */
    mat4 operator+() const {
        return *this;
    }
    /** Negation */
    mat4 operator-() const {
        return *this * -1.0f;
    }
    const void* tobytes() const {
        return _M;
    }
  private:
    class MatRow{
      public:
        mat4& m;
        unsigned i;
        MatRow(mat4& mm,int ii) : m(mm), i(ii){ }
        float& operator[](unsigned j){
            //each row is padded out to be a vec4
            if( j >= 4 )
                throw std::runtime_error("Bad index");
            return this->m._M[i][j];
        }
    };
    class CMatRow{
      public:
        const mat4& m;
        unsigned i;
        CMatRow(const mat4& mm,int ii) : m(mm), i(ii){ }
        float operator[](unsigned j) const {
            //each row is padded out to be a vec4
            if( j >= 4 )
                throw std::runtime_error("Bad index");
            //~ std::cout << "[" << i << "][" << j << "] = index " << i*4+j << "\n";
            return this->m._M[i][j];
        }
    };
  public:
    MatRow operator[](unsigned i) {
        return MatRow(*this,i);
    }
    CMatRow operator[](unsigned i) const {
        return CMatRow(*this,i);
    }
    bool operator==(const mat4& o) const {
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                if( (*this)[i][j] != o[i][j] )
                    return false;
            }
        }
        return true;
    }
    bool operator!=(const mat4& o) const {
        return ! (*this==o);
    }
    operator std::string() const {
        std::ostringstream oss;
        for(unsigned i=0;i<4;++i){
            oss << "[";
            for(unsigned j=0;j<4;++j){
                oss << (*this)[i][j] << "   ";
            }
            oss << "]\n";
        }
        return oss.str();
    }
    mat4 transpose() const {
        mat4 R;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                R[i][j] = (*this)[j][i];
            }
        }
        return R;
    }
    static mat4 identity(){
        mat4 R;
        for(unsigned i=0;i<4;++i){
            R[i][i]=1;
        }
        return R;
    }
    //no padding; not suitable for use in array or uniform buffer
    const std::array<float,4*4> data() const {
        std::array<float,4*4> rv;
        int k=0;
        for(int i=0;i<4;++i){
            for(int j=0;j<4;++j){
                rv[k++] = (*this)[i][j];
            }
        }
        return rv;
    }
};
class vec4{
    vec4(const vec2&) = delete;
    vec4(const vec3&) = delete;
    public:
    float x,y,z,w;
    vec4(){
        x=y=z=w=0;
    }
    template<typename T>
    vec4(T q) {
        x=y=z=w=(float)q;
    }
    template<typename T1, typename T2, typename T3, typename T4>
    vec4( T1 x, T2 y, T3 z, T4 w){
        this->x=(float) x;
        this->y=(float) y;
        this->z=(float) z;
        this->w=(float) w;
    }
    template<typename T>
    vec4(const vec3& v, T w) {
        this->x = (float) v.x;
        this->y = (float) v.y;
        this->z = (float) v.z;
        this->w = (float) w;
    }
    void setXYZ(vec3 v){
        x=v.x;
        y=v.y;
        z=v.z;
    }
    void setXY(vec2 v){
        x=v.x;
        y=v.y;
    }
    const vec2 xy() const {
        return vec2(x,y);
    }
    const vec3 xyz() const {
        return vec3(x,y,z);
    }
    const vec4 xyzw() const {
        return *this;
    }
    static vec4 from(const float* fp){
        vec4 v;
        for(unsigned i=0;i<4;++i)
            v[i] = float(fp[i]);
        return v;
    }
    vec4(const std::initializer_list<float>& L){
        if( L.size() != 4 )
            throw std::runtime_error("Bad size for vector initializer");
        auto F = L.begin();
            this->w=*F++;
    }
    float& operator[](unsigned key){
        switch(key){
            case 0: return x;
            case 1: return y;
                case 2: return z;
                case 3: return w;
            default: throw std::runtime_error("Bad index");
        }
    }
    float operator[](unsigned key) const{
        switch(key){
            case 0: return x;
            case 1: return y;
                case 2: return z;
                case 3: return w;
            default: throw std::runtime_error("Bad index");
        }
    }
    void operator+=(const vec4& o){
        for(unsigned i=0;i<4;++i)
            (*this)[i] += o[i];
    }
    void operator-=(const vec4& o){
        for(unsigned i=0;i<4;++i)
            (*this)[i] -= o[i];
    }
    void operator*=(const vec4& o){
        for(unsigned i=0;i<4;++i)
            (*this)[i] *= o[i];
    }
    void operator*=(const float o){
        for(unsigned i=0;i<4;++i)
            (*this)[i] *= o;
    }
    void operator/=(const vec4& o){
        for(unsigned i=0;i<4;++i)
            (*this)[i] /= o[i];
    }
    vec4 operator-()const {
        vec4 res;
        for(unsigned i=0;i<4;++i){
            double tmp = (*this)[i];
            tmp = -tmp;
            res[i] = (float) tmp;
        }
        return res;
    }
    vec4 operator+() const {
        vec4 res = *this;
        return res;
    }
    vec4 operator+(const vec4& other) const {
        vec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] + other[i];
        return res;
    }
    vec4 operator-(const vec4& other) const {
        vec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] - other[i];
        return res;
    }
    vec4 operator*(const vec4& other) const {
        vec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] * other[i];
        return res;
    }
    vec4 operator/(const vec4& other) const {
        vec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] / other[i];
        return res;
    }
    vec4 operator*(float other) const {
        vec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] * float(other );
        return res;
    }
    vec4 operator*(double other) const {
        vec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] * float(other );
        return res;
    }
    vec4 operator*(int other) const {
        vec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] * float(other );
        return res;
    }
    vec4 operator*(unsigned other) const {
        vec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] * float( other );
        return res;
    }
    vec4 operator/(float other) const {
        vec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] / float(other);
        return res;
    }
    vec4 operator/(double other) const {
        vec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] / float(other);
        return res;
    }
    vec4 operator/(int other) const {
        vec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] / float(other);
        return res;
    }
    vec4 operator/(unsigned other) const {
        vec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] / float(other);
        return res;
    }
    bool operator<(const vec4& o) const {
        for(unsigned i=0;i<4;++i){
            if( (*this)[i] != o[i] )
                return (*this)[i] < o[i];
        }
        return false;
    }
    bool operator==(const vec4& o) const {
        for(unsigned i=0;i<4;++i){
            if((*this)[i] != o[i])
                return false;
        }
        return true;
    }
    bool operator!=(const vec4& o)const {
        return ! (*this == o);
    }
    operator std::string() const {
        std::ostringstream oss;
        oss << "vec" << 4 << "(";
        for(unsigned i=0;i<4;++i){
            oss << " " << (*this)[i];
        }
        oss << ")";
        return oss.str();
    }
    bool isZero() const {
        for(unsigned i=0;i<4;++i){
            if( (*this)[i] != 0 )
                return false;
        }
        return true;
    }
    unsigned size() const {
        return 4;
    }
};
inline
mat4 operator*( float f, const mat4& m ){
    return m*f;
}
inline
mat4 operator*( double f, const mat4& m ){
    return m*f;
}
inline
mat4 operator*( int f, const mat4& m ){
    return m*f;
}
inline
mat4 operator*( unsigned f, const mat4& m ){
    return m*f;
}
inline
vec4 operator*( float f, const vec4& m ){
    return m*f;
}
inline
vec4 operator*( double f, const vec4& m ){
    return m*f;
}
inline
vec4 operator*( int f, const vec4& m ){
    return m*f;
}
inline
vec4 operator*( unsigned f, const vec4& m ){
    return m*f;
}
inline
vec4 operator*(const mat4& m, const vec4& o) {
    //mat * vector
    vec4 R;
    for(unsigned i=0;i<4;++i){
        float total = (float) 0;
        for(unsigned j=0;j<4;++j){
            total += m[i][j] * o[j];
        }
        R[i] = total;
    }
    return R;
}
//vector-matrix multiplication
inline
vec4 operator*(const vec4& v, const mat4& o){
    vec4 R;
    for(unsigned i=0;i<4;++i){
        float total=0;
        for(unsigned j=0;j<4;++j){
            total += float(v[j])*float(o[j][i]);
        }
        R[i]=float(total);
    }
    return R;
}
inline
mat4 transpose(const mat4& m){
    return m.transpose();
}
inline
float dot(const vec4& v, const vec4& w){
    float total =0;
    for(unsigned i=0;i<4;++i){
        total += v[i] * w[i];
    }
    return total;
}
inline
float length(const vec4& v){
    return (float)std::sqrt( dot(v,v) );
}
inline
std::ostream& operator<<(std::ostream& o, const vec4& v){
    o << (std::string)v;
    return o;
}
inline
std::ostream& operator<<(std::ostream& o, const mat4& v){
    o << (std::string)v;
    return o;
}
///////////////////////////////////////////////////////////////
//only defined for vec3/ivec3/uvec3
inline
vec3 cross(const vec3& v, const vec3& w){
    return vec3(
        v.y*w.z - w.y*v.z,
        w.x*v.z - v.x*w.z,
        v.x*w.y - w.x*v.y
    );
}
//~ //vec4::xy()
//~ inline const VEC2TYPE VEC4TYPE::xy() const 
//~ { 
    //~ return VEC2TYPE(x,y); 
//~ }
//~ //vec4::xyz()
//~ inline const VEC3TYPE VEC4TYPE::xyz() const 
//~ { 
    //~ return VEC3TYPE(x,y,z); 
//~ }
//~ //vec3.xy()
//~ inline const VEC2TYPE VEC3TYPE::xy() const 
//~ { 
    //~ return VEC2TYPE(x,y); 
//~ }
//~ inline void VEC4TYPE::setXYZ(VEC3TYPE v){
    //~ x=v.x;
    //~ y=v.y;
    //~ z=v.z;
//~ }
//~ inline void VEC4TYPE::setXY(VEC2TYPE v){
    //~ x=v.x;
    //~ y=v.y;
//~ }
//~ inline void VEC3TYPE::setXY(VEC2TYPE v){
    //~ x=v.x;
    //~ y=v.y;
//~ }
//included from gen.h
//instantiates matN and vecN for one type (float, int, uint) 
//macro trickery to expand macros into real symbols.
//we need to do two levels of expansion to get
//the behavior we want; see
//https://stackoverflow.com/questions/1489932/how-to-concatenate-twice-with-the-c-preprocessor-and-expand-a-macro-as-in-arg
//float, uint32_t, etc.
//~ #define VEC2TYPE concat(vec2_ , SCALARTYPE1)
//~ #define VEC3TYPE concat(vec3_ , SCALARTYPE1)
//~ #define VEC4TYPE concat(vec4_ , SCALARTYPE1)
//~ #define MAT2TYPE concat( mat2_ , SCALARTYPE1 )
//~ #define MAT3TYPE concat( mat3_ , SCALARTYPE1 )
//~ #define MAT4TYPE concat( mat4_ , SCALARTYPE1 )
class uvec4;
class uvec3;
class uvec2;
class umat4;
class umat3;
class umat2;
/////////////////////////////////////////////////////////////////////
class umat2 {
    public:
    //suitable for using in uniform buffer
    uint32_t _M[2][4];
    umat2() {
        _M[0][0] = 0; _M[0][1] = 0; _M[0][2] = 0; _M[0][3] = 0;
        _M[1][0] = 0; _M[1][1] = 0; _M[1][2] = 0; _M[1][3] = 0;
    }
    umat2( uint32_t m0, uint32_t m1,
                uint32_t m2, uint32_t m3)
    {
        _M[0][0] = m0; _M[0][1] = m1; _M[0][2] = 0; _M[0][3] = 0;
        _M[1][0] = m2; _M[1][1] = m3; _M[1][2] = 0; _M[1][3] = 0;
    }
//included from instantiate.h
//matrix operators that work for all matN types
//Symbols defined in instantiate.h:
// MATSIZE: 2, 3, or 4
// MatType: type of the matrix that contains these
// VecType: The type of vector that goes with this matrix
    /** Initialize matrix from an initializer_list. */
    umat2(const std::initializer_list<uint32_t>& L){
        if( L.size() != 2*2 )
            throw std::runtime_error("Bad number of arguments");
        //quiet bogus VS warnings
            _M[0][0] = _M[0][1] = _M[0][2] = _M[0][3] = 0;
            _M[1][0] = _M[1][1] = _M[1][2] = _M[1][3] = 0;
        auto x = L.begin();
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                _M[i][j] = *x++;
            }
        }
    }
    /** Matrix-matrix addition*/
    umat2 operator+(const umat2& m2) const {
        umat2 r;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                r[i][j] = (*this)[i][j] + m2[i][j];
            }
        }
        return r;
    }
    /** Matrix-matrix subtraction*/
    umat2 operator-(const umat2& m2) const {
        umat2 r;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                r[i][j] = (*this)[i][j] - m2[i][j];
            }
        }
        return r;
    }
    /** Matrix-matrix multiplication*/
    umat2 operator*(const umat2& o) const {
        //mat * mat
        umat2 R;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                uint32_t total=0;
                for(unsigned k=0;k<2;++k){
                    total += (*this)[i][k] * o[k][j];
                }
                R[i][j] = total;
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    umat2 operator*(float o) const {
        //mat * scalar
        umat2 R;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                R[i][j] = (*this)[i][j] * uint32_t(o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    umat2 operator*(double o) const {
        //mat * scalar
        umat2 R;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                R[i][j] = (*this)[i][j] * uint32_t(o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    umat2 operator*(int o) const {
        //mat * scalar
        umat2 R;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                R[i][j] = (*this)[i][j] *uint32_t( o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    umat2 operator*(unsigned o) const {
        //mat * scalar
        umat2 R;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                R[i][j] = (*this)[i][j] * uint32_t( o );
            }
        }
        return R;
    }
    //Vector<MATSIZE> operator*(const Vector<MATSIZE>& v) const ;
    /** Unary + */
    umat2 operator+() const {
        return *this;
    }
    /** Negation */
    umat2 operator-() const {
        return *this * -1.0f;
    }
    const void* tobytes() const {
        return _M;
    }
  private:
    class MatRow{
      public:
        umat2& m;
        unsigned i;
        MatRow(umat2& mm,int ii) : m(mm), i(ii){ }
        uint32_t& operator[](unsigned j){
            //each row is padded out to be a vec4
            if( j >= 2 )
                throw std::runtime_error("Bad index");
            return this->m._M[i][j];
        }
    };
    class CMatRow{
      public:
        const umat2& m;
        unsigned i;
        CMatRow(const umat2& mm,int ii) : m(mm), i(ii){ }
        uint32_t operator[](unsigned j) const {
            //each row is padded out to be a vec4
            if( j >= 2 )
                throw std::runtime_error("Bad index");
            //~ std::cout << "[" << i << "][" << j << "] = index " << i*4+j << "\n";
            return this->m._M[i][j];
        }
    };
  public:
    MatRow operator[](unsigned i) {
        return MatRow(*this,i);
    }
    CMatRow operator[](unsigned i) const {
        return CMatRow(*this,i);
    }
    bool operator==(const umat2& o) const {
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                if( (*this)[i][j] != o[i][j] )
                    return false;
            }
        }
        return true;
    }
    bool operator!=(const umat2& o) const {
        return ! (*this==o);
    }
    operator std::string() const {
        std::ostringstream oss;
        for(unsigned i=0;i<2;++i){
            oss << "[";
            for(unsigned j=0;j<2;++j){
                oss << (*this)[i][j] << "   ";
            }
            oss << "]\n";
        }
        return oss.str();
    }
    umat2 transpose() const {
        umat2 R;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                R[i][j] = (*this)[j][i];
            }
        }
        return R;
    }
    static umat2 identity(){
        umat2 R;
        for(unsigned i=0;i<2;++i){
            R[i][i]=1;
        }
        return R;
    }
    //no padding; not suitable for use in array or uniform buffer
    const std::array<uint32_t,2*2> data() const {
        std::array<uint32_t,2*2> rv;
        int k=0;
        for(int i=0;i<2;++i){
            for(int j=0;j<2;++j){
                rv[k++] = (*this)[i][j];
            }
        }
        return rv;
    }
};
class uvec2{
  public:
    uvec2(const uvec3&) = delete;
    uvec2(const uvec4&) = delete;
    public:
    uint32_t x,y;
    uvec2(){
        this->x=0;
        this->y=0;
    }
    template<typename T1>
    uvec2(T1 x){
        this->x = this->y = (uint32_t) x;
    }
    template<typename T1, typename T2>
    uvec2(T1 x, T2 y){
        this->x = (uint32_t) x;
        this->y = (uint32_t) y;
    }
    const uvec2 xy() const {
        return *this;
    }
    static uvec2 from(const float* fp){
        uvec2 v;
        for(unsigned i=0;i<2;++i)
            v[i] = uint32_t(fp[i]);
        return v;
    }
    uvec2(const std::initializer_list<uint32_t>& L){
        if( L.size() != 2 )
            throw std::runtime_error("Bad size for vector initializer");
        auto F = L.begin();
            this->x=*F++;
            this->y=*F++;
    }
    uint32_t& operator[](unsigned key){
        switch(key){
            case 0: return x;
            case 1: return y;
            default: throw std::runtime_error("Bad index");
        }
    }
    uint32_t operator[](unsigned key) const{
        switch(key){
            case 0: return x;
            case 1: return y;
            default: throw std::runtime_error("Bad index");
        }
    }
    void operator+=(const uvec2& o){
        for(unsigned i=0;i<2;++i)
            (*this)[i] += o[i];
    }
    void operator-=(const uvec2& o){
        for(unsigned i=0;i<2;++i)
            (*this)[i] -= o[i];
    }
    void operator*=(const uvec2& o){
        for(unsigned i=0;i<2;++i)
            (*this)[i] *= o[i];
    }
    void operator*=(const uint32_t o){
        for(unsigned i=0;i<2;++i)
            (*this)[i] *= o;
    }
    void operator/=(const uvec2& o){
        for(unsigned i=0;i<2;++i)
            (*this)[i] /= o[i];
    }
    uvec2 operator-()const {
        uvec2 res;
        for(unsigned i=0;i<2;++i){
            double tmp = (*this)[i];
            tmp = -tmp;
            res[i] = (uint32_t) tmp;
        }
        return res;
    }
    uvec2 operator+() const {
        uvec2 res = *this;
        return res;
    }
    uvec2 operator+(const uvec2& other) const {
        uvec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] + other[i];
        return res;
    }
    uvec2 operator-(const uvec2& other) const {
        uvec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] - other[i];
        return res;
    }
    uvec2 operator*(const uvec2& other) const {
        uvec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] * other[i];
        return res;
    }
    uvec2 operator/(const uvec2& other) const {
        uvec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] / other[i];
        return res;
    }
    uvec2 operator*(float other) const {
        uvec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] * uint32_t(other );
        return res;
    }
    uvec2 operator*(double other) const {
        uvec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] * uint32_t(other );
        return res;
    }
    uvec2 operator*(int other) const {
        uvec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] * uint32_t(other );
        return res;
    }
    uvec2 operator*(unsigned other) const {
        uvec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] * uint32_t( other );
        return res;
    }
    uvec2 operator/(float other) const {
        uvec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] / uint32_t(other);
        return res;
    }
    uvec2 operator/(double other) const {
        uvec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] / uint32_t(other);
        return res;
    }
    uvec2 operator/(int other) const {
        uvec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] / uint32_t(other);
        return res;
    }
    uvec2 operator/(unsigned other) const {
        uvec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] / uint32_t(other);
        return res;
    }
    bool operator<(const uvec2& o) const {
        for(unsigned i=0;i<2;++i){
            if( (*this)[i] != o[i] )
                return (*this)[i] < o[i];
        }
        return false;
    }
    bool operator==(const uvec2& o) const {
        for(unsigned i=0;i<2;++i){
            if((*this)[i] != o[i])
                return false;
        }
        return true;
    }
    bool operator!=(const uvec2& o)const {
        return ! (*this == o);
    }
    operator std::string() const {
        std::ostringstream oss;
        oss << "vec" << 2 << "(";
        for(unsigned i=0;i<2;++i){
            oss << " " << (*this)[i];
        }
        oss << ")";
        return oss.str();
    }
    bool isZero() const {
        for(unsigned i=0;i<2;++i){
            if( (*this)[i] != 0 )
                return false;
        }
        return true;
    }
    unsigned size() const {
        return 2;
    }
};
inline
umat2 operator*( float f, const umat2& m ){
    return m*f;
}
inline
umat2 operator*( double f, const umat2& m ){
    return m*f;
}
inline
umat2 operator*( int f, const umat2& m ){
    return m*f;
}
inline
umat2 operator*( unsigned f, const umat2& m ){
    return m*f;
}
inline
uvec2 operator*( float f, const uvec2& m ){
    return m*f;
}
inline
uvec2 operator*( double f, const uvec2& m ){
    return m*f;
}
inline
uvec2 operator*( int f, const uvec2& m ){
    return m*f;
}
inline
uvec2 operator*( unsigned f, const uvec2& m ){
    return m*f;
}
inline
uvec2 operator*(const umat2& m, const uvec2& o) {
    //mat * vector
    uvec2 R;
    for(unsigned i=0;i<2;++i){
        uint32_t total = (uint32_t) 0;
        for(unsigned j=0;j<2;++j){
            total += m[i][j] * o[j];
        }
        R[i] = total;
    }
    return R;
}
//vector-matrix multiplication
inline
uvec2 operator*(const uvec2& v, const umat2& o){
    uvec2 R;
    for(unsigned i=0;i<2;++i){
        uint32_t total=0;
        for(unsigned j=0;j<2;++j){
            total += uint32_t(v[j])*uint32_t(o[j][i]);
        }
        R[i]=uint32_t(total);
    }
    return R;
}
inline
umat2 transpose(const umat2& m){
    return m.transpose();
}
inline
uint32_t dot(const uvec2& v, const uvec2& w){
    uint32_t total =0;
    for(unsigned i=0;i<2;++i){
        total += v[i] * w[i];
    }
    return total;
}
inline
float length(const uvec2& v){
    return (float)std::sqrt( dot(v,v) );
}
inline
std::ostream& operator<<(std::ostream& o, const uvec2& v){
    o << (std::string)v;
    return o;
}
inline
std::ostream& operator<<(std::ostream& o, const umat2& v){
    o << (std::string)v;
    return o;
}
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
class umat3 {
    public:
    //suitable for using in uniform buffer
    uint32_t _M[3][4];
    umat3() {
        for(int i=0;i<3;++i){
            for(int j=0;j<4;++j){
                _M[i][j]= (uint32_t) 0;
            }
        }
    }
    umat3( uint32_t m0, uint32_t m1, uint32_t m2,
                uint32_t m3, uint32_t m4, uint32_t m5,
                uint32_t m6, uint32_t m7, uint32_t m8) :
                    umat3({m0,m1,m2,m3,m4,m5,m6,m7,m8})
    {
        _M[0][0] = m0; _M[0][1] = m1; _M[0][2] = m2; _M[0][3] = 0;
        _M[1][0] = m3; _M[1][1] = m4; _M[1][2] = m5; _M[1][3] = 0;
        _M[2][0] = m6; _M[2][1] = m7; _M[2][2] = m8; _M[2][3] = 0;
    }
//included from instantiate.h
//matrix operators that work for all matN types
//Symbols defined in instantiate.h:
// MATSIZE: 2, 3, or 4
// MatType: type of the matrix that contains these
// VecType: The type of vector that goes with this matrix
    /** Initialize matrix from an initializer_list. */
    umat3(const std::initializer_list<uint32_t>& L){
        if( L.size() != 3*3 )
            throw std::runtime_error("Bad number of arguments");
        //quiet bogus VS warnings
            _M[0][0] = _M[0][1] = _M[0][2] = _M[0][3] = 0;
            _M[1][0] = _M[1][1] = _M[1][2] = _M[1][3] = 0;
            _M[2][0] = _M[2][1] = _M[2][2] = _M[2][3] = 0;
        auto x = L.begin();
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                _M[i][j] = *x++;
            }
        }
    }
    /** Matrix-matrix addition*/
    umat3 operator+(const umat3& m2) const {
        umat3 r;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                r[i][j] = (*this)[i][j] + m2[i][j];
            }
        }
        return r;
    }
    /** Matrix-matrix subtraction*/
    umat3 operator-(const umat3& m2) const {
        umat3 r;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                r[i][j] = (*this)[i][j] - m2[i][j];
            }
        }
        return r;
    }
    /** Matrix-matrix multiplication*/
    umat3 operator*(const umat3& o) const {
        //mat * mat
        umat3 R;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                uint32_t total=0;
                for(unsigned k=0;k<3;++k){
                    total += (*this)[i][k] * o[k][j];
                }
                R[i][j] = total;
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    umat3 operator*(float o) const {
        //mat * scalar
        umat3 R;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                R[i][j] = (*this)[i][j] * uint32_t(o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    umat3 operator*(double o) const {
        //mat * scalar
        umat3 R;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                R[i][j] = (*this)[i][j] * uint32_t(o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    umat3 operator*(int o) const {
        //mat * scalar
        umat3 R;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                R[i][j] = (*this)[i][j] *uint32_t( o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    umat3 operator*(unsigned o) const {
        //mat * scalar
        umat3 R;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                R[i][j] = (*this)[i][j] * uint32_t( o );
            }
        }
        return R;
    }
    //Vector<MATSIZE> operator*(const Vector<MATSIZE>& v) const ;
    /** Unary + */
    umat3 operator+() const {
        return *this;
    }
    /** Negation */
    umat3 operator-() const {
        return *this * -1.0f;
    }
    const void* tobytes() const {
        return _M;
    }
  private:
    class MatRow{
      public:
        umat3& m;
        unsigned i;
        MatRow(umat3& mm,int ii) : m(mm), i(ii){ }
        uint32_t& operator[](unsigned j){
            //each row is padded out to be a vec4
            if( j >= 3 )
                throw std::runtime_error("Bad index");
            return this->m._M[i][j];
        }
    };
    class CMatRow{
      public:
        const umat3& m;
        unsigned i;
        CMatRow(const umat3& mm,int ii) : m(mm), i(ii){ }
        uint32_t operator[](unsigned j) const {
            //each row is padded out to be a vec4
            if( j >= 3 )
                throw std::runtime_error("Bad index");
            //~ std::cout << "[" << i << "][" << j << "] = index " << i*4+j << "\n";
            return this->m._M[i][j];
        }
    };
  public:
    MatRow operator[](unsigned i) {
        return MatRow(*this,i);
    }
    CMatRow operator[](unsigned i) const {
        return CMatRow(*this,i);
    }
    bool operator==(const umat3& o) const {
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                if( (*this)[i][j] != o[i][j] )
                    return false;
            }
        }
        return true;
    }
    bool operator!=(const umat3& o) const {
        return ! (*this==o);
    }
    operator std::string() const {
        std::ostringstream oss;
        for(unsigned i=0;i<3;++i){
            oss << "[";
            for(unsigned j=0;j<3;++j){
                oss << (*this)[i][j] << "   ";
            }
            oss << "]\n";
        }
        return oss.str();
    }
    umat3 transpose() const {
        umat3 R;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                R[i][j] = (*this)[j][i];
            }
        }
        return R;
    }
    static umat3 identity(){
        umat3 R;
        for(unsigned i=0;i<3;++i){
            R[i][i]=1;
        }
        return R;
    }
    //no padding; not suitable for use in array or uniform buffer
    const std::array<uint32_t,3*3> data() const {
        std::array<uint32_t,3*3> rv;
        int k=0;
        for(int i=0;i<3;++i){
            for(int j=0;j<3;++j){
                rv[k++] = (*this)[i][j];
            }
        }
        return rv;
    }
};
class uvec3{
  public:
    uvec3(const uvec2&) = delete;
    uvec3(const uvec4&) = delete;
    public:
    uint32_t x=0,y=0,z=0;
    uvec3(){
        x=y=z=0;
    }
    template<typename T>
    uvec3(T q) {
        x=y=z=(uint32_t)q;
    }
    template<typename T>
    uvec3( uvec2 v, T z){
        this->x = (uint32_t) v.x;
        this->y = (uint32_t) v.y;
        this->z = (uint32_t) z;
    }
    uvec3(uint32_t x, uint32_t y, uint32_t z){
        this->x=x;
        this->y=y;
        this->z=z;
    }
    void setXY(uvec2 v){
        x=v.x;
        y=v.y;
    }
    const uvec2 xy() const{
        return uvec2(x,y);
    }
    const uvec3 xyz() const {
        return *this;
    }
    static uvec3 from(const float* fp){
        uvec3 v;
        for(unsigned i=0;i<3;++i)
            v[i] = uint32_t(fp[i]);
        return v;
    }
    uvec3(const std::initializer_list<uint32_t>& L){
        if( L.size() != 3 )
            throw std::runtime_error("Bad size for vector initializer");
        auto F = L.begin();
            this->x=*F++;
            this->y=*F++;
            this->z=*F++;
    }
    uint32_t& operator[](unsigned key){
        switch(key){
            case 0: return x;
            case 1: return y;
                case 2: return z;
            default: throw std::runtime_error("Bad index");
        }
    }
    uint32_t operator[](unsigned key) const{
        switch(key){
            case 0: return x;
            case 1: return y;
                case 2: return z;
            default: throw std::runtime_error("Bad index");
        }
    }
    void operator+=(const uvec3& o){
        for(unsigned i=0;i<3;++i)
            (*this)[i] += o[i];
    }
    void operator-=(const uvec3& o){
        for(unsigned i=0;i<3;++i)
            (*this)[i] -= o[i];
    }
    void operator*=(const uvec3& o){
        for(unsigned i=0;i<3;++i)
            (*this)[i] *= o[i];
    }
    void operator*=(const uint32_t o){
        for(unsigned i=0;i<3;++i)
            (*this)[i] *= o;
    }
    void operator/=(const uvec3& o){
        for(unsigned i=0;i<3;++i)
            (*this)[i] /= o[i];
    }
    uvec3 operator-()const {
        uvec3 res;
        for(unsigned i=0;i<3;++i){
            double tmp = (*this)[i];
            tmp = -tmp;
            res[i] = (uint32_t) tmp;
        }
        return res;
    }
    uvec3 operator+() const {
        uvec3 res = *this;
        return res;
    }
    uvec3 operator+(const uvec3& other) const {
        uvec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] + other[i];
        return res;
    }
    uvec3 operator-(const uvec3& other) const {
        uvec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] - other[i];
        return res;
    }
    uvec3 operator*(const uvec3& other) const {
        uvec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] * other[i];
        return res;
    }
    uvec3 operator/(const uvec3& other) const {
        uvec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] / other[i];
        return res;
    }
    uvec3 operator*(float other) const {
        uvec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] * uint32_t(other );
        return res;
    }
    uvec3 operator*(double other) const {
        uvec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] * uint32_t(other );
        return res;
    }
    uvec3 operator*(int other) const {
        uvec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] * uint32_t(other );
        return res;
    }
    uvec3 operator*(unsigned other) const {
        uvec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] * uint32_t( other );
        return res;
    }
    uvec3 operator/(float other) const {
        uvec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] / uint32_t(other);
        return res;
    }
    uvec3 operator/(double other) const {
        uvec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] / uint32_t(other);
        return res;
    }
    uvec3 operator/(int other) const {
        uvec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] / uint32_t(other);
        return res;
    }
    uvec3 operator/(unsigned other) const {
        uvec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] / uint32_t(other);
        return res;
    }
    bool operator<(const uvec3& o) const {
        for(unsigned i=0;i<3;++i){
            if( (*this)[i] != o[i] )
                return (*this)[i] < o[i];
        }
        return false;
    }
    bool operator==(const uvec3& o) const {
        for(unsigned i=0;i<3;++i){
            if((*this)[i] != o[i])
                return false;
        }
        return true;
    }
    bool operator!=(const uvec3& o)const {
        return ! (*this == o);
    }
    operator std::string() const {
        std::ostringstream oss;
        oss << "vec" << 3 << "(";
        for(unsigned i=0;i<3;++i){
            oss << " " << (*this)[i];
        }
        oss << ")";
        return oss.str();
    }
    bool isZero() const {
        for(unsigned i=0;i<3;++i){
            if( (*this)[i] != 0 )
                return false;
        }
        return true;
    }
    unsigned size() const {
        return 3;
    }
};
inline
umat3 operator*( float f, const umat3& m ){
    return m*f;
}
inline
umat3 operator*( double f, const umat3& m ){
    return m*f;
}
inline
umat3 operator*( int f, const umat3& m ){
    return m*f;
}
inline
umat3 operator*( unsigned f, const umat3& m ){
    return m*f;
}
inline
uvec3 operator*( float f, const uvec3& m ){
    return m*f;
}
inline
uvec3 operator*( double f, const uvec3& m ){
    return m*f;
}
inline
uvec3 operator*( int f, const uvec3& m ){
    return m*f;
}
inline
uvec3 operator*( unsigned f, const uvec3& m ){
    return m*f;
}
inline
uvec3 operator*(const umat3& m, const uvec3& o) {
    //mat * vector
    uvec3 R;
    for(unsigned i=0;i<3;++i){
        uint32_t total = (uint32_t) 0;
        for(unsigned j=0;j<3;++j){
            total += m[i][j] * o[j];
        }
        R[i] = total;
    }
    return R;
}
//vector-matrix multiplication
inline
uvec3 operator*(const uvec3& v, const umat3& o){
    uvec3 R;
    for(unsigned i=0;i<3;++i){
        uint32_t total=0;
        for(unsigned j=0;j<3;++j){
            total += uint32_t(v[j])*uint32_t(o[j][i]);
        }
        R[i]=uint32_t(total);
    }
    return R;
}
inline
umat3 transpose(const umat3& m){
    return m.transpose();
}
inline
uint32_t dot(const uvec3& v, const uvec3& w){
    uint32_t total =0;
    for(unsigned i=0;i<3;++i){
        total += v[i] * w[i];
    }
    return total;
}
inline
float length(const uvec3& v){
    return (float)std::sqrt( dot(v,v) );
}
inline
std::ostream& operator<<(std::ostream& o, const uvec3& v){
    o << (std::string)v;
    return o;
}
inline
std::ostream& operator<<(std::ostream& o, const umat3& v){
    o << (std::string)v;
    return o;
}
///////////////////////////////////////////////////////////////
class umat4 {
    public:
    //suitable for using in uniform buffer
    uint32_t _M[4][4];
    umat4() {
        _M[0][0] = 0; _M[0][1] = 0; _M[0][2] = 0; _M[0][3] = 0;
        _M[1][0] = 0; _M[1][1] = 0; _M[1][2] = 0; _M[1][3] = 0;
        _M[2][0] = 0; _M[2][1] = 0; _M[2][2] = 0; _M[2][3] = 0;
        _M[3][0] = 0; _M[3][1] = 0; _M[3][2] = 0; _M[3][3] = 0;
    }
    umat4( uint32_t m0, uint32_t m1, uint32_t m2, uint32_t m3,
                uint32_t m4, uint32_t m5, uint32_t m6, uint32_t m7,
                uint32_t m8, uint32_t m9, uint32_t ma, uint32_t mb,
                uint32_t mc, uint32_t md, uint32_t me, uint32_t mf)
    {
        _M[0][0] = m0; _M[0][1] = m1; _M[0][2] = m2; _M[0][3] = m3;
        _M[1][0] = m4; _M[1][1] = m5; _M[1][2] = m6; _M[1][3] = m7;
        _M[2][0] = m8; _M[2][1] = m9; _M[2][2] = ma; _M[2][3] = mb;
        _M[3][0] = mc; _M[3][1] = md; _M[3][2] = me; _M[3][3] = mf;
    }
    umat4( double m0, double m1, double m2, double m3,
                double m4, double m5, double m6, double m7,
                double m8, double m9, double ma, double mb,
                double mc, double md, double me, double mf)
    {
        _M[0][0] = (uint32_t) m0; _M[0][1] = (uint32_t) m1; _M[0][2] = (uint32_t) m2; _M[0][3] = (uint32_t) m3;
        _M[1][0] = (uint32_t) m4; _M[1][1] = (uint32_t) m5; _M[1][2] = (uint32_t) m6; _M[1][3] = (uint32_t) m7;
        _M[2][0] = (uint32_t) m8; _M[2][1] = (uint32_t) m9; _M[2][2] = (uint32_t) ma; _M[2][3] = (uint32_t) mb;
        _M[3][0] = (uint32_t) mc; _M[3][1] = (uint32_t) md; _M[3][2] = (uint32_t) me; _M[3][3] = (uint32_t) mf;
    }
    umat3 upper3x3() const
    {
       return umat3(
            (*this)[0][0], (*this)[0][1], (*this)[0][2],
            (*this)[1][0], (*this)[1][1], (*this)[1][2],
            (*this)[2][0], (*this)[2][1], (*this)[2][2]
        );
    }
//included from instantiate.h
//matrix operators that work for all matN types
//Symbols defined in instantiate.h:
// MATSIZE: 2, 3, or 4
// MatType: type of the matrix that contains these
// VecType: The type of vector that goes with this matrix
    /** Initialize matrix from an initializer_list. */
    umat4(const std::initializer_list<uint32_t>& L){
        if( L.size() != 4*4 )
            throw std::runtime_error("Bad number of arguments");
        //quiet bogus VS warnings
            _M[0][0] = _M[0][1] = _M[0][2] = _M[0][3] = 0;
            _M[1][0] = _M[1][1] = _M[1][2] = _M[1][3] = 0;
            _M[2][0] = _M[2][1] = _M[2][2] = _M[2][3] = 0;
            _M[3][0] = _M[3][1] = _M[3][2] = _M[3][3] = 0;
        auto x = L.begin();
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                _M[i][j] = *x++;
            }
        }
    }
    /** Matrix-matrix addition*/
    umat4 operator+(const umat4& m2) const {
        umat4 r;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                r[i][j] = (*this)[i][j] + m2[i][j];
            }
        }
        return r;
    }
    /** Matrix-matrix subtraction*/
    umat4 operator-(const umat4& m2) const {
        umat4 r;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                r[i][j] = (*this)[i][j] - m2[i][j];
            }
        }
        return r;
    }
    /** Matrix-matrix multiplication*/
    umat4 operator*(const umat4& o) const {
        //mat * mat
        umat4 R;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                uint32_t total=0;
                for(unsigned k=0;k<4;++k){
                    total += (*this)[i][k] * o[k][j];
                }
                R[i][j] = total;
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    umat4 operator*(float o) const {
        //mat * scalar
        umat4 R;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                R[i][j] = (*this)[i][j] * uint32_t(o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    umat4 operator*(double o) const {
        //mat * scalar
        umat4 R;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                R[i][j] = (*this)[i][j] * uint32_t(o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    umat4 operator*(int o) const {
        //mat * scalar
        umat4 R;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                R[i][j] = (*this)[i][j] *uint32_t( o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    umat4 operator*(unsigned o) const {
        //mat * scalar
        umat4 R;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                R[i][j] = (*this)[i][j] * uint32_t( o );
            }
        }
        return R;
    }
    //Vector<MATSIZE> operator*(const Vector<MATSIZE>& v) const ;
    /** Unary + */
    umat4 operator+() const {
        return *this;
    }
    /** Negation */
    umat4 operator-() const {
        return *this * -1.0f;
    }
    const void* tobytes() const {
        return _M;
    }
  private:
    class MatRow{
      public:
        umat4& m;
        unsigned i;
        MatRow(umat4& mm,int ii) : m(mm), i(ii){ }
        uint32_t& operator[](unsigned j){
            //each row is padded out to be a vec4
            if( j >= 4 )
                throw std::runtime_error("Bad index");
            return this->m._M[i][j];
        }
    };
    class CMatRow{
      public:
        const umat4& m;
        unsigned i;
        CMatRow(const umat4& mm,int ii) : m(mm), i(ii){ }
        uint32_t operator[](unsigned j) const {
            //each row is padded out to be a vec4
            if( j >= 4 )
                throw std::runtime_error("Bad index");
            //~ std::cout << "[" << i << "][" << j << "] = index " << i*4+j << "\n";
            return this->m._M[i][j];
        }
    };
  public:
    MatRow operator[](unsigned i) {
        return MatRow(*this,i);
    }
    CMatRow operator[](unsigned i) const {
        return CMatRow(*this,i);
    }
    bool operator==(const umat4& o) const {
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                if( (*this)[i][j] != o[i][j] )
                    return false;
            }
        }
        return true;
    }
    bool operator!=(const umat4& o) const {
        return ! (*this==o);
    }
    operator std::string() const {
        std::ostringstream oss;
        for(unsigned i=0;i<4;++i){
            oss << "[";
            for(unsigned j=0;j<4;++j){
                oss << (*this)[i][j] << "   ";
            }
            oss << "]\n";
        }
        return oss.str();
    }
    umat4 transpose() const {
        umat4 R;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                R[i][j] = (*this)[j][i];
            }
        }
        return R;
    }
    static umat4 identity(){
        umat4 R;
        for(unsigned i=0;i<4;++i){
            R[i][i]=1;
        }
        return R;
    }
    //no padding; not suitable for use in array or uniform buffer
    const std::array<uint32_t,4*4> data() const {
        std::array<uint32_t,4*4> rv;
        int k=0;
        for(int i=0;i<4;++i){
            for(int j=0;j<4;++j){
                rv[k++] = (*this)[i][j];
            }
        }
        return rv;
    }
};
class uvec4{
    uvec4(const uvec2&) = delete;
    uvec4(const uvec3&) = delete;
    public:
    uint32_t x,y,z,w;
    uvec4(){
        x=y=z=w=0;
    }
    template<typename T>
    uvec4(T q) {
        x=y=z=w=(uint32_t)q;
    }
    template<typename T1, typename T2, typename T3, typename T4>
    uvec4( T1 x, T2 y, T3 z, T4 w){
        this->x=(uint32_t) x;
        this->y=(uint32_t) y;
        this->z=(uint32_t) z;
        this->w=(uint32_t) w;
    }
    template<typename T>
    uvec4(const uvec3& v, T w) {
        this->x = (uint32_t) v.x;
        this->y = (uint32_t) v.y;
        this->z = (uint32_t) v.z;
        this->w = (uint32_t) w;
    }
    void setXYZ(uvec3 v){
        x=v.x;
        y=v.y;
        z=v.z;
    }
    void setXY(uvec2 v){
        x=v.x;
        y=v.y;
    }
    const uvec2 xy() const {
        return uvec2(x,y);
    }
    const uvec3 xyz() const {
        return uvec3(x,y,z);
    }
    const uvec4 xyzw() const {
        return *this;
    }
    static uvec4 from(const float* fp){
        uvec4 v;
        for(unsigned i=0;i<4;++i)
            v[i] = uint32_t(fp[i]);
        return v;
    }
    uvec4(const std::initializer_list<uint32_t>& L){
        if( L.size() != 4 )
            throw std::runtime_error("Bad size for vector initializer");
        auto F = L.begin();
            this->w=*F++;
    }
    uint32_t& operator[](unsigned key){
        switch(key){
            case 0: return x;
            case 1: return y;
                case 2: return z;
                case 3: return w;
            default: throw std::runtime_error("Bad index");
        }
    }
    uint32_t operator[](unsigned key) const{
        switch(key){
            case 0: return x;
            case 1: return y;
                case 2: return z;
                case 3: return w;
            default: throw std::runtime_error("Bad index");
        }
    }
    void operator+=(const uvec4& o){
        for(unsigned i=0;i<4;++i)
            (*this)[i] += o[i];
    }
    void operator-=(const uvec4& o){
        for(unsigned i=0;i<4;++i)
            (*this)[i] -= o[i];
    }
    void operator*=(const uvec4& o){
        for(unsigned i=0;i<4;++i)
            (*this)[i] *= o[i];
    }
    void operator*=(const uint32_t o){
        for(unsigned i=0;i<4;++i)
            (*this)[i] *= o;
    }
    void operator/=(const uvec4& o){
        for(unsigned i=0;i<4;++i)
            (*this)[i] /= o[i];
    }
    uvec4 operator-()const {
        uvec4 res;
        for(unsigned i=0;i<4;++i){
            double tmp = (*this)[i];
            tmp = -tmp;
            res[i] = (uint32_t) tmp;
        }
        return res;
    }
    uvec4 operator+() const {
        uvec4 res = *this;
        return res;
    }
    uvec4 operator+(const uvec4& other) const {
        uvec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] + other[i];
        return res;
    }
    uvec4 operator-(const uvec4& other) const {
        uvec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] - other[i];
        return res;
    }
    uvec4 operator*(const uvec4& other) const {
        uvec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] * other[i];
        return res;
    }
    uvec4 operator/(const uvec4& other) const {
        uvec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] / other[i];
        return res;
    }
    uvec4 operator*(float other) const {
        uvec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] * uint32_t(other );
        return res;
    }
    uvec4 operator*(double other) const {
        uvec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] * uint32_t(other );
        return res;
    }
    uvec4 operator*(int other) const {
        uvec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] * uint32_t(other );
        return res;
    }
    uvec4 operator*(unsigned other) const {
        uvec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] * uint32_t( other );
        return res;
    }
    uvec4 operator/(float other) const {
        uvec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] / uint32_t(other);
        return res;
    }
    uvec4 operator/(double other) const {
        uvec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] / uint32_t(other);
        return res;
    }
    uvec4 operator/(int other) const {
        uvec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] / uint32_t(other);
        return res;
    }
    uvec4 operator/(unsigned other) const {
        uvec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] / uint32_t(other);
        return res;
    }
    bool operator<(const uvec4& o) const {
        for(unsigned i=0;i<4;++i){
            if( (*this)[i] != o[i] )
                return (*this)[i] < o[i];
        }
        return false;
    }
    bool operator==(const uvec4& o) const {
        for(unsigned i=0;i<4;++i){
            if((*this)[i] != o[i])
                return false;
        }
        return true;
    }
    bool operator!=(const uvec4& o)const {
        return ! (*this == o);
    }
    operator std::string() const {
        std::ostringstream oss;
        oss << "vec" << 4 << "(";
        for(unsigned i=0;i<4;++i){
            oss << " " << (*this)[i];
        }
        oss << ")";
        return oss.str();
    }
    bool isZero() const {
        for(unsigned i=0;i<4;++i){
            if( (*this)[i] != 0 )
                return false;
        }
        return true;
    }
    unsigned size() const {
        return 4;
    }
};
inline
umat4 operator*( float f, const umat4& m ){
    return m*f;
}
inline
umat4 operator*( double f, const umat4& m ){
    return m*f;
}
inline
umat4 operator*( int f, const umat4& m ){
    return m*f;
}
inline
umat4 operator*( unsigned f, const umat4& m ){
    return m*f;
}
inline
uvec4 operator*( float f, const uvec4& m ){
    return m*f;
}
inline
uvec4 operator*( double f, const uvec4& m ){
    return m*f;
}
inline
uvec4 operator*( int f, const uvec4& m ){
    return m*f;
}
inline
uvec4 operator*( unsigned f, const uvec4& m ){
    return m*f;
}
inline
uvec4 operator*(const umat4& m, const uvec4& o) {
    //mat * vector
    uvec4 R;
    for(unsigned i=0;i<4;++i){
        uint32_t total = (uint32_t) 0;
        for(unsigned j=0;j<4;++j){
            total += m[i][j] * o[j];
        }
        R[i] = total;
    }
    return R;
}
//vector-matrix multiplication
inline
uvec4 operator*(const uvec4& v, const umat4& o){
    uvec4 R;
    for(unsigned i=0;i<4;++i){
        uint32_t total=0;
        for(unsigned j=0;j<4;++j){
            total += uint32_t(v[j])*uint32_t(o[j][i]);
        }
        R[i]=uint32_t(total);
    }
    return R;
}
inline
umat4 transpose(const umat4& m){
    return m.transpose();
}
inline
uint32_t dot(const uvec4& v, const uvec4& w){
    uint32_t total =0;
    for(unsigned i=0;i<4;++i){
        total += v[i] * w[i];
    }
    return total;
}
inline
float length(const uvec4& v){
    return (float)std::sqrt( dot(v,v) );
}
inline
std::ostream& operator<<(std::ostream& o, const uvec4& v){
    o << (std::string)v;
    return o;
}
inline
std::ostream& operator<<(std::ostream& o, const umat4& v){
    o << (std::string)v;
    return o;
}
///////////////////////////////////////////////////////////////
//only defined for vec3/ivec3/uvec3
inline
uvec3 cross(const uvec3& v, const uvec3& w){
    return uvec3(
        v.y*w.z - w.y*v.z,
        w.x*v.z - v.x*w.z,
        v.x*w.y - w.x*v.y
    );
}
//~ //vec4::xy()
//~ inline const VEC2TYPE VEC4TYPE::xy() const 
//~ { 
    //~ return VEC2TYPE(x,y); 
//~ }
//~ //vec4::xyz()
//~ inline const VEC3TYPE VEC4TYPE::xyz() const 
//~ { 
    //~ return VEC3TYPE(x,y,z); 
//~ }
//~ //vec3.xy()
//~ inline const VEC2TYPE VEC3TYPE::xy() const 
//~ { 
    //~ return VEC2TYPE(x,y); 
//~ }
//~ inline void VEC4TYPE::setXYZ(VEC3TYPE v){
    //~ x=v.x;
    //~ y=v.y;
    //~ z=v.z;
//~ }
//~ inline void VEC4TYPE::setXY(VEC2TYPE v){
    //~ x=v.x;
    //~ y=v.y;
//~ }
//~ inline void VEC3TYPE::setXY(VEC2TYPE v){
    //~ x=v.x;
    //~ y=v.y;
//~ }
//included from gen.h
//instantiates matN and vecN for one type (float, int, uint) 
//macro trickery to expand macros into real symbols.
//we need to do two levels of expansion to get
//the behavior we want; see
//https://stackoverflow.com/questions/1489932/how-to-concatenate-twice-with-the-c-preprocessor-and-expand-a-macro-as-in-arg
//float, uint32_t, etc.
//~ #define VEC2TYPE concat(vec2_ , SCALARTYPE1)
//~ #define VEC3TYPE concat(vec3_ , SCALARTYPE1)
//~ #define VEC4TYPE concat(vec4_ , SCALARTYPE1)
//~ #define MAT2TYPE concat( mat2_ , SCALARTYPE1 )
//~ #define MAT3TYPE concat( mat3_ , SCALARTYPE1 )
//~ #define MAT4TYPE concat( mat4_ , SCALARTYPE1 )
class ivec4;
class ivec3;
class ivec2;
class imat4;
class imat3;
class imat2;
/////////////////////////////////////////////////////////////////////
class imat2 {
    public:
    //suitable for using in uniform buffer
    int32_t _M[2][4];
    imat2() {
        _M[0][0] = 0; _M[0][1] = 0; _M[0][2] = 0; _M[0][3] = 0;
        _M[1][0] = 0; _M[1][1] = 0; _M[1][2] = 0; _M[1][3] = 0;
    }
    imat2( int32_t m0, int32_t m1,
                int32_t m2, int32_t m3)
    {
        _M[0][0] = m0; _M[0][1] = m1; _M[0][2] = 0; _M[0][3] = 0;
        _M[1][0] = m2; _M[1][1] = m3; _M[1][2] = 0; _M[1][3] = 0;
    }
//included from instantiate.h
//matrix operators that work for all matN types
//Symbols defined in instantiate.h:
// MATSIZE: 2, 3, or 4
// MatType: type of the matrix that contains these
// VecType: The type of vector that goes with this matrix
    /** Initialize matrix from an initializer_list. */
    imat2(const std::initializer_list<int32_t>& L){
        if( L.size() != 2*2 )
            throw std::runtime_error("Bad number of arguments");
        //quiet bogus VS warnings
            _M[0][0] = _M[0][1] = _M[0][2] = _M[0][3] = 0;
            _M[1][0] = _M[1][1] = _M[1][2] = _M[1][3] = 0;
        auto x = L.begin();
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                _M[i][j] = *x++;
            }
        }
    }
    /** Matrix-matrix addition*/
    imat2 operator+(const imat2& m2) const {
        imat2 r;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                r[i][j] = (*this)[i][j] + m2[i][j];
            }
        }
        return r;
    }
    /** Matrix-matrix subtraction*/
    imat2 operator-(const imat2& m2) const {
        imat2 r;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                r[i][j] = (*this)[i][j] - m2[i][j];
            }
        }
        return r;
    }
    /** Matrix-matrix multiplication*/
    imat2 operator*(const imat2& o) const {
        //mat * mat
        imat2 R;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                int32_t total=0;
                for(unsigned k=0;k<2;++k){
                    total += (*this)[i][k] * o[k][j];
                }
                R[i][j] = total;
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    imat2 operator*(float o) const {
        //mat * scalar
        imat2 R;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                R[i][j] = (*this)[i][j] * int32_t(o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    imat2 operator*(double o) const {
        //mat * scalar
        imat2 R;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                R[i][j] = (*this)[i][j] * int32_t(o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    imat2 operator*(int o) const {
        //mat * scalar
        imat2 R;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                R[i][j] = (*this)[i][j] *int32_t( o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    imat2 operator*(unsigned o) const {
        //mat * scalar
        imat2 R;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                R[i][j] = (*this)[i][j] * int32_t( o );
            }
        }
        return R;
    }
    //Vector<MATSIZE> operator*(const Vector<MATSIZE>& v) const ;
    /** Unary + */
    imat2 operator+() const {
        return *this;
    }
    /** Negation */
    imat2 operator-() const {
        return *this * -1.0f;
    }
    const void* tobytes() const {
        return _M;
    }
  private:
    class MatRow{
      public:
        imat2& m;
        unsigned i;
        MatRow(imat2& mm,int ii) : m(mm), i(ii){ }
        int32_t& operator[](unsigned j){
            //each row is padded out to be a vec4
            if( j >= 2 )
                throw std::runtime_error("Bad index");
            return this->m._M[i][j];
        }
    };
    class CMatRow{
      public:
        const imat2& m;
        unsigned i;
        CMatRow(const imat2& mm,int ii) : m(mm), i(ii){ }
        int32_t operator[](unsigned j) const {
            //each row is padded out to be a vec4
            if( j >= 2 )
                throw std::runtime_error("Bad index");
            //~ std::cout << "[" << i << "][" << j << "] = index " << i*4+j << "\n";
            return this->m._M[i][j];
        }
    };
  public:
    MatRow operator[](unsigned i) {
        return MatRow(*this,i);
    }
    CMatRow operator[](unsigned i) const {
        return CMatRow(*this,i);
    }
    bool operator==(const imat2& o) const {
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                if( (*this)[i][j] != o[i][j] )
                    return false;
            }
        }
        return true;
    }
    bool operator!=(const imat2& o) const {
        return ! (*this==o);
    }
    operator std::string() const {
        std::ostringstream oss;
        for(unsigned i=0;i<2;++i){
            oss << "[";
            for(unsigned j=0;j<2;++j){
                oss << (*this)[i][j] << "   ";
            }
            oss << "]\n";
        }
        return oss.str();
    }
    imat2 transpose() const {
        imat2 R;
        for(unsigned i=0;i<2;++i){
            for(unsigned j=0;j<2;++j){
                R[i][j] = (*this)[j][i];
            }
        }
        return R;
    }
    static imat2 identity(){
        imat2 R;
        for(unsigned i=0;i<2;++i){
            R[i][i]=1;
        }
        return R;
    }
    //no padding; not suitable for use in array or uniform buffer
    const std::array<int32_t,2*2> data() const {
        std::array<int32_t,2*2> rv;
        int k=0;
        for(int i=0;i<2;++i){
            for(int j=0;j<2;++j){
                rv[k++] = (*this)[i][j];
            }
        }
        return rv;
    }
};
class ivec2{
  public:
    ivec2(const ivec3&) = delete;
    ivec2(const ivec4&) = delete;
    public:
    int32_t x,y;
    ivec2(){
        this->x=0;
        this->y=0;
    }
    template<typename T1>
    ivec2(T1 x){
        this->x = this->y = (int32_t) x;
    }
    template<typename T1, typename T2>
    ivec2(T1 x, T2 y){
        this->x = (int32_t) x;
        this->y = (int32_t) y;
    }
    const ivec2 xy() const {
        return *this;
    }
    static ivec2 from(const float* fp){
        ivec2 v;
        for(unsigned i=0;i<2;++i)
            v[i] = int32_t(fp[i]);
        return v;
    }
    ivec2(const std::initializer_list<int32_t>& L){
        if( L.size() != 2 )
            throw std::runtime_error("Bad size for vector initializer");
        auto F = L.begin();
            this->x=*F++;
            this->y=*F++;
    }
    int32_t& operator[](unsigned key){
        switch(key){
            case 0: return x;
            case 1: return y;
            default: throw std::runtime_error("Bad index");
        }
    }
    int32_t operator[](unsigned key) const{
        switch(key){
            case 0: return x;
            case 1: return y;
            default: throw std::runtime_error("Bad index");
        }
    }
    void operator+=(const ivec2& o){
        for(unsigned i=0;i<2;++i)
            (*this)[i] += o[i];
    }
    void operator-=(const ivec2& o){
        for(unsigned i=0;i<2;++i)
            (*this)[i] -= o[i];
    }
    void operator*=(const ivec2& o){
        for(unsigned i=0;i<2;++i)
            (*this)[i] *= o[i];
    }
    void operator*=(const int32_t o){
        for(unsigned i=0;i<2;++i)
            (*this)[i] *= o;
    }
    void operator/=(const ivec2& o){
        for(unsigned i=0;i<2;++i)
            (*this)[i] /= o[i];
    }
    ivec2 operator-()const {
        ivec2 res;
        for(unsigned i=0;i<2;++i){
            double tmp = (*this)[i];
            tmp = -tmp;
            res[i] = (int32_t) tmp;
        }
        return res;
    }
    ivec2 operator+() const {
        ivec2 res = *this;
        return res;
    }
    ivec2 operator+(const ivec2& other) const {
        ivec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] + other[i];
        return res;
    }
    ivec2 operator-(const ivec2& other) const {
        ivec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] - other[i];
        return res;
    }
    ivec2 operator*(const ivec2& other) const {
        ivec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] * other[i];
        return res;
    }
    ivec2 operator/(const ivec2& other) const {
        ivec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] / other[i];
        return res;
    }
    ivec2 operator*(float other) const {
        ivec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] * int32_t(other );
        return res;
    }
    ivec2 operator*(double other) const {
        ivec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] * int32_t(other );
        return res;
    }
    ivec2 operator*(int other) const {
        ivec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] * int32_t(other );
        return res;
    }
    ivec2 operator*(unsigned other) const {
        ivec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] * int32_t( other );
        return res;
    }
    ivec2 operator/(float other) const {
        ivec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] / int32_t(other);
        return res;
    }
    ivec2 operator/(double other) const {
        ivec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] / int32_t(other);
        return res;
    }
    ivec2 operator/(int other) const {
        ivec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] / int32_t(other);
        return res;
    }
    ivec2 operator/(unsigned other) const {
        ivec2 res;
        for(unsigned i=0;i<2;++i)
            res[i] = (*this)[i] / int32_t(other);
        return res;
    }
    bool operator<(const ivec2& o) const {
        for(unsigned i=0;i<2;++i){
            if( (*this)[i] != o[i] )
                return (*this)[i] < o[i];
        }
        return false;
    }
    bool operator==(const ivec2& o) const {
        for(unsigned i=0;i<2;++i){
            if((*this)[i] != o[i])
                return false;
        }
        return true;
    }
    bool operator!=(const ivec2& o)const {
        return ! (*this == o);
    }
    operator std::string() const {
        std::ostringstream oss;
        oss << "vec" << 2 << "(";
        for(unsigned i=0;i<2;++i){
            oss << " " << (*this)[i];
        }
        oss << ")";
        return oss.str();
    }
    bool isZero() const {
        for(unsigned i=0;i<2;++i){
            if( (*this)[i] != 0 )
                return false;
        }
        return true;
    }
    unsigned size() const {
        return 2;
    }
};
inline
imat2 operator*( float f, const imat2& m ){
    return m*f;
}
inline
imat2 operator*( double f, const imat2& m ){
    return m*f;
}
inline
imat2 operator*( int f, const imat2& m ){
    return m*f;
}
inline
imat2 operator*( unsigned f, const imat2& m ){
    return m*f;
}
inline
ivec2 operator*( float f, const ivec2& m ){
    return m*f;
}
inline
ivec2 operator*( double f, const ivec2& m ){
    return m*f;
}
inline
ivec2 operator*( int f, const ivec2& m ){
    return m*f;
}
inline
ivec2 operator*( unsigned f, const ivec2& m ){
    return m*f;
}
inline
ivec2 operator*(const imat2& m, const ivec2& o) {
    //mat * vector
    ivec2 R;
    for(unsigned i=0;i<2;++i){
        int32_t total = (int32_t) 0;
        for(unsigned j=0;j<2;++j){
            total += m[i][j] * o[j];
        }
        R[i] = total;
    }
    return R;
}
//vector-matrix multiplication
inline
ivec2 operator*(const ivec2& v, const imat2& o){
    ivec2 R;
    for(unsigned i=0;i<2;++i){
        int32_t total=0;
        for(unsigned j=0;j<2;++j){
            total += int32_t(v[j])*int32_t(o[j][i]);
        }
        R[i]=int32_t(total);
    }
    return R;
}
inline
imat2 transpose(const imat2& m){
    return m.transpose();
}
inline
int32_t dot(const ivec2& v, const ivec2& w){
    int32_t total =0;
    for(unsigned i=0;i<2;++i){
        total += v[i] * w[i];
    }
    return total;
}
inline
float length(const ivec2& v){
    return (float)std::sqrt( dot(v,v) );
}
inline
std::ostream& operator<<(std::ostream& o, const ivec2& v){
    o << (std::string)v;
    return o;
}
inline
std::ostream& operator<<(std::ostream& o, const imat2& v){
    o << (std::string)v;
    return o;
}
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
class imat3 {
    public:
    //suitable for using in uniform buffer
    int32_t _M[3][4];
    imat3() {
        for(int i=0;i<3;++i){
            for(int j=0;j<4;++j){
                _M[i][j]= (int32_t) 0;
            }
        }
    }
    imat3( int32_t m0, int32_t m1, int32_t m2,
                int32_t m3, int32_t m4, int32_t m5,
                int32_t m6, int32_t m7, int32_t m8) :
                    imat3({m0,m1,m2,m3,m4,m5,m6,m7,m8})
    {
        _M[0][0] = m0; _M[0][1] = m1; _M[0][2] = m2; _M[0][3] = 0;
        _M[1][0] = m3; _M[1][1] = m4; _M[1][2] = m5; _M[1][3] = 0;
        _M[2][0] = m6; _M[2][1] = m7; _M[2][2] = m8; _M[2][3] = 0;
    }
//included from instantiate.h
//matrix operators that work for all matN types
//Symbols defined in instantiate.h:
// MATSIZE: 2, 3, or 4
// MatType: type of the matrix that contains these
// VecType: The type of vector that goes with this matrix
    /** Initialize matrix from an initializer_list. */
    imat3(const std::initializer_list<int32_t>& L){
        if( L.size() != 3*3 )
            throw std::runtime_error("Bad number of arguments");
        //quiet bogus VS warnings
            _M[0][0] = _M[0][1] = _M[0][2] = _M[0][3] = 0;
            _M[1][0] = _M[1][1] = _M[1][2] = _M[1][3] = 0;
            _M[2][0] = _M[2][1] = _M[2][2] = _M[2][3] = 0;
        auto x = L.begin();
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                _M[i][j] = *x++;
            }
        }
    }
    /** Matrix-matrix addition*/
    imat3 operator+(const imat3& m2) const {
        imat3 r;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                r[i][j] = (*this)[i][j] + m2[i][j];
            }
        }
        return r;
    }
    /** Matrix-matrix subtraction*/
    imat3 operator-(const imat3& m2) const {
        imat3 r;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                r[i][j] = (*this)[i][j] - m2[i][j];
            }
        }
        return r;
    }
    /** Matrix-matrix multiplication*/
    imat3 operator*(const imat3& o) const {
        //mat * mat
        imat3 R;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                int32_t total=0;
                for(unsigned k=0;k<3;++k){
                    total += (*this)[i][k] * o[k][j];
                }
                R[i][j] = total;
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    imat3 operator*(float o) const {
        //mat * scalar
        imat3 R;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                R[i][j] = (*this)[i][j] * int32_t(o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    imat3 operator*(double o) const {
        //mat * scalar
        imat3 R;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                R[i][j] = (*this)[i][j] * int32_t(o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    imat3 operator*(int o) const {
        //mat * scalar
        imat3 R;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                R[i][j] = (*this)[i][j] *int32_t( o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    imat3 operator*(unsigned o) const {
        //mat * scalar
        imat3 R;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                R[i][j] = (*this)[i][j] * int32_t( o );
            }
        }
        return R;
    }
    //Vector<MATSIZE> operator*(const Vector<MATSIZE>& v) const ;
    /** Unary + */
    imat3 operator+() const {
        return *this;
    }
    /** Negation */
    imat3 operator-() const {
        return *this * -1.0f;
    }
    const void* tobytes() const {
        return _M;
    }
  private:
    class MatRow{
      public:
        imat3& m;
        unsigned i;
        MatRow(imat3& mm,int ii) : m(mm), i(ii){ }
        int32_t& operator[](unsigned j){
            //each row is padded out to be a vec4
            if( j >= 3 )
                throw std::runtime_error("Bad index");
            return this->m._M[i][j];
        }
    };
    class CMatRow{
      public:
        const imat3& m;
        unsigned i;
        CMatRow(const imat3& mm,int ii) : m(mm), i(ii){ }
        int32_t operator[](unsigned j) const {
            //each row is padded out to be a vec4
            if( j >= 3 )
                throw std::runtime_error("Bad index");
            //~ std::cout << "[" << i << "][" << j << "] = index " << i*4+j << "\n";
            return this->m._M[i][j];
        }
    };
  public:
    MatRow operator[](unsigned i) {
        return MatRow(*this,i);
    }
    CMatRow operator[](unsigned i) const {
        return CMatRow(*this,i);
    }
    bool operator==(const imat3& o) const {
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                if( (*this)[i][j] != o[i][j] )
                    return false;
            }
        }
        return true;
    }
    bool operator!=(const imat3& o) const {
        return ! (*this==o);
    }
    operator std::string() const {
        std::ostringstream oss;
        for(unsigned i=0;i<3;++i){
            oss << "[";
            for(unsigned j=0;j<3;++j){
                oss << (*this)[i][j] << "   ";
            }
            oss << "]\n";
        }
        return oss.str();
    }
    imat3 transpose() const {
        imat3 R;
        for(unsigned i=0;i<3;++i){
            for(unsigned j=0;j<3;++j){
                R[i][j] = (*this)[j][i];
            }
        }
        return R;
    }
    static imat3 identity(){
        imat3 R;
        for(unsigned i=0;i<3;++i){
            R[i][i]=1;
        }
        return R;
    }
    //no padding; not suitable for use in array or uniform buffer
    const std::array<int32_t,3*3> data() const {
        std::array<int32_t,3*3> rv;
        int k=0;
        for(int i=0;i<3;++i){
            for(int j=0;j<3;++j){
                rv[k++] = (*this)[i][j];
            }
        }
        return rv;
    }
};
class ivec3{
  public:
    ivec3(const ivec2&) = delete;
    ivec3(const ivec4&) = delete;
    public:
    int32_t x=0,y=0,z=0;
    ivec3(){
        x=y=z=0;
    }
    template<typename T>
    ivec3(T q) {
        x=y=z=(int32_t)q;
    }
    template<typename T>
    ivec3( ivec2 v, T z){
        this->x = (int32_t) v.x;
        this->y = (int32_t) v.y;
        this->z = (int32_t) z;
    }
    ivec3(int32_t x, int32_t y, int32_t z){
        this->x=x;
        this->y=y;
        this->z=z;
    }
    void setXY(ivec2 v){
        x=v.x;
        y=v.y;
    }
    const ivec2 xy() const{
        return ivec2(x,y);
    }
    const ivec3 xyz() const {
        return *this;
    }
    static ivec3 from(const float* fp){
        ivec3 v;
        for(unsigned i=0;i<3;++i)
            v[i] = int32_t(fp[i]);
        return v;
    }
    ivec3(const std::initializer_list<int32_t>& L){
        if( L.size() != 3 )
            throw std::runtime_error("Bad size for vector initializer");
        auto F = L.begin();
            this->x=*F++;
            this->y=*F++;
            this->z=*F++;
    }
    int32_t& operator[](unsigned key){
        switch(key){
            case 0: return x;
            case 1: return y;
                case 2: return z;
            default: throw std::runtime_error("Bad index");
        }
    }
    int32_t operator[](unsigned key) const{
        switch(key){
            case 0: return x;
            case 1: return y;
                case 2: return z;
            default: throw std::runtime_error("Bad index");
        }
    }
    void operator+=(const ivec3& o){
        for(unsigned i=0;i<3;++i)
            (*this)[i] += o[i];
    }
    void operator-=(const ivec3& o){
        for(unsigned i=0;i<3;++i)
            (*this)[i] -= o[i];
    }
    void operator*=(const ivec3& o){
        for(unsigned i=0;i<3;++i)
            (*this)[i] *= o[i];
    }
    void operator*=(const int32_t o){
        for(unsigned i=0;i<3;++i)
            (*this)[i] *= o;
    }
    void operator/=(const ivec3& o){
        for(unsigned i=0;i<3;++i)
            (*this)[i] /= o[i];
    }
    ivec3 operator-()const {
        ivec3 res;
        for(unsigned i=0;i<3;++i){
            double tmp = (*this)[i];
            tmp = -tmp;
            res[i] = (int32_t) tmp;
        }
        return res;
    }
    ivec3 operator+() const {
        ivec3 res = *this;
        return res;
    }
    ivec3 operator+(const ivec3& other) const {
        ivec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] + other[i];
        return res;
    }
    ivec3 operator-(const ivec3& other) const {
        ivec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] - other[i];
        return res;
    }
    ivec3 operator*(const ivec3& other) const {
        ivec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] * other[i];
        return res;
    }
    ivec3 operator/(const ivec3& other) const {
        ivec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] / other[i];
        return res;
    }
    ivec3 operator*(float other) const {
        ivec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] * int32_t(other );
        return res;
    }
    ivec3 operator*(double other) const {
        ivec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] * int32_t(other );
        return res;
    }
    ivec3 operator*(int other) const {
        ivec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] * int32_t(other );
        return res;
    }
    ivec3 operator*(unsigned other) const {
        ivec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] * int32_t( other );
        return res;
    }
    ivec3 operator/(float other) const {
        ivec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] / int32_t(other);
        return res;
    }
    ivec3 operator/(double other) const {
        ivec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] / int32_t(other);
        return res;
    }
    ivec3 operator/(int other) const {
        ivec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] / int32_t(other);
        return res;
    }
    ivec3 operator/(unsigned other) const {
        ivec3 res;
        for(unsigned i=0;i<3;++i)
            res[i] = (*this)[i] / int32_t(other);
        return res;
    }
    bool operator<(const ivec3& o) const {
        for(unsigned i=0;i<3;++i){
            if( (*this)[i] != o[i] )
                return (*this)[i] < o[i];
        }
        return false;
    }
    bool operator==(const ivec3& o) const {
        for(unsigned i=0;i<3;++i){
            if((*this)[i] != o[i])
                return false;
        }
        return true;
    }
    bool operator!=(const ivec3& o)const {
        return ! (*this == o);
    }
    operator std::string() const {
        std::ostringstream oss;
        oss << "vec" << 3 << "(";
        for(unsigned i=0;i<3;++i){
            oss << " " << (*this)[i];
        }
        oss << ")";
        return oss.str();
    }
    bool isZero() const {
        for(unsigned i=0;i<3;++i){
            if( (*this)[i] != 0 )
                return false;
        }
        return true;
    }
    unsigned size() const {
        return 3;
    }
};
inline
imat3 operator*( float f, const imat3& m ){
    return m*f;
}
inline
imat3 operator*( double f, const imat3& m ){
    return m*f;
}
inline
imat3 operator*( int f, const imat3& m ){
    return m*f;
}
inline
imat3 operator*( unsigned f, const imat3& m ){
    return m*f;
}
inline
ivec3 operator*( float f, const ivec3& m ){
    return m*f;
}
inline
ivec3 operator*( double f, const ivec3& m ){
    return m*f;
}
inline
ivec3 operator*( int f, const ivec3& m ){
    return m*f;
}
inline
ivec3 operator*( unsigned f, const ivec3& m ){
    return m*f;
}
inline
ivec3 operator*(const imat3& m, const ivec3& o) {
    //mat * vector
    ivec3 R;
    for(unsigned i=0;i<3;++i){
        int32_t total = (int32_t) 0;
        for(unsigned j=0;j<3;++j){
            total += m[i][j] * o[j];
        }
        R[i] = total;
    }
    return R;
}
//vector-matrix multiplication
inline
ivec3 operator*(const ivec3& v, const imat3& o){
    ivec3 R;
    for(unsigned i=0;i<3;++i){
        int32_t total=0;
        for(unsigned j=0;j<3;++j){
            total += int32_t(v[j])*int32_t(o[j][i]);
        }
        R[i]=int32_t(total);
    }
    return R;
}
inline
imat3 transpose(const imat3& m){
    return m.transpose();
}
inline
int32_t dot(const ivec3& v, const ivec3& w){
    int32_t total =0;
    for(unsigned i=0;i<3;++i){
        total += v[i] * w[i];
    }
    return total;
}
inline
float length(const ivec3& v){
    return (float)std::sqrt( dot(v,v) );
}
inline
std::ostream& operator<<(std::ostream& o, const ivec3& v){
    o << (std::string)v;
    return o;
}
inline
std::ostream& operator<<(std::ostream& o, const imat3& v){
    o << (std::string)v;
    return o;
}
///////////////////////////////////////////////////////////////
class imat4 {
    public:
    //suitable for using in uniform buffer
    int32_t _M[4][4];
    imat4() {
        _M[0][0] = 0; _M[0][1] = 0; _M[0][2] = 0; _M[0][3] = 0;
        _M[1][0] = 0; _M[1][1] = 0; _M[1][2] = 0; _M[1][3] = 0;
        _M[2][0] = 0; _M[2][1] = 0; _M[2][2] = 0; _M[2][3] = 0;
        _M[3][0] = 0; _M[3][1] = 0; _M[3][2] = 0; _M[3][3] = 0;
    }
    imat4( int32_t m0, int32_t m1, int32_t m2, int32_t m3,
                int32_t m4, int32_t m5, int32_t m6, int32_t m7,
                int32_t m8, int32_t m9, int32_t ma, int32_t mb,
                int32_t mc, int32_t md, int32_t me, int32_t mf)
    {
        _M[0][0] = m0; _M[0][1] = m1; _M[0][2] = m2; _M[0][3] = m3;
        _M[1][0] = m4; _M[1][1] = m5; _M[1][2] = m6; _M[1][3] = m7;
        _M[2][0] = m8; _M[2][1] = m9; _M[2][2] = ma; _M[2][3] = mb;
        _M[3][0] = mc; _M[3][1] = md; _M[3][2] = me; _M[3][3] = mf;
    }
    imat4( double m0, double m1, double m2, double m3,
                double m4, double m5, double m6, double m7,
                double m8, double m9, double ma, double mb,
                double mc, double md, double me, double mf)
    {
        _M[0][0] = (int32_t) m0; _M[0][1] = (int32_t) m1; _M[0][2] = (int32_t) m2; _M[0][3] = (int32_t) m3;
        _M[1][0] = (int32_t) m4; _M[1][1] = (int32_t) m5; _M[1][2] = (int32_t) m6; _M[1][3] = (int32_t) m7;
        _M[2][0] = (int32_t) m8; _M[2][1] = (int32_t) m9; _M[2][2] = (int32_t) ma; _M[2][3] = (int32_t) mb;
        _M[3][0] = (int32_t) mc; _M[3][1] = (int32_t) md; _M[3][2] = (int32_t) me; _M[3][3] = (int32_t) mf;
    }
    imat3 upper3x3() const
    {
       return imat3(
            (*this)[0][0], (*this)[0][1], (*this)[0][2],
            (*this)[1][0], (*this)[1][1], (*this)[1][2],
            (*this)[2][0], (*this)[2][1], (*this)[2][2]
        );
    }
//included from instantiate.h
//matrix operators that work for all matN types
//Symbols defined in instantiate.h:
// MATSIZE: 2, 3, or 4
// MatType: type of the matrix that contains these
// VecType: The type of vector that goes with this matrix
    /** Initialize matrix from an initializer_list. */
    imat4(const std::initializer_list<int32_t>& L){
        if( L.size() != 4*4 )
            throw std::runtime_error("Bad number of arguments");
        //quiet bogus VS warnings
            _M[0][0] = _M[0][1] = _M[0][2] = _M[0][3] = 0;
            _M[1][0] = _M[1][1] = _M[1][2] = _M[1][3] = 0;
            _M[2][0] = _M[2][1] = _M[2][2] = _M[2][3] = 0;
            _M[3][0] = _M[3][1] = _M[3][2] = _M[3][3] = 0;
        auto x = L.begin();
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                _M[i][j] = *x++;
            }
        }
    }
    /** Matrix-matrix addition*/
    imat4 operator+(const imat4& m2) const {
        imat4 r;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                r[i][j] = (*this)[i][j] + m2[i][j];
            }
        }
        return r;
    }
    /** Matrix-matrix subtraction*/
    imat4 operator-(const imat4& m2) const {
        imat4 r;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                r[i][j] = (*this)[i][j] - m2[i][j];
            }
        }
        return r;
    }
    /** Matrix-matrix multiplication*/
    imat4 operator*(const imat4& o) const {
        //mat * mat
        imat4 R;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                int32_t total=0;
                for(unsigned k=0;k<4;++k){
                    total += (*this)[i][k] * o[k][j];
                }
                R[i][j] = total;
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    imat4 operator*(float o) const {
        //mat * scalar
        imat4 R;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                R[i][j] = (*this)[i][j] * int32_t(o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    imat4 operator*(double o) const {
        //mat * scalar
        imat4 R;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                R[i][j] = (*this)[i][j] * int32_t(o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    imat4 operator*(int o) const {
        //mat * scalar
        imat4 R;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                R[i][j] = (*this)[i][j] *int32_t( o );
            }
        }
        return R;
    }
    /** Multiply scalar by matrix */
    imat4 operator*(unsigned o) const {
        //mat * scalar
        imat4 R;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                R[i][j] = (*this)[i][j] * int32_t( o );
            }
        }
        return R;
    }
    //Vector<MATSIZE> operator*(const Vector<MATSIZE>& v) const ;
    /** Unary + */
    imat4 operator+() const {
        return *this;
    }
    /** Negation */
    imat4 operator-() const {
        return *this * -1.0f;
    }
    const void* tobytes() const {
        return _M;
    }
  private:
    class MatRow{
      public:
        imat4& m;
        unsigned i;
        MatRow(imat4& mm,int ii) : m(mm), i(ii){ }
        int32_t& operator[](unsigned j){
            //each row is padded out to be a vec4
            if( j >= 4 )
                throw std::runtime_error("Bad index");
            return this->m._M[i][j];
        }
    };
    class CMatRow{
      public:
        const imat4& m;
        unsigned i;
        CMatRow(const imat4& mm,int ii) : m(mm), i(ii){ }
        int32_t operator[](unsigned j) const {
            //each row is padded out to be a vec4
            if( j >= 4 )
                throw std::runtime_error("Bad index");
            //~ std::cout << "[" << i << "][" << j << "] = index " << i*4+j << "\n";
            return this->m._M[i][j];
        }
    };
  public:
    MatRow operator[](unsigned i) {
        return MatRow(*this,i);
    }
    CMatRow operator[](unsigned i) const {
        return CMatRow(*this,i);
    }
    bool operator==(const imat4& o) const {
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                if( (*this)[i][j] != o[i][j] )
                    return false;
            }
        }
        return true;
    }
    bool operator!=(const imat4& o) const {
        return ! (*this==o);
    }
    operator std::string() const {
        std::ostringstream oss;
        for(unsigned i=0;i<4;++i){
            oss << "[";
            for(unsigned j=0;j<4;++j){
                oss << (*this)[i][j] << "   ";
            }
            oss << "]\n";
        }
        return oss.str();
    }
    imat4 transpose() const {
        imat4 R;
        for(unsigned i=0;i<4;++i){
            for(unsigned j=0;j<4;++j){
                R[i][j] = (*this)[j][i];
            }
        }
        return R;
    }
    static imat4 identity(){
        imat4 R;
        for(unsigned i=0;i<4;++i){
            R[i][i]=1;
        }
        return R;
    }
    //no padding; not suitable for use in array or uniform buffer
    const std::array<int32_t,4*4> data() const {
        std::array<int32_t,4*4> rv;
        int k=0;
        for(int i=0;i<4;++i){
            for(int j=0;j<4;++j){
                rv[k++] = (*this)[i][j];
            }
        }
        return rv;
    }
};
class ivec4{
    ivec4(const ivec2&) = delete;
    ivec4(const ivec3&) = delete;
    public:
    int32_t x,y,z,w;
    ivec4(){
        x=y=z=w=0;
    }
    template<typename T>
    ivec4(T q) {
        x=y=z=w=(int32_t)q;
    }
    template<typename T1, typename T2, typename T3, typename T4>
    ivec4( T1 x, T2 y, T3 z, T4 w){
        this->x=(int32_t) x;
        this->y=(int32_t) y;
        this->z=(int32_t) z;
        this->w=(int32_t) w;
    }
    template<typename T>
    ivec4(const ivec3& v, T w) {
        this->x = (int32_t) v.x;
        this->y = (int32_t) v.y;
        this->z = (int32_t) v.z;
        this->w = (int32_t) w;
    }
    void setXYZ(ivec3 v){
        x=v.x;
        y=v.y;
        z=v.z;
    }
    void setXY(ivec2 v){
        x=v.x;
        y=v.y;
    }
    const ivec2 xy() const {
        return ivec2(x,y);
    }
    const ivec3 xyz() const {
        return ivec3(x,y,z);
    }
    const ivec4 xyzw() const {
        return *this;
    }
    static ivec4 from(const float* fp){
        ivec4 v;
        for(unsigned i=0;i<4;++i)
            v[i] = int32_t(fp[i]);
        return v;
    }
    ivec4(const std::initializer_list<int32_t>& L){
        if( L.size() != 4 )
            throw std::runtime_error("Bad size for vector initializer");
        auto F = L.begin();
            this->w=*F++;
    }
    int32_t& operator[](unsigned key){
        switch(key){
            case 0: return x;
            case 1: return y;
                case 2: return z;
                case 3: return w;
            default: throw std::runtime_error("Bad index");
        }
    }
    int32_t operator[](unsigned key) const{
        switch(key){
            case 0: return x;
            case 1: return y;
                case 2: return z;
                case 3: return w;
            default: throw std::runtime_error("Bad index");
        }
    }
    void operator+=(const ivec4& o){
        for(unsigned i=0;i<4;++i)
            (*this)[i] += o[i];
    }
    void operator-=(const ivec4& o){
        for(unsigned i=0;i<4;++i)
            (*this)[i] -= o[i];
    }
    void operator*=(const ivec4& o){
        for(unsigned i=0;i<4;++i)
            (*this)[i] *= o[i];
    }
    void operator*=(const int32_t o){
        for(unsigned i=0;i<4;++i)
            (*this)[i] *= o;
    }
    void operator/=(const ivec4& o){
        for(unsigned i=0;i<4;++i)
            (*this)[i] /= o[i];
    }
    ivec4 operator-()const {
        ivec4 res;
        for(unsigned i=0;i<4;++i){
            double tmp = (*this)[i];
            tmp = -tmp;
            res[i] = (int32_t) tmp;
        }
        return res;
    }
    ivec4 operator+() const {
        ivec4 res = *this;
        return res;
    }
    ivec4 operator+(const ivec4& other) const {
        ivec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] + other[i];
        return res;
    }
    ivec4 operator-(const ivec4& other) const {
        ivec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] - other[i];
        return res;
    }
    ivec4 operator*(const ivec4& other) const {
        ivec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] * other[i];
        return res;
    }
    ivec4 operator/(const ivec4& other) const {
        ivec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] / other[i];
        return res;
    }
    ivec4 operator*(float other) const {
        ivec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] * int32_t(other );
        return res;
    }
    ivec4 operator*(double other) const {
        ivec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] * int32_t(other );
        return res;
    }
    ivec4 operator*(int other) const {
        ivec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] * int32_t(other );
        return res;
    }
    ivec4 operator*(unsigned other) const {
        ivec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] * int32_t( other );
        return res;
    }
    ivec4 operator/(float other) const {
        ivec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] / int32_t(other);
        return res;
    }
    ivec4 operator/(double other) const {
        ivec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] / int32_t(other);
        return res;
    }
    ivec4 operator/(int other) const {
        ivec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] / int32_t(other);
        return res;
    }
    ivec4 operator/(unsigned other) const {
        ivec4 res;
        for(unsigned i=0;i<4;++i)
            res[i] = (*this)[i] / int32_t(other);
        return res;
    }
    bool operator<(const ivec4& o) const {
        for(unsigned i=0;i<4;++i){
            if( (*this)[i] != o[i] )
                return (*this)[i] < o[i];
        }
        return false;
    }
    bool operator==(const ivec4& o) const {
        for(unsigned i=0;i<4;++i){
            if((*this)[i] != o[i])
                return false;
        }
        return true;
    }
    bool operator!=(const ivec4& o)const {
        return ! (*this == o);
    }
    operator std::string() const {
        std::ostringstream oss;
        oss << "vec" << 4 << "(";
        for(unsigned i=0;i<4;++i){
            oss << " " << (*this)[i];
        }
        oss << ")";
        return oss.str();
    }
    bool isZero() const {
        for(unsigned i=0;i<4;++i){
            if( (*this)[i] != 0 )
                return false;
        }
        return true;
    }
    unsigned size() const {
        return 4;
    }
};
inline
imat4 operator*( float f, const imat4& m ){
    return m*f;
}
inline
imat4 operator*( double f, const imat4& m ){
    return m*f;
}
inline
imat4 operator*( int f, const imat4& m ){
    return m*f;
}
inline
imat4 operator*( unsigned f, const imat4& m ){
    return m*f;
}
inline
ivec4 operator*( float f, const ivec4& m ){
    return m*f;
}
inline
ivec4 operator*( double f, const ivec4& m ){
    return m*f;
}
inline
ivec4 operator*( int f, const ivec4& m ){
    return m*f;
}
inline
ivec4 operator*( unsigned f, const ivec4& m ){
    return m*f;
}
inline
ivec4 operator*(const imat4& m, const ivec4& o) {
    //mat * vector
    ivec4 R;
    for(unsigned i=0;i<4;++i){
        int32_t total = (int32_t) 0;
        for(unsigned j=0;j<4;++j){
            total += m[i][j] * o[j];
        }
        R[i] = total;
    }
    return R;
}
//vector-matrix multiplication
inline
ivec4 operator*(const ivec4& v, const imat4& o){
    ivec4 R;
    for(unsigned i=0;i<4;++i){
        int32_t total=0;
        for(unsigned j=0;j<4;++j){
            total += int32_t(v[j])*int32_t(o[j][i]);
        }
        R[i]=int32_t(total);
    }
    return R;
}
inline
imat4 transpose(const imat4& m){
    return m.transpose();
}
inline
int32_t dot(const ivec4& v, const ivec4& w){
    int32_t total =0;
    for(unsigned i=0;i<4;++i){
        total += v[i] * w[i];
    }
    return total;
}
inline
float length(const ivec4& v){
    return (float)std::sqrt( dot(v,v) );
}
inline
std::ostream& operator<<(std::ostream& o, const ivec4& v){
    o << (std::string)v;
    return o;
}
inline
std::ostream& operator<<(std::ostream& o, const imat4& v){
    o << (std::string)v;
    return o;
}
///////////////////////////////////////////////////////////////
//only defined for vec3/ivec3/uvec3
inline
ivec3 cross(const ivec3& v, const ivec3& w){
    return ivec3(
        v.y*w.z - w.y*v.z,
        w.x*v.z - v.x*w.z,
        v.x*w.y - w.x*v.y
    );
}
//~ //vec4::xy()
//~ inline const VEC2TYPE VEC4TYPE::xy() const 
//~ { 
    //~ return VEC2TYPE(x,y); 
//~ }
//~ //vec4::xyz()
//~ inline const VEC3TYPE VEC4TYPE::xyz() const 
//~ { 
    //~ return VEC3TYPE(x,y,z); 
//~ }
//~ //vec3.xy()
//~ inline const VEC2TYPE VEC3TYPE::xy() const 
//~ { 
    //~ return VEC2TYPE(x,y); 
//~ }
//~ inline void VEC4TYPE::setXYZ(VEC3TYPE v){
    //~ x=v.x;
    //~ y=v.y;
    //~ z=v.z;
//~ }
//~ inline void VEC4TYPE::setXY(VEC2TYPE v){
    //~ x=v.x;
    //~ y=v.y;
//~ }
//~ inline void VEC3TYPE::setXY(VEC2TYPE v){
    //~ x=v.x;
    //~ y=v.y;
//~ }


//~ typedef vec2_float vec2;
//~ typedef vec3_float vec3;
//~ typedef vec4_float vec4;
//~ typedef vec2_int32_t ivec2;
//~ typedef vec3_int32_t ivec3;
//~ typedef vec4_int32_t ivec4;
//~ typedef vec2_uint32_t uvec2;
//~ typedef vec3_uint32_t uvec3;
//~ typedef vec4_uint32_t uvec4;

//~ typedef mat2_float mat2;
//~ typedef mat3_float mat3;
//~ typedef mat4_float mat4;
//~ typedef mat2_int32_t imat2;
//~ typedef mat3_int32_t imat3;
//~ typedef mat4_int32_t imat4;
//~ typedef mat2_uint32_t umat2;
//~ typedef mat3_uint32_t umat3;
//~ typedef mat4_uint32_t umat4;
 

//normalize is only defined for float vectors
inline
vec2 normalize(const vec2& a){
    return 1.0f/length(a) * a;
}

inline
vec3 normalize(const vec3& a){
    return 1.0f/length(a) * a;
}

inline
vec4 normalize(const vec4& a){
    return 1.0f/length(a) * a;
}
 

//by jh
inline
mat4 translation(const vec3& t){
    return mat4( 1.0f, 0.0f, 0.0f, 0.0f, 
                 0.0f, 1.0f, 0.0f, 0.0f, 
                 0.0f, 0.0f, 1.0f, 0.0f, 
                 t[0], t[1], t[2], 1.0f);
}

//by jh
inline
mat4 translation(float tx, float ty, float tz){
    return mat4( 1.0f, 0.0f, 0.0f, 0.0f, 
                 0.0f, 1.0f, 0.0f, 0.0f, 
                 0.0f, 0.0f, 1.0f, 0.0f, 
                 tx,   ty,   tz,   1.0f);
}


//by jh
inline
mat4 translation(double tx, double ty, double tz){
    return translation((float)tx, (float)ty, (float)tz);
}

//by jh
inline
mat3 translation2(const vec2& t){
    return mat3( 1.0f, 0.0f, 0.0f, 
                 0.0f, 1.0f, 0.0f, 
                 t[0],t[1],1.0f);
}
//by jh
inline
mat3 translation2(float tx, float ty){
    return mat3( 1.0f, 0.0f, 0.0f, 
                 0.0f, 1.0f, 0.0f, 
                 tx, ty, 1.0f);
}


//by jh
inline
mat4 scaling( float sx, float sy, float sz ){
     return mat4(
        sx,   0.0f, 0.0f, 0.0f, 
        0.0f, sy,   0.0f, 0.0f, 
        0.0f, 0.0f, sz,   0.0f, 
        0.0f, 0.0f, 0.0f, 1.0f);
}

//by jh
inline
mat4 scaling( vec3 s){
   return scaling(s.x,s.y,s.z);
}

inline
mat4 scaling( double sx, double sy, double sz ){
    return scaling( (float)sx, (float)sy, (float)sz);
}


//by jh
inline
mat3 scaling3( float sx, float sy, float sz ){
    return scaling(sx,sy,sz).upper3x3();
}


//by jh
inline
mat3 scaling2( float sx, float sy){
    return mat3(
        sx, 0.0f, 0.0f,
        0.0f, sy, 0.0f,
        0.0f, 0.0f ,1.0f);
}

//by jh
inline
mat3 scaling2( vec2 s){
    return scaling2(s.x,s.y);
}

/*
//by jh, from 2801 transforms3d notes (not from tdl)
//almost the same as TDL, but the signs are flipped...Need to check this
mat4 axisRotation(const vec3& S, float angle){
    float s=std::sin(angle);
    float c=std::cos(angle);
    float ssx = s*S.x;
    float ssy = s*S.y;
    float ssz = s*S.z;
    float sxsy = S.x*S.y;
    float sxsz = S.x*S.z;
    float sysz = S.y*S.z;
    return mat4(
        S.x*S.x*(1-c)+c ,      sxsy*(1-c)-ssz ,     sxsz*(1-c)+ssy,      0,
        sxsy*(1-c)+ssz,        S.y*S.y*(1-c)+c ,    sysz*(1-c)-s*S.x,    0,
        sxsz*(1-c)-ssy,        sysz*(1-c)+ssx,      S.z*S.z*(1-c)+c,     0,
        0,                     0,                   0,                   1);
}
*/

    
inline
mat4 axisRotation(const vec3& axis, float angle){
    //from TDL
    //axis=normalize(axis)
    float x = axis[0],
    y = axis[1],
    z = axis[2],
    xx = x * x,
    yy = y * y,
    zz = z * z,
    c = (float)cos(angle),
    s = (float)sin(angle),
    oneMinusCosine = 1 - c,
    zs = z*s,
    xs = x*s,
    ys = y*s,
    xy = x*y,
    xz = x*z,
    yz = y*z;
    return mat4(
        xx + (1 - xx) * c,              
        xy * oneMinusCosine + zs,      
        xz * oneMinusCosine - ys,
        0,
        xy * oneMinusCosine - zs,
        yy + (1 - yy) * c,
        yz * oneMinusCosine + xs,
        0,
        xz * oneMinusCosine + ys,
        yz * oneMinusCosine - xs,
        zz + (1 - zz) * c,
        0,
        0, 0, 0, 1
    );
}

   
inline
mat3 axisRotation3(const vec3& axis, float angle){
    //from TDL
    //axis=normalize(axis)
    return axisRotation(axis,angle).upper3x3();
}
 

inline
mat4 axisRotation(const vec4& axis, float angle){
    return axisRotation(axis.xyz(),angle);
}
  

inline
mat2 inverse(const mat2& m){
    //Return matrix inverse. From TDL.
    auto d = 1.0f / (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
    return mat2(d * m[1][1], -d * m[0][1], -d * m[1][0], d * m[0][0]);
}
    
inline
mat3 inverse(const mat3& m){
    //Return matrix inverse. From TDL.
    auto t00 = m[1][1] * m[2][2] - m[1][2] * m[2][1];
    auto t10 = m[0][1] * m[2][2] - m[0][2] * m[2][1];
    auto t20 = m[0][1] * m[1][2] - m[0][2] * m[1][1];
    auto d = 1.0f / (m[0][0] * t00 - m[1][0] * t10 + m[2][0] * t20);
    return mat3( d * t00, -d * t10, d * t20,
          -d * (m[1][0] * m[2][2] - m[1][2] * m[2][0]),
           d * (m[0][0] * m[2][2] - m[0][2] * m[2][0]),
          -d * (m[0][0] * m[1][2] - m[0][2] * m[1][0]),
           d * (m[1][0] * m[2][1] - m[1][1] * m[2][0]),
          -d * (m[0][0] * m[2][1] - m[0][1] * m[2][0]),
           d * (m[0][0] * m[1][1] - m[0][1] * m[1][0]) );
}
inline
mat4 inverse(const mat4& m){
    //Return matrix inverse. From TDL.

    auto tmp_0 = m[2][2] * m[3][3];
    auto tmp_1 = m[3][2] * m[2][3];
    auto tmp_2 = m[1][2] * m[3][3];
    auto tmp_3 = m[3][2] * m[1][3];
    auto tmp_4 = m[1][2] * m[2][3];
    auto tmp_5 = m[2][2] * m[1][3];
    auto tmp_6 = m[0][2] * m[3][3];
    auto tmp_7 = m[3][2] * m[0][3];
    auto tmp_8 = m[0][2] * m[2][3];
    auto tmp_9 = m[2][2] * m[0][3];
    auto tmp_10 = m[0][2] * m[1][3];
    auto tmp_11 = m[1][2] * m[0][3];
    auto tmp_12 = m[2][0] * m[3][1];
    auto tmp_13 = m[3][0] * m[2][1];
    auto tmp_14 = m[1][0] * m[3][1];
    auto tmp_15 = m[3][0] * m[1][1];
    auto tmp_16 = m[1][0] * m[2][1];
    auto tmp_17 = m[2][0] * m[1][1];
    auto tmp_18 = m[0][0] * m[3][1];
    auto tmp_19 = m[3][0] * m[0][1];
    auto tmp_20 = m[0][0] * m[2][1];
    auto tmp_21 = m[2][0] * m[0][1];
    auto tmp_22 = m[0][0] * m[1][1];
    auto tmp_23 = m[1][0] * m[0][1];

    auto t0 = (tmp_0 * m[1][1] + tmp_3 * m[2][1] + tmp_4 * m[3][1]) -        (tmp_1 * m[1][1] + tmp_2 * m[2][1] + tmp_5 * m[3][1]);
    auto t1 = (tmp_1 * m[0][1] + tmp_6 * m[2][1] + tmp_9 * m[3][1]) -        (tmp_0 * m[0][1] + tmp_7 * m[2][1] + tmp_8 * m[3][1]);
    auto t2 = (tmp_2 * m[0][1] + tmp_7 * m[1][1] + tmp_10 * m[3][1]) -        (tmp_3 * m[0][1] + tmp_6 * m[1][1] + tmp_11 * m[3][1]);
    auto t3 = (tmp_5 * m[0][1] + tmp_8 * m[1][1] + tmp_11 * m[2][1]) -        (tmp_4 * m[0][1] + tmp_9 * m[1][1] + tmp_10 * m[2][1]);
    auto d = 1.0f / (m[0][0] * t0 + m[1][0] * t1 + m[2][0] * t2 + m[3][0] * t3);

    return mat4(d * t0, d * t1, d * t2, d * t3,
       d * ((tmp_1 * m[1][0] + tmp_2 * m[2][0] + tmp_5 * m[3][0]) -
          (tmp_0 * m[1][0] + tmp_3 * m[2][0] + tmp_4 * m[3][0])),
       d * ((tmp_0 * m[0][0] + tmp_7 * m[2][0] + tmp_8 * m[3][0]) -
          (tmp_1 * m[0][0] + tmp_6 * m[2][0] + tmp_9 * m[3][0])),
       d * ((tmp_3 * m[0][0] + tmp_6 * m[1][0] + tmp_11 * m[3][0]) -
          (tmp_2 * m[0][0] + tmp_7 * m[1][0] + tmp_10 * m[3][0])),
       d * ((tmp_4 * m[0][0] + tmp_9 * m[1][0] + tmp_10 * m[2][0]) -
          (tmp_5 * m[0][0] + tmp_8 * m[1][0] + tmp_11 * m[2][0])),
       d * ((tmp_12 * m[1][3] + tmp_15 * m[2][3] + tmp_16 * m[3][3]) -
          (tmp_13 * m[1][3] + tmp_14 * m[2][3] + tmp_17 * m[3][3])),
       d * ((tmp_13 * m[0][3] + tmp_18 * m[2][3] + tmp_21 * m[3][3]) -
          (tmp_12 * m[0][3] + tmp_19 * m[2][3] + tmp_20 * m[3][3])),
       d * ((tmp_14 * m[0][3] + tmp_19 * m[1][3] + tmp_22 * m[3][3]) -
          (tmp_15 * m[0][3] + tmp_18 * m[1][3] + tmp_23 * m[3][3])),
       d * ((tmp_17 * m[0][3] + tmp_20 * m[1][3] + tmp_23 * m[2][3]) -
          (tmp_16 * m[0][3] + tmp_21 * m[1][3] + tmp_22 * m[2][3])),
       d * ((tmp_14 * m[2][2] + tmp_17 * m[3][2] + tmp_13 * m[1][2]) -
          (tmp_16 * m[3][2] + tmp_12 * m[1][2] + tmp_15 * m[2][2])),
       d * ((tmp_20 * m[3][2] + tmp_12 * m[0][2] + tmp_19 * m[2][2]) -
          (tmp_18 * m[2][2] + tmp_21 * m[3][2] + tmp_13 * m[0][2])),
       d * ((tmp_18 * m[1][2] + tmp_23 * m[3][2] + tmp_15 * m[0][2]) -
          (tmp_22 * m[3][2] + tmp_14 * m[0][2] + tmp_19 * m[1][2])),
       d * ((tmp_22 * m[2][2] + tmp_16 * m[0][2] + tmp_21 * m[1][2]) -
          (tmp_20 * m[1][2] + tmp_23 * m[2][2] + tmp_17 * m[0][2])));
}


// ~ def det(M) const {
    // ~ """Return matrix determinant. From TDL.
    // ~ @param M: matrix
    // ~ @rtype: number
    // ~ """
    // ~ if type(M) == mat2:
        // ~ return m[0][0]*m[1][1] - m[0][1]*m[1][0]
    // ~ elif type(M) == mat3:
        // ~ return m[2][2] * (m[0][0] * m[1][1] - m[0][1] * m[1][0]) -              m[2][1] * (m[0][0] * m[1][2] - m[0][2] * m[1][0]) +                m[2][0] * (m[0][1] * m[1][2] - m[0][2] * m[1][1])
    // ~ elif type(M) == mat4:
        // ~ t01 = m[0][0] * m[1][1] - m[0][1] * m[1][0]
        // ~ t02 = m[0][0] * m[1][2] - m[0][2] * m[1][0]
        // ~ t03 = m[0][0] * m[1][3] - m[0][3] * m[1][0]
        // ~ t12 = m[0][1] * m[1][2] - m[0][2] * m[1][1]
        // ~ t13 = m[0][1] * m[1][3] - m[0][3] * m[1][1]
        // ~ t23 = m[0][2] * m[1][3] - m[0][3] * m[1][2]
        // ~ return (m[3][3] * (m[2][2] * t01 - m[2][1] * t02 + m[2][0] * t12) -
             // ~ m[3][2] * (m[2][3] * t01 - m[2][1] * t03 + m[2][0] * t13) +
             // ~ m[3][1] * (m[2][3] * t02 - m[2][2] * t03 + m[2][0] * t23) -
             // ~ m[3][0] * (m[2][3] * t12 - m[2][2] * t13 + m[2][1] * t23) )
    // ~ else:
        // ~ assert 0


#include <random>

 
inline
float radians(double deg){
    return float(deg)/180.0f * 3.14159265358979323f;
}
 
template<typename T>
T mix( const T& a, const T& b, double d)
{
    return a + float(d) * (b-a);
}
 
 
namespace {
    const double pi = 4.0 * std::atan(1.0);
    const double PI = 4.0 * std::atan(1.0);
}

inline
float uniform( float min, float max){
    static std::minstd_rand R(12345);   //12345=seed
    std::uniform_real_distribution<float> D(min,max);
    float rv = D(R);
    return rv;
}

inline
float uniform() {
    return uniform(0.0f, 1.0f) ;
}

inline
int randrange( int min, int max){
    static std::minstd_rand R(54321);  //seed
    static std::uniform_real_distribution<float> D(0.0f, 1.0f);
    float t = D(R);
    int rv = (int)( float(min) + t * float(max-min));
    if( rv >= max )
        rv = max-1;
    return rv;
}
    
    
#ifdef _MSC_VER
#pragma warning(pop)
#endif
