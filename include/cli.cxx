/*******************************************
 * @title   CLI handler
 * @brief   handle CLI command
 * @author  Bin Qu
 * @date    2019.9.28
 * @copyright   You can edit and remodify
 * this file.
*******************************************/

namespace sds
{
    /**
     * @brief token for cli command
    */
    enum CLITokType
    {
        tok_invalid = -1,
        tok_equal = -2,
        tok_param = -3,
        tok_string = -4,
        tok_integer = -5,
        tok_float = -6,
        tok_short_param = -7
    };

    /**
     * @brief 
    */
    enum CLIState
    {

    };

    /**
     * @brief lexer for cli command
    */
    class CLILexer
    {
    private:
        /**
         * @brief argument value
        */
        char* argv;
    public:

        

        /**
         * @brief constructor
         * @param argc the count of cli arguments
         * @param argv the values of cli arguments
        */
        CLILexer(char* argv)
            : argv(argv)
        {}
    };

    /**
     * @brief abstract tree for cli command
    */
    class CLIAST
    {

    };

    /**
     * @brief perform action based on CLIAST
    */
    class CLIAction
    {

    };
}