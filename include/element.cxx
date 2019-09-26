/*******************************************
 * @title   Grid
 * @brief   define grid type
 * @author  Bin Qu
 * @date    2019.9.23
 * @copyright   You can edit and remodify
 * this file.
*******************************************/

#include <cstdio>
#include <cstdlib>
#include <map>

/// namespace sudoku solver
namespace sds
{   
typedef unsigned char byte;
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
        unsigned int mask_cell_len;
        const byte len_byte = 8;

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
         * @brief set mask bit
         * @param i 1-d address
         * @param digit which bit
         * @param candidate can I use this digit?
        */
        inline void setMaskBit(const unsigned int& i, const byte& digit,
            const bool& candidate)
        {
            if (candidate)
                mask[i * mask_cell_len +
                    (my_ceil(digit, len_byte) - 1)] |=
                    (0x01 << ((digit - 1) % len_byte));
            else
                mask[i * mask_cell_len +
                    (my_ceil(digit, len_byte) - 1)] &=
                    (0xFE << ((digit - 1) % len_byte));
        }

        /**
         * @brief overload "(i, j)"
         * @return if the digit is available
        */
        bool operator()(const unsigned int& i,
            const unsigned int& j, const byte& digit)
        { 
            return (mask[i * length * mask_cell_len +
                j * mask_cell_len +
                (my_ceil(digit, len_byte) - 1)] &
                (0x01 << ((digit - 1) % len_byte))) >>
                ((digit - 1) % len_byte);
        }

        /**
         * @update candidated digit mask
        */
        void updateMask()
        {
            byte buf_mask[mask_cell_len];
            /* scan lattice */
            /* row scan */
            std::printf("update row mask...\r\n");
            for (int row = 1; row <= length; row++)
            {
                /* initialize buffer mask */
                for (int i = 0; i < mask_cell_len; i++)
                {
                    buf_mask[i] = 0xFF;
                    if (i == mask_cell_len - 1)
                        buf_mask[i] >>= len_byte - length % len_byte;
                }

                /* modify buffer mask */
                for (int i = (row - 1) * length;
                    i < row * length; i++)
                    if (lattices[i] != 0)
                        buf_mask[my_ceil(lattices[i], len_byte) - 1] &=
                            ~(0x01 << ((lattices[i] - 1) % len_byte));
                
                /* set mask (I suggest to vectorize this loop) */
                for (int i = (row - 1) * length * mask_cell_len;
                    i < row * length * mask_cell_len; i++)
                        mask[i] &= buf_mask[i % mask_cell_len];
            }

            /* column scan */
            std::printf("update column mask...\r\n");
            /* use scalar to help implement parallelism */
            unsigned int sizegrid = Size();
            for (int col = 1; col <= length; col++)
            {
                /* initialize buffer mask */
                for (int i = 0; i < mask_cell_len; i++)
                {
                    buf_mask[i] = 0xFF;
                    if (i == mask_cell_len - 1)
                        buf_mask[i] >>= len_byte - length % len_byte;
                }
                
                /* modify buffer mask */
                for (int i = col - 1; i < sizegrid - length + col;
                    i += length)
                    if (lattices[i] != 0)
                        buf_mask[my_ceil(lattices[i], len_byte) - 1] &=
                            ~(0x01 << ((lattices[i] - 1) % len_byte));
                
                /* set mask */
                for (int i = col - 1; i < sizegrid - length + col;
                    i += length)
                    for (int j = 0; j < mask_cell_len; j++)
                        mask[i * mask_cell_len + j] &= buf_mask[j];
            }
            
            /* block scan */
            std::printf("update block mask...\r\n");
            for (int block_y = 1; block_y <= blocklength; block_y++)
                for (int block_x = 1; block_x <= blocklength; block_x++)
                {
                    /* initialize buffer mask */
                    for (int i = 0; i < mask_cell_len; i++)
                    {
                        buf_mask[i] = 0xFF;
                        if (i == mask_cell_len - 1)
                            buf_mask[i] >>= len_byte - length % len_byte;
                    }
                    
                    /* modify buffer mask */
                    for (int i = (block_y - 1) * blocklength;
                        i < block_y * blocklength; i++)
                        for (int j = (block_x - 1) * blocklength;
                            j < block_x * blocklength; j++)
                        {
                            unsigned int k = i * length + j;
                            if (lattices[k] != 0)
                                buf_mask[my_ceil(lattices[k], len_byte) - 1] &=
                                    ~(0x01 << ((lattices[k] - 1) % len_byte));
                        }

                    /* set mask */
                    for (int i = (block_y - 1) * blocklength;
                        i < block_y * blocklength; i++)
                        for (int j = (block_x - 1) * blocklength;
                            j < block_x * blocklength; j++)
                            for (int k = 0; k < mask_cell_len; k++)
                                mask[i * length * mask_cell_len +
                                     j * mask_cell_len + k] &= buf_mask[k];
                }

            /* scan where mask should be 0 */
            for (int i = 0; i < sizegrid; i++)
                if (lattices[i] != 0)
                    for (int j = 0; j < mask_cell_len; j++)
                        mask[i * mask_cell_len + j] = 0;
        }

        /**
         * @brief fill blanks that only conains one
         * candidate.
        */
        void fill()
        {
            std::printf("fill blanks...\r\n");
            /* traversing lefttop to rightdown */
            unsigned int sizegrid = length * length;
            for (int i = 0; i < sizegrid; i++)
            {
                if (lattices[i] == 0)
                {
                    for (int j = 0; j < mask_cell_len; j++)
                    {
                        switch (mask[i * mask_cell_len + j])
                        {
                        case 1:
                            lattices[i] += j * len_byte + 1;
                            break;
                        case 2:
                            lattices[i] += j * len_byte + 2;
                            break;
                        case 4:
                            lattices[i] += j * 8 + 3;
                            break;
                        case 8:
                            lattices[i] += j * 8 + 4;
                            break;
                        case 16:
                            lattices[i] += j * 8 + 5;
                            break;
                        case 32:
                            lattices[i] += j * 8 + 6;
                            break;
                        case 64:
                            lattices[i] += j * 8 + 7;
                            break;
                        case 128:
                            lattices[i] += j * 8 + 8;
                            break;
                        default:
                            break;
                        }
                    }
                    /* check lattice value */
                    if (lattices[i] > length)
                        lattices[i] = 0;
                }
            }
        }

        /**
         * @brief use i-excluding algorithm to further modify
         * mask.
         * @return is any update?
        */
        bool excluding()
        {

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
            mask_cell_len = my_ceil(length, len_byte);
            mask_len = Size() * mask_cell_len;
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