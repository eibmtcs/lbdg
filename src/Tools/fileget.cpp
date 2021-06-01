#include <fstream>

#include "fileget.hpp"
#include "subFile.hpp"


Fileget::Fileget(int bs) {
    buffersize = bs;
    path = "/opt/lbdg/ldraw/";
    buffer = new char[bs];
}

std::string Fileget::openLDConfig() {
    std::string search = path + "LDConfig.ldr";
    filestream.open(search);

    if(filestream.is_open()) {
        std::cout << "Fileget::openLDConfig: Load LDconfig file: " << search << std::endl;
        filestream.getline(buffer, buffersize, '\0');
    } else {
        std::cout << "Fileget::openLDConfig: Can't find LDconfig file: " << search << std::endl;
    }
    filestream.close();

    return buffer;
}

std::string Fileget::open(std::string s) {
    std::string search = s;
    filestream.open(search);

    if(filestream.is_open()) {
        std::cout << "Fileget::open: Load file: " << search << std::endl;
        filestream.getline(buffer, buffersize, '\0');
    } else {
        std::cout << "Fileget::open: Can't find file: " << s << std::endl;
    }

    filestream.close();

    return buffer;
}

std::string Fileget::openModel(std::string s) {
    std::string search = path + "models/" + s;
    filestream.open(search);

    if(filestream.is_open()) {
        std::cout << "Fileget::openModel: Load file: " << search << std::endl;
        filestream.getline(buffer, buffersize, '\0');
    } else {
        std::cout << "Fileget::openModel: Can't find models: " << s << std::endl;
    }

    filestream.close();

    return buffer;
}

std::string Fileget::openParts(std::string s) {
    for (auto &i : s)
        i = tolower(i);
    std::vector<std::string> search = {
        path + "parts/" + s,
        path + "parts/s" + s,
        path + "p/" + s,
    };
    
    decltype(search.size()) begin = -1;
    decltype(search.size()) end = search.size();
    do {
        begin++;
        filestream.close();
        filestream.open(search[begin]);
    } while (!filestream.is_open() && begin < end);

    if (filestream.is_open()) {
        std::cout << "Fileget::openParts: Load file: " << search[begin] << std::endl;
        filestream.getline(buffer, buffersize, '\0');
    } else {
        std::cout << "Fileget::openParts: Can't find parts: " << s << std::endl;
        return "";
    }

    filestream.close();

    return buffer;
}
