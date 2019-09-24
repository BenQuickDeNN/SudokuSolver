/*******************************************
 * @title   File Interpreter
 * @brief   Implementation of lexer and 
 * parser
 * @author  Bin Qu
 * @date    2019.9.24
 * @copyright   You can edit and remodify
 * this file.
*******************************************/

#include <cstdio>
#include <cctype>
#include <string>

namespace sds
{
    enum TokType
    {
        tok_empty = 0,
        tok_eof = -1,
        tok_var = -2,
        tok_digit = -3,
        tok_equal = -4,
        tok_lb = -5,
        tok_rb = -6
    };

    enum State
    {
        START,
        S1,
        S2,
        S3,
        S4,
        S5,
        S6,
        S7,
        EMPTY,
        VAR,
        DIGIT,
        EQUAL,
        LB,
        RB
    };

    struct Token
    {
        std::string text;
        TokType tokType;
    };

    static void logError(const std::string& info,
        const unsigned int& index)
    { std::fprintf(stderr, "%s in %d\r\n",
        info.c_str(), index); }

    class Lexer
    {
    private:
        /* content index */
        unsigned int idx = 0;
        /* script content */
        std::string content;
    public:

        void resetIndex() { idx = 0; }

        Token* getNextTok()
        {
            State state = State::START;
            char lastChar;
            while (idx < content.size())
            {
                idx++;
                lastChar = content.c_str()[idx - 1];
                switch (state)
                {
                case State::START:
                    if ('#' == lastChar)
                        state = State::S1;
                    else if (std::isalpha(lastChar))
                        state = State::S2;
                    else if (' ' == lastChar || '\n' == lastChar)
                        state = State::S3;
                    else if (std::isdigit(lastChar))
                        state = State::S4;
                    else if ('=' == lastChar)
                        state = State::S5;
                    else if ('[' == lastChar)
                        state = State::S6;
                    else if (']' == lastChar)
                        state = State::S7;
                    else if (EOF == lastChar)
                    {
                        state = State::EMPTY;
                        Token token;
                        token.text = "";
                        token.tokType = TokType::
                            tok_empty;
                        return &token;
                    }
                    else
                    {
                        logError("invalid character!", idx);
                        return nullptr;
                    }
                    break;
                case State::S1:
                case State::S2:
                case State::S3:
                case State::S4:
                case State::S5:
                case State::S6:
                case State::S7:
                
                default:
                    logError("unknown state!", idx);
                    return nullptr;
                }
                
            }
            logError("not a completed token!", idx);
            return nullptr;
        }

        /**
         * @brief constructor that assigns
         * content.
        */
        Lexer(std::string content)
            : content(content)
        {}
    };
}