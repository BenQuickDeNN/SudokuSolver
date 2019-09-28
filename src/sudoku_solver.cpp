/*******************************************
 * @title   Sudoku Solver
 * @brief   A project that automatically
 * solves sudoku.
 * @author  Bin Qu
 * @date    2019.9.23
 * @copyright   You can edit and remodify
 * this project.
*******************************************/

#include "cli.h"
#include "test.cpp"

#include <getopt.h>



int main(int argc, char** argv)
{
    /* test field */
    //test1();
    //test2();
    //test3();

    /* initialize variable */
    char* filename = nullptr;
    int c;

    /* process short options */
    while ((c = getopt(argc, argv, "hvf:")) != -1)
        switch (c)
        {
        case 'h':
            sds::showHelpInfo();
            break;
        case 'v':
            sds::showVersionInfo();
            break;
        default:
            sds::showInvalidCLIInfo;
            break;
        }

    /* load sudoku file */

    /* initialize global vars */

    /* compute the number of blocks */

    /* generate candidates */

    /* fill lattices */

    /* excluding */

    /* output the solution */

    return 0;
}