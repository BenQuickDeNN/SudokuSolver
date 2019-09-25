/*******************************************
 * @title   CSV file reader
 * @brief   read CSV file
 * @author  Bin Qu
 * @date    2019.9.25
 * @copyright   You can edit and remodify
 * this file.
*******************************************/

#include <cctype>
#include <cstdio>
#include <string>

namespace sds
{
    enum CSVTokType
    {
        tok_empty = -1,
        tok_digit = -2,
        tok_invalid = -3
    };
    enum CSVState
    {
        START, S1, S2
    };
    class CSVLexer
    {
    private:
        unsigned int idx = 0;
        unsigned int line = 1;
        unsigned int col = 1;
        std::string content;

        void logError(std::string info)
        { std::fprintf(stderr, "%s in line %d, col %d\r\n", info, line, col); }

    public:

        void resetIdx() 
        {
            idx = 0;
            line = 1;
            col = 1;
        }

        int getCSVTok()
        {
            std::string digitStr;
            CSVState state = CSVState::START;
            char lastChar;
            while (idx <= content.size())
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
                        return std::stoi(digitStr);
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

        CSVLexer(const std::string& content)
            : content(content)
        { }
    };
}