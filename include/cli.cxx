/*******************************************
 * @title   CLI handler
 * @brief   handle CLI command & infomation
 * @author  Bin Qu
 * @date    2019.9.28
 * @copyright   You can edit and remodify
 * this file.
*******************************************/

#ifndef CLI_MESSAGE_H
#define CLI_MESSAGE_H
#include "cli_message.h"
#endif

#include <cstdio>

namespace sds
{
    /**
     * @brief display help info in CLI
    */
    static void showHelpInfo();
    /**
     * @brief display version info in CLI
    */
    static void showVersionInfo();
    /**
     * @brief display invalid info in CLI
    */
    static void showInvalidCLIInfo();

    static void showHelpInfo()
    { std::printf("%s", CLI_HELP.c_str()); }

    static void showVersionInfo()
    { std::printf("%s", CLI_VERSION.c_str()); }

    static void showInvalidCLIInfo()
    {
        std::printf("%s", CLI_INVALID_INFO.c_str());
        showHelpInfo();
    }
}