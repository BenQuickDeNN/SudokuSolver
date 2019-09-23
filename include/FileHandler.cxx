/*******************************************
 * @title   File Handler
 * @brief   load sudoku file and output the
 * @author  Bin Qu
 * @date    2019.9.23
 * @copyright   You can edit and remodify
 * this file.
*******************************************/

//#include <string>   // string

namespace sudoku_solver
{
    class SudokuFile
    {
    private:

        unsigned int gridsize;
        unsigned int blocksize;
        unsigned int numOfBlocks;

    public:
        /**
        * @brief load sudoku game file
        */
        static void loadGame();

        /**
        * @brief output solution to file
        */
        static void outputSolution();

        /**
         * @brief constructor
        */
        SudokuFile(int gridsize, int blocksize)
            : gridsize(gridsize), 
            blocksize(blocksize)
        {
            numOfBlocks = gridsize / blocksize;
            numOfBlocks *= numOfBlocks;
        }
        
    };
}