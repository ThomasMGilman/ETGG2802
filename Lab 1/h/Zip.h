#pragma once
//written by jh sp 2018, fa 2018, fa 2019
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <memory>

class Zip{
    Zip(const Zip&) = delete;
    void operator=(const Zip&) = delete;
  public:
    class ZipEntry {
        Zip& owner;
        std::uint32_t startingOffset;
        friend class Zip;
      public:
        std::string name;
        ZipEntry(Zip& own, std::string n, std::uint32_t soff);
        std::vector<std::uint8_t> read();
        std::string getName(){ return name; }
    };
  
    Zip( std::string f );
    Zip( const std::vector<uint8_t>& f);
    std::vector<ZipEntry> getEntries();
    std::vector<std::string> namelist();
    ZipEntry open(std::string name);
    std::vector<std::uint8_t> read(const ZipEntry& ze);
private:
    void init();
    std::shared_ptr< std::basic_istream<char> > inp;
    std::vector<ZipEntry> entries;
    std::string fname;
};
        

