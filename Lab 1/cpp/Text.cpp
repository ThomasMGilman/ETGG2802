
#include "Text.h"
#include "DataTexture2DArray.h"
#include "Program.h"
#include <string>
#include <utility>

using namespace std;

namespace {
    
    class Font{
      public:
        DataTexture2DArray* fontsheet;
        int maxwidth;
        int maxheight;
        vector<unsigned> widths;
        Font(){}
        Font(const void* data, int maxwidth, int maxheight, const unsigned* widths, unsigned numchars ){
            fontsheet = new DataTexture2DArray( 
                maxwidth, maxheight, numchars, 
                GL_RGBA, GL_RED, GL_UNSIGNED_BYTE, data );
            fontsheet->generateMipmap();
            this->maxwidth = maxwidth;
            this->maxheight = maxheight;
            this->widths.insert( this->widths.end(), widths, widths+numchars);
        }
    };
    Program* prog = nullptr;
    map<int, Font> fonts;
    GLuint vao;
    
    namespace font24 {
        #include "consolefont24.h"
    }
    namespace font18 {
        #include "consolefont18.h"
    }
    namespace font14 {
        #include "consolefont14.h"
    }
    namespace font12 {
        #include "consolefont12.h"
    }


    map<int, vector<unsigned> > widths;
    
    const char* textvs = 
        "#version 430\n"
        "layout(location=0) in float dummy;\n"
        "layout(binding=1) uniform sampler2DArray datatex;\n"
        "void main(){\n"
        "    //ascii, width, xoffset, yoffset. width & offsets are in pixels\n"
        "    vec4 p = texelFetch(datatex, ivec3(gl_InstanceID,0,0),0 );\n"
        "    gl_Position = p;\n"
        "}\n";


    const char* textgs = 
        "#version 430\n"
        "layout(points) in;\n"
        "layout(triangle_strip,max_vertices=4) out;\n"
        ""
        "uniform float depth;\n"
        "uniform vec2 textOrigin;\n"          
        "uniform vec4 screenSize;\n"
        "uniform float charCellHeight;\n"
        ""
        "out vec2 g_texCoord;\n"
        "flat out float g_charWidth;\n"
        "flat out float g_ascii;\n"
        ""
        "vec2 map(float x, float y){\n"
        "    //map pixel coordinates to view space coordinates\n"
        "    vec2 v = vec2(x,y);\n"
        "    return vec2(-1.0) + 2.0 * v * screenSize.zw ;\n"
        "}\n"
        "void main(){\n"
        "    g_ascii = gl_in[0].gl_Position.x;\n"
        "    g_charWidth = gl_in[0].gl_Position.y;\n"
        "    float xOffset = gl_in[0].gl_Position.z;\n"
        "    float yOffset = gl_in[0].gl_Position.w;\n"
        "    float x = textOrigin.x;\n"
        "    float y = textOrigin.y;\n"
        "    x += xOffset;\n"
        "    y += yOffset;\n"
        ""
        "    gl_Position = vec4(map(x,y+charCellHeight),depth,1);\n"
        "    g_texCoord = vec2(0,0);\n"
        "    EmitVertex();\n"
        ""
        "    gl_Position = vec4(map(x,y),depth,1);\n"
        "    g_texCoord = vec2(0,1);\n"
        "    EmitVertex();\n"
        ""
        "    gl_Position = vec4(map(x+g_charWidth,y+charCellHeight),depth,1);\n"
        "    g_texCoord = vec2(1,0);\n"
        "    EmitVertex();\n"
        ""
        "    gl_Position = vec4(map(x+g_charWidth,y),depth,1);\n"
        "    g_texCoord = vec2(1,1);\n"
        "    EmitVertex();\n"
        "}\n";
        
    const char* textfs = 
        "#version 430\n"
        "in vec2 g_texCoord;\n"
        "flat in float g_ascii;\n"
        "flat in float g_charWidth;\n"
        "uniform float charCellHeight;\n"
        "layout(binding=0) uniform sampler2DArray fontsheet;\n"
        "uniform vec3 textColor;\n"
        "out vec4 color;\n"
        ""
        "void main(){\n" 
        "    ivec3 texc = ivec3( g_texCoord.x * g_charWidth, g_texCoord.y*charCellHeight, g_ascii );\n"
        "    vec4 fsc = texelFetch( fontsheet, texc, 0 );\n" 
        "    color = vec4( textColor, fsc.r );\n"
        "}";

}

    
Text::Text( int size, vec2 p, std::string s)
{
    
    if( fonts.empty() ){
        fonts[12] = Font( font12::chardata, font12::MAXWIDTH, font12::MAXHEIGHT, font12::widths, font12::LAST );
        fonts[14] = Font( font14::chardata, font14::MAXWIDTH, font14::MAXHEIGHT, font14::widths, font14::LAST );
        fonts[18] = Font( font18::chardata, font18::MAXWIDTH, font18::MAXHEIGHT, font18::widths, font18::LAST );
        fonts[24] = Font( font24::chardata, font24::MAXWIDTH, font24::MAXHEIGHT, font24::widths, font24::LAST );
    }
     
    if( !prog ){
        array< pair< string,string>,6 > A;
        A[0] = {"textvs",textvs};
        A[3] = {"textgs",textgs};
        A[4] = {"textfs",textfs};
        prog = new Program( A );
        
        GLuint tmp[1];
        glGenVertexArrays(1,tmp);
        vao = tmp[0];
        glBindVertexArray(vao);
        Buffer P(std::vector<float>{ 0.0f });
        P.bind(GL_ARRAY_BUFFER);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer( 0, 1, GL_FLOAT, false, 1*4, 0 );
        glBindVertexArray(0);
    }
    
    if( fonts.find(size) == fonts.end() )
        throw std::runtime_error("Unsupported font size: "+std::to_string(size));
        
    this->size=size;
    this->pos = p;
    this->color = vec3(1,1,1);
    datatex = std::make_shared<DataTexture2DArray>(1,1,1, GL_RGBA32F, GL_RGBA, GL_FLOAT, nullptr );
    datatex->generateMipmap();
    setText(s);
}


void Text::setColor(vec3 c){
    this->color = c;
}

void Text::setPosition(vec2 p ){
    this->pos = p;
}


void Text::setText(std::string s){
    this->data = s;
    this->dirty = true;
}
    
void Text::draw(int screenWidth, int screenHeight, float depth){
    
    if( this->dirty ) {
        //ascii, width, xoffset, yoffset. width & offsets are in pixels
        std::vector<vec4> v;
        v.reserve(this->data.length());
        float xo = 0;
        float yo = 0;
        for(unsigned i=0;i<this->data.length();++i){
            if( this->data[i] == '\n' ){
                xo=0;
                yo += float(fonts[size].maxheight);
            } else if( this->data[i] >= 0 && this->data[i] < 127 ){
                unsigned ascii = (unsigned) this->data[i];
                float w = float(fonts[size].widths[ascii]);
                v.push_back( vec4( float(ascii), w, xo, yo ) );
                xo += w;
            }
        }
        datatex->setData( unsigned(this->data.length()) ,1,1, GL_RGBA32F, GL_RGBA, GL_FLOAT, v.data() );
        datatex->generateMipmap();
        this->dirty = false;
    }
    
    unsigned numChars = (unsigned) (this->data.length());
    if( numChars == 0 )
        return;
        
    //~ bool depthEnabled = glIsEnabled(GL_DEPTH_TEST);
    //~ glDisable(GL_DEPTH_TEST);
    auto curr = Program::current;
    prog->use();
    
    glBindVertexArray(vao);
    Program::setUniform("depth",depth);
    Program::setUniform("screenSize", vec4(
        float(screenWidth), float(screenHeight) ,1.0f/float(screenWidth), 1.0f/float(screenHeight)) );
    Program::setUniform("charCellHeight", fonts[size].maxheight );
    fonts[size].fontsheet->bind(0);
    datatex->bind(1);
    Program::setUniform("textOrigin",pos+vec2(1,-1));
    Program::setUniform("textColor", vec3(0,0,0));
    glDrawArraysInstanced( GL_POINTS, 0,1, numChars );
    Program::setUniform("textOrigin",pos);
    Program::setUniform("textColor", color);
    glDrawArraysInstanced( GL_POINTS, 0,1, numChars );
    //~ if( depthEnabled )
        //~ glEnable(GL_DEPTH_TEST);
    if( curr )
        curr->use();
}
