#include <iostream>
#include "print.hpp"
#include "fstream"

Print::Print(std::string modelName, std::vector<std::vector<std::pair<std::string, int>>> thumbnail = {{{}}})
{
    this->modelName = modelName;
    this->thumbnail = thumbnail;
}


void Print::makeHtml()
{
    std::ofstream b;
    std::string indexFile = "./" + this->modelName + "/index.html";
    /* std::cerr << this->modelName << std::endl << indexFile << std::endl; */
    std::string data;

    bool firstInI, firstInJ;
    firstInI = firstInJ = true;
    for (auto &i : thumbnail) {
        if (!firstInI)
            data += ",\n";
        data += "[";
        firstInJ = true;
        for (auto &j : i) {
            if (!firstInJ)
                data += ",";
            data += "[\"" + j.first.substr(0, j.first.size()-4) + "\"," + std::to_string(j.second) + "]";
            firstInJ = false;
        }
        data += "]";
        firstInI = false;
    }
    /* std::cerr << data << std::endl; */

    b.open(indexFile);
    std::string a =
      " <!doctype html>\n"
      " <html>\n"
      "     <head>\n"
      " <style>\n"
      " .steps {\n"
      "     width: 800px;\n"
      "     height: 600px;\n"
      "     position: relative;\n"
      "     margin: 10px auto;\n"
      " }\n"
      " .parts {\n"
      "     position: absolute;\n"
      "     top: 20px;\n"
      "     left: 25px;\n"
      "     min-width: 80px;\n"
      "     max-width: 165px;\n"
      "     height: auto;\n"
      "     background: #ffffff;\n"
      "     border: 2px solid;\n"
      "     border-radius: 5px;\n"
      "     padding: 2px;\n"
      " }\n"
      " .part {\n"
      "     float: left;\n"
      "     height: 47px;\n"
      "     width: 40px;\n"
      " }\n"
      " .thumbnail {\n"
      "     height: 30px;\n"
      "     width: 40px;\n"
      " }\n"
      " .quantity {\n"
      "     font-weight: bold;\n"
      "     font-size: 12px;\n"
      "     text-align: center;\n"
      "     height: 8px;\n"
      "     width: 40px;\n"
      " }\n"
      " .model {\n"
      "     width: 100%;\n"
      "     height: 100%;\n"
      " }\n"
      " .numberBox {\n"
      "     position: absolute;\n"
      "     bottom: 40px;\n"
      "     right: 50px;\n"
      "     height: 45px;\n"
      "     width: 45px;\n"
      "     background: #ffffff;\n"
      "     border: 2px solid;\n"
      "     border-radius: 50px;\n"
      " }\n"
      " .number {\n"
      "     position: absolute;\n"
      "     top: 50%;\n"
      "     width: 50%;\n"
      "     transform: translate(50%, -50%);\n"
      "     text-align: center;\n"
      "     font-size: 20px;\n"
      "     font-weight: bold;\n"
      " }\n"
      " </style>\n"
      "     </head>\n"
      "     <body>\n"
      "         <div id=\"steps\"></div>\n"
      "         <script>\n"
      "             var modleName;\n"
      "             var thumbnail = new Array();\n"
      "             modleName = \"" + this->modelName + "\";\n"
      "             thumbnail = [\n"
      + data +

      /*
                       ["4315",2],["4600",2],["3031",1]
                       ["3024",4],["3020",1],["3623",2],["3021",1],["3710",1],["4079",1]
                       ["3024",2],["3829c01",1]

                       [["3788",2],["3024",2]],
                       [["4315",2],["4600",2],["3031",1]],
                       [["3024",4],["3020",1],["3623",2],["3021",1],["3710",1],["4079",1]],
                       [["3024",2],["3829c01",1]],
                       [["3788",2],["3024",2]],
                       [["3937",1],["3938",1],["4070",2],["6141",2],["3023",2],["3822",1],["3821",1],["3005",2],["3623",2],["3004",1]],
                       [["3068b",1],["3023",6],["3004",1],["3024",2],["3710",1]],
                       [["3823",2],["4214",1]],
                       [["4213",1],["3020",1],["4624",4],["3641",4]]
                       */

      "             ];\n"
      "             for (i = 0; i < thumbnail.length; i++) {\n"
      "                 document.getElementById(\"steps\").innerHTML += \"<div class='steps'><div class='parts' id='part\" + i + \"'></div><img class='model' src='./images/\" + modleName + \" [\" + (i+1) + \" - \" + thumbnail.length + \"].png'><div class='numberBox'><div class='number'>\" + (i+1) + \"</div></div></div>\\n\";\n"
      "                 for (j = 0; j < thumbnail[i].length; j++) {\n"
      "                     document.getElementById(\"part\" + i).innerHTML += \"<div class='part'><img class='thumbnail' src='/home/eibmtcs/Workhome/opengl/thumbnail/\" + thumbnail[i][j][0] + \".png'><div class='quantity'>\" + thumbnail[i][j][1] + \"x</div></div>\";\n"
      "                 }\n"
      "             }\n"
      "         </script>\n"
      "     </body>\n"
      " </html>\n";
    b << a;
    b.close();
}
