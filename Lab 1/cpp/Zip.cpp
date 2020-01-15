//written by jh sp 2018, fa 2018, fa 2019

#include "Zip.h"
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
#include <cassert>
#include <cstring>
#include "miniz_tinfl.h"
#include <sstream>

using namespace std;

namespace {
    #pragma pack(push,1)
    struct ZipLocalFileHeader{
        std::uint32_t sig;  //0x4034b50
        std::uint16_t version;
        std::uint16_t flags;
        std::uint16_t method;
        std::uint16_t time;
        std::uint16_t date;
        std::uint32_t crc;
        std::uint32_t compressedSize;
        std::uint32_t uncompressedSize;
        std::uint16_t filenameLength;
        std::uint16_t extraLength;
        //filename, extra is here
    };
    struct ZipCentralDirectoryHeader{
        std::uint32_t sig;
        std::uint16_t cVersion, eVersion, flags, method, time, date;
        std::uint32_t crc, compressedSize, uncompressedSize;
        uint16_t filenameLength, extraLength, commentLength, diskNumber, iAttributes;
        uint32_t eAttributes, localHeaderOffset;
        //filename, extra, and comment follow
    };
    struct ZipDataDescriptor{
        uint32_t crc, compressedSize, uncompressedSize;
    };
    #pragma pack(pop)
    
    std::vector<std::uint8_t> extract_deflate(ZipLocalFileHeader& hdr, basic_istream<char>& in)
    {
        using namespace std;

        vector<uint8_t> ibuff(hdr.compressedSize);
        vector<uint8_t> obuff(hdr.uncompressedSize);
        
        in.read( (char*) ibuff.data(), ibuff.size() );
        size_t sz = tinfl_decompress_mem_to_mem( 
            obuff.data(), obuff.size(), 
            ibuff.data(), ibuff.size(),
            0 );
        if( sz != obuff.size() ){
            throw std::runtime_error("Unable to decompress");
        }
        return obuff;
    }

}

Zip::Zip( std::string f ){
    using namespace std;
    this->fname=f;
    using namespace std;
    
    auto p = new ifstream(fname,ios::binary);
    if(!p->good() )
        throw runtime_error("Cannot open zip file "+fname);

    inp = shared_ptr<basic_istream<char>>(p);

    init();
}

//FIXME: inefficient
Zip::Zip( const std::vector<uint8_t>& f)
{
    std::string x( (char*) f.data(), f.size() );
    auto p = new istringstream(x,ios::binary);
    inp = shared_ptr<basic_istream<char>>(p);
    init();
}

void Zip::init()
{
    ZipLocalFileHeader hdr;
    
    size_t recordStart;
    auto& in = *inp;
    
    int recordCount=0;
    recordStart = unsigned(in.tellg());
    in.read( (char*)&hdr, sizeof(hdr));
    while( hdr.sig == 0x4034b50 ){
        ++recordCount;
        in.seekg(recordStart + 
            sizeof(hdr) + 
            hdr.filenameLength + 
            hdr.compressedSize + 
            hdr.extraLength +
            //if bit 3 of flags = 1: There's a data descriptor here too
            ( (hdr.flags & (1<<3)) ? sizeof(ZipDataDescriptor):0 ) );
        recordStart = unsigned(in.tellg());
        in.read( (char*)&hdr, sizeof(hdr));
    }
    
    if( recordCount == 0 )
        throw runtime_error("This does not appear to be a zip file");
        
    ZipCentralDirectoryHeader chdr;
    for(int i=0;i<recordCount;++i){
        in.seekg(recordStart);
        in.read( (char*) &chdr, sizeof(chdr));
        assert( chdr.sig == 0x2014b50 );
        vector<char> filenameV(chdr.filenameLength);
        in.read(filenameV.data(),chdr.filenameLength);
        string filename(filenameV.data(),chdr.filenameLength);
        ZipEntry meta(*this,filename, chdr.localHeaderOffset);
        entries.push_back(meta);
        recordStart += sizeof(chdr) + chdr.filenameLength + chdr.extraLength + chdr.commentLength;
    }
}

std::vector<Zip::ZipEntry> Zip::getEntries(){
    return entries;
}

std::vector<std::string> Zip::namelist(){
    using namespace std;
    vector<string> n;
    for( auto e : getEntries() ){
        n.push_back(e.name);
    }
    return n;
}

Zip::ZipEntry Zip::open(std::string name){
    using namespace std;

    for(ZipEntry& ze : entries ){
        if( ze.name == name ){
            return ze;
        }
    }
    throw runtime_error("No such entry "+name+" in zip file");
}

std::vector<std::uint8_t> Zip::read(const ZipEntry& ze){
    using namespace std;
    
    istream& in = *inp;

    uint32_t recordStart = ze.startingOffset;
    in.seekg(recordStart);
    ZipLocalFileHeader hdr;
    in.read( (char*)&hdr, sizeof(hdr));
    if( hdr.flags & 1 )
        throw runtime_error("Cannot extract encrypted file");

    in.seekg(recordStart + 
            sizeof(hdr) + 
            hdr.filenameLength + 
            hdr.extraLength );
    
    if( hdr.method == 0 ){
        //stored
        std::vector<std::uint8_t> data;
        data.resize(hdr.uncompressedSize);
        in.read( (char*)data.data(), hdr.uncompressedSize);
        return data;
    }
    else if( hdr.method == 8 ){
        //deflate
        return extract_deflate(hdr,in);
    }
    else{
        throw runtime_error("Cannot extract file: Unknown compression scheme");
    }
}

Zip::ZipEntry::ZipEntry(Zip& own, std::string n, std::uint32_t soff): owner(own){
    name=n;
    startingOffset=soff;
}

std::vector<uint8_t> Zip::ZipEntry::read()
{
    return owner.read(*this);
}

