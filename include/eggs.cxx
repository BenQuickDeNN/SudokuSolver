/*******************************************
 * @title   eggs
 * @brief   Eggs for this project
 * @author  Bin Qu
 * @date    2019.9.28
 * @copyright   You can edit and remodify
 * this file.
*******************************************/

#include <ctime>
#include <cstdio>
#include <string>

namespace sds
{
    /* celebrate the birthday */
    static std::string BirthDayInfo =
"\
Today is 5th Oct, happy birth day to my love!\r\n\
------------------------------\r\n\
          *        *          \r\n\
       *     *  *     *       \r\n\
       *              *       \r\n\
        *            *        \r\n\
         *          *         \r\n\
            *     *           \r\n\
               *              \r\n\
------------------------------\r\n\
";

    /**
     * @brief show birthday info
    */
    static void celebrateBirthDay();

    static void celebrateBirthDay()
    {
        std::time_t now = std::time(0);
        std::tm* ltm = std::localtime(&now);
        if (ltm->tm_mon + 1 == 10 && ltm->tm_mday == 5)
        std::printf("%s", BirthDayInfo.c_str());
    }
}