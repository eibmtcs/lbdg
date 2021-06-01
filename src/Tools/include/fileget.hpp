#ifndef __FILEGET_HPP__
#define __FILEGET_HPP__

#include <fstream>

class Fileget {
public:
    Fileget(int bs);
    std::string open(std::string s);
    std::string openLDConfig();
    std::string openModel(std::string s);
    std::string openParts(std::string s);
    std::ifstream filestream;
    std::string path;
    char *buffer;
    double buffersize;
};

#endif
