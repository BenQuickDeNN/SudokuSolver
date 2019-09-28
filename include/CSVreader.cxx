/*******************************************
 * @title   CSV file reader
 * @brief   read CSV file
 * @author  Bin Qu
 * @date    2019.9.25
 * @copyright   You can edit and remodify
 * this file.
*******************************************/

#include "element.h"
#include "FileHandler.h"

#include <cctype>
#include <cmath>
#include <cstdio>
#include <string>

namespace sds
{
    /**
     * Token type of lexer
    */
    enum CSVTokType
    {
        tok_empty = -1,
        tok_digit = -2,
        tok_invalid = -3,
        tok_eof = -4
    };

    /**
     * State of lexer
    */
    enum CSVState
    {
        START, S1, S2
    };

    /**
     * Lexer that generates tokens in CSV file.
    */
    class CSVLexer
    {
    private:
        /**
         * content index
        */
        unsigned int idx = 0;
        /**
         * content line
        */
        unsigned int line = 1;
        /**
         * content column
        */
        unsigned int col = 1;
        /**
         * content text
        */
        std::string content;

        /**
         * @brief handle error info
         * @param info error information
        */
        void logError(std::string info)
         { std::fprintf(stderr, "lexxing error: %s in line %d, col %d\r\n", info, line, col); }

    public:

        /**
         * @brief reset content index
         * You should call this function
         * before starting new lexing.
        */
        void resetIdx();

        /**
         * @brief get token in csv file
         * @return digital token or tokType
        */
        int getCSVTok();

        /**
         * @brief constructor that inputs content
         * @param content conttent text
        */
        CSVLexer(const std::string& content)
            : content(content)
        { }
    };

    void CSVLexer::resetIdx()
    {
        idx = 0;
        line = 1;
        col = 1;
    }

    int CSVLexer::getCSVTok()
    {
        if (idx >= content.size())
            return CSVTokType::tok_eof;
        std::string digitStr;
        CSVState state = CSVState::START;
        char lastChar;
        while (idx < content.size())
        {
            lastChar = content.c_str()[idx];
            idx++;
            col++;
            switch (state)
            {
            case CSVState::START:
                if (' ' == lastChar || '\t' == lastChar)
                    state = CSVState::S1;
                else if (std::isdigit(lastChar))
                {
                    state = CSVState::S2;
                    digitStr = lastChar;
                }
                else if ('\n' == lastChar || EOF == lastChar)
                {
                    line++;
                    col = 1;
                    return CSVTokType::tok_empty;
                }
                else
                {
                    logError("invalid symbol " + lastChar);
                    return CSVTokType::tok_invalid;
                }
                break;
            case CSVState::S1:
                if (' ' == lastChar || '\t' == lastChar)
                    break;
                else if (std::isdigit(lastChar))
                {
                    state = CSVState::S2;
                    digitStr = lastChar;
                }
                else if ('\n' == lastChar || EOF == lastChar)
                {
                    line++;
                    col = 1;
                    return CSVTokType::tok_empty;
                }
                else
                {
                    logError("invalid symbol " + lastChar);
                    return CSVTokType::tok_invalid;
                }
                break;
            case CSVState::S2:
                if (std::isdigit(lastChar))
                    digitStr += lastChar;
                else if (',' == lastChar ||
                    ' ' == lastChar || '\n' == lastChar ||
                    EOF == lastChar)
                {
                    if ('\n' == lastChar)
                    {
                        line++;
                        col = 1;
                    }
                    return std::stoi(digitStr);
                }
                else
                {
                    logError("invalid symbol " + lastChar);
                    return CSVTokType::tok_invalid;
                }
                break;
            default:
                logError("unknown state");
                return CSVTokType::tok_invalid;
            }
        }
        return CSVTokType::tok_empty;
    }

    /**
     * @brief convert csv data to grid entity
     * @param filename csv file path
     * @return grid entity
    */
    static Grid* CSVtoGrid(std::string filename)
    {
        std::string content = readText(filename);
        std::printf("read csv file %s...done\r\n", filename.c_str());
        int tmpChar;
        unsigned int idx = 0;
        char buf[content.size()];
        CSVLexer csvlexer(content);
        csvlexer.resetIdx();
        for (int i = 0; i < content.size(); i++)
        {
            tmpChar = csvlexer.getCSVTok();
            if (tmpChar == CSVTokType::tok_eof)
                break;
            if (tmpChar != CSVTokType::tok_empty &&
                tmpChar != CSVTokType::tok_invalid)
            {
                buf[idx] = tmpChar;
                idx++;
            }
        }
        std::printf("csv lexing done, the number of digits = %d.\r\n", idx);
        /* check if the grid is a square? */
        if (
            idx != 4*4 && idx != 9*9 && idx != 16*16 &&
            idx != 25*25 && idx != 36*36 && idx != 49*49 &&
            idx != 64*64 && idx != 81*81 && idx != 100*100
        )
        {
            std::fprintf(stderr, "error: %s is not a sudoku game file\r\n", filename);
            return nullptr;
        }
        /* initialize a grid */
        unsigned int length = std::sqrt(idx);
        unsigned int blocklength = std::sqrt(length);
        /* add static modifier to keep grid global */
        static Grid grid(length, blocklength);
        for (int i = 0; i < length; i++)
            for (int j = 0; j < length; j++)
                grid(i, j) = buf[i * length + j];
        return &grid;
    }
}