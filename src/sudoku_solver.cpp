/*******************************************
 * @title   Sudoku Solver
 * @brief   A project that automatically
 * solves sudoku.
 * @author  Bin Qu
 * @date    2019.9.23
 * @copyright   You can edit and remodify
 * this project.
*******************************************/

#ifndef CLI_OPTIONS_H
#define CLI_OPTIONS_H
#include "cli_options.h"
#endif

#include "cli.h"
#include "CSVreader.h"
#include "eggs.h"
#include "element.h"
#include "FileHandler.h"

#include "test.cpp"

#include <getopt.h>



int main(int argc, char** argv)
{
    /* test field */
    //test1();
    //test2();
    //test3();

    ///* initialize variable */
    char* filename = nullptr;
    int c;
    int option_index;

    ///* process options */
    while (true)
    {
        option_index = 0;
        c = getopt_long(argc, argv, "hvf:", long_options, &option_index);
        /* detect the end of the options */
        if (c == -1)
            break;
        switch (c)
        {
        case 'h':
            sds::showHelpInfo();
            break;
        case 'v':
            sds::showVersionInfo();
            break;
        case 'f':
            filename = optarg;
            break;
        case '?':
            break;
        default:
            sds::showInvalidCLIInfo();
            abort();
        }
    }

    int returnCode = 0;
    ///* load sudoku file */
    if (filename != nullptr)
    {
        sds::Grid* grid = sds::CSVtoGrid(filename);
        if (grid == nullptr)
        {
            std::fprintf(stderr, "Fail to load the file %s, abort...\r\n", filename);
            abort();
        }
        std::printf("The initialized sudoku game is:\r\n");
        grid->dispGrid();

        ///* initialize global vars */
        std::printf("Initializing mask...\r\n");
        grid->initializeMask();

        ///* solve sudoku */
        unsigned int ie = 2;
        while (true)
        {
            /* kernel */
            while(grid->fill()) { }
            if (grid->isCompleted())
                break;
            if (grid->excluding(ie))
            {
                ie = 2;
                continue;
            }
            ie++;
            if (ie > grid->Length())
                break;
        }

        ///* output the solution */
        if (grid->isCompleted())
        {
            std::printf("The sudoku is compeleted\r\n");
            std::printf("The solution is:\r\n");
            grid->dispGrid();
        }
        else
        {
            std::fprintf(stderr, "bad sudoku: the solution of the sudoku may be multiple\r\n");
            returnCode = -1;
        }
    }

    ///* play eggs */
    sds::celebrateBirthDay();

    ///* exit program */
    exit(returnCode);
}