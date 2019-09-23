/*******************************************
 * @title   Grid
 * @brief   define grid type
 * @author  Bin Qu
 * @date    2019.9.23
 * @copyright   You can edit and remodify
 * this file.
*******************************************/

#include <vector>

namespace sudoku_solver
{
    class Lattice
    {
    public:
        static const char ASCII_ZERO = '0';
        /**
         * @brief grid indicator, ranging
         * from 1~blocksize^2
        */
        unsigned char value;
        /**
         * @brief digit that can be
         * chosen as value, ranging from
         * 1~blocksize^2
        */
        static unsigned char
            *available_digits;
        /**
         * @brief values that may be used to
         * fill this lattice
        */
        std::vector<unsigned char>
            candidated_values;
    };
}