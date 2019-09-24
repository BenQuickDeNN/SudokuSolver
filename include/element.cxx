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
        bool* mask;
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
            const unsigned int& j, const char& digit)
        { return mask[i * length * length + j *
            length + (digit - 1)]; }

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
            mask = (bool*)std::malloc(Size() *
                length * sizeof(bool));
            /* set true */
            for (int i = 0; i < Size() * length;
                i++)
                mask[i] = true;
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