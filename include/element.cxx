/*******************************************
 * @title   Grid
 * @brief   define grid type
 * @author  Bin Qu
 * @date    2019.9.23
 * @copyright   You can edit and remodify
 * this file.
*******************************************/

#include <cstdlib>
#include <map>

/// namespace sudoku solver
namespace sds
{   
typedef char byte;
    class Grid
    {
    private:
        /**
         * @brief element lattice
        */
        char* lattices;
        /**
         * @brief candidated digit
         * mask
        */
        byte* mask;
        unsigned int mask_len;

        /**
         * @brief get upper bound
        */
        inline byte my_ceil(byte a, byte b)
        { return (a + b - 1) / b; }

        /**
         * @brief grid size length
        */
        unsigned int length;
        /**
         * @brief length of block
        */
        unsigned int blocklength;
    public:

        const unsigned int& Length() {return length;}

        unsigned int Size() {return length * length;}

        const unsigned int& BlockLength()
        { return blocklength;}

        unsigned int BlockSize()
        { return blocklength * blocklength; }
        
        /**
         * @brief map block address to lattice
         * address
        */
        char& Block(const unsigned int& i,
            const unsigned int& j)
        { return lattices[i * BlockSize() + 
            j * blocklength]; }
        
        /**
         * @brief overload "(i, j)"
         * @return element(i, j)
        */
        char& operator()(const unsigned int& i,
            const unsigned int& j)
        { return lattices[i * length + j]; }

        /**
         * @brief overload "(i, j)"
         * @return if the digit is available
        */
        bool operator()(const unsigned int& i,
            const unsigned int& j, const byte& digit)
        { 
            const byte len_byte = sizeof(byte);
            return (mask[i * length * length + j * length +
                (my_ceil(digit, len_byte) - 1)] &
                (0x01 << ((digit - 1) % len_byte))) >>
                (digit % len_byte);
        }

        /**
         * @brief constructor that allocates
         * memory
        */
        Grid(const unsigned int& length,
            const unsigned int& blocklength)
            :length(length), blocklength(blocklength)
        {
            /* allocate memory for lattices */
            lattices = (char*)std::malloc(
                Size() * sizeof(char));
            /* set zero */
            for (int i = 0; i < Size(); i++)
                lattices[i] = 0;

            /* allocate memory for mask */
            mask_len = Size() * my_ceil(length, sizeof(byte));
            mask = (byte*)std::malloc(mask_len *
                sizeof(byte));
            /* set true */
            for (int i = 0; i < mask_len; i++)
                mask[i] = 0xFF;
        }

        /**
         * @brief deconstructor that frees
         * memory
        */
        ~Grid()
        {
            /* free memory */
            std::free(lattices);
            std::free(mask);
        }
    };
}