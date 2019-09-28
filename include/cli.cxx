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
     * @brief lexer for cli command
    */
    class CLILexer
    {
    private:
        int argc;
        char** argv;
    public:

        /**
         * @brief constructor
         * @param argc the count of cli arguments
         * @param argv the values of cli arguments
        */
        CLILexer(const int& argc, char** argv)
            : argc(argc), argv(argv)
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