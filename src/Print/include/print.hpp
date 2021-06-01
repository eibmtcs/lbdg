#ifndef __PRINT_HPP__
#define __PRINT_HPP__

#include <string>
#include <vector>

class Print {
public:
    /* Print() = default; */
    Print(std::string modelName, std::vector<std::vector<std::pair<std::string, int>>> thumbnail);

    void makeHtml();

    void makePdf();

private:
    std::string modelName;
    std::vector<std::vector<std::pair<std::string, int>>> thumbnail;
};

#endif
