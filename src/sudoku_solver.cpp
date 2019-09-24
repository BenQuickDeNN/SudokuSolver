/*******************************************
 * @title   Sudoku Solver
 * @brief   A project that automatically
 * solves sudoku.
 * @author  Bin Qu
 * @date    2019.9.23
 * @copyright   You can edit and remodify
 * this project.
*******************************************/

#include "element.h"

#include <cstdio>

void test1();

int main()
{
    /* test field */
    test1();
    /* load sudoku file */

    /* initialize global vars */

    /* compute the number of blocks */

    /* generate candidates */

    /* fill lattices */

    /* excluding */

    /* output the solution */

    return 0;
}

/* test for grid */
void test1()
{
    std::printf("start test1...\r\n");

    sds::Grid grid(9, 3);

    std::printf("print grid lattices:\r\n");
    for (int i = 0; i < grid.Length(); i++)
    {
        for (int j = 0; j < grid.Length(); j++)
            std::printf("%d\t", grid(i, j));
        std::printf("\r\n");
    }

    std::printf("print digit mask:\r\n");
    for (int i = 0; i < grid.Length(); i++)
    {
        for (int j = 0; j < grid.Length(); j++)
        {   
            for (char digit = 1; digit <= 
                grid.Length(); digit++)
                std::printf("%d\t", grid(i, j, digit));
            std::printf("\r\n");
        }
        std::printf("\r\n");
    }
}