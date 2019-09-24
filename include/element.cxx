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
    private:
        bool editable = true;
        /**
         * @brief grid indicator, ranging
         * from 1~blocksize^2
        */
        char value;
    public:
        static const char ASCII_ZERO = '0';
        /**
         * @brief digit that can be
         * chosen as value, ranging from
         * 1~blocksize^2
        */
        static char
            *available_digits;
        /**
         * @brief values that may be used to
         * fill this lattice
        */
        std::vector<char>
            candidated_values;

        /**
         * @brief set value of the lattice
         * @param value input value
         * @return is the setting success?
        */
        bool setValue(char value)
        {
            if (editable) 
                this->value = value;
            else
                return false;
            return true;
        }

        /**
         * @brief get value of the lattice
         * @return value of the lattice
        */
        char getValue() { return value; }
        
        /**
         * @brief constructor that sets value
         * and editability
         * @param value input value
         * @param editable is the lattice
         * editable?
        */
        Lattice(char value, bool editable)
            : value(value), editable(editable)
        {}
        
        /**
         * @brief constructor that does nothing
        */
        Lattice()
        {}
    };
    
    class Block
    {
    private:
        static unsigned int Blocksize;
    public:

    };

    class Grid
    {
    public:
        
    };
}