/*******************************************
 * @title   File Handler
 * @brief   load sudoku file and output the
 * @author  Bin Qu
 * @date    2019.9.23
 * @copyright   You can edit and remodify
 * this file.
*******************************************/

#include <cstdio>   // fprintf
#include <fstream>  // handling file
#include <string>   // string

namespace sds
{
    /**
     * @brief read text content from file
     * @param filename file path
     * @return text content
    */
    static std::string readText(std::string filename);

    static std::string readText(std::string filename)
    {
        std::string content;
        std::string tempStr;
        std::ifstream tmpFile(filename);
        if (tmpFile.is_open())
        {
            while (std::getline(tmpFile, tempStr))
                content += tempStr + "\n";
            /* set EOF at last element */
            content[content.size() - 1] = EOF;
        }
        else
            std::fprintf(stderr, "cannot open the file \"%s\"\r\n",
                filename.c_str());
        tmpFile.close();
        return content;
    }
}