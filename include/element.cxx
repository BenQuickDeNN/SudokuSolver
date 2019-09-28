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
        /**
         * @brief the length of mask array
        */
        unsigned int mask_len;
        /**
         * @brief the length of mask element
        */
        unsigned int mask_cell_len;
        /**
         * @brief the length of a byte (bit)
        */
        const byte len_byte = 8;

        /**
         * @brief get upper bound
        */
        static inline byte my_ceil(byte a, byte b)
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
            const bool& candidate);

        /**
         * @brief overload "(i, j)"
         * @return if the digit is available
        */
        bool operator()(const unsigned int& i,
            const unsigned int& j, const byte& digit)
        { 
            return (mask[i * length * mask_cell_len +
                j * mask_cell_len + (my_ceil(digit, len_byte) - 1)] &
                (0x01 << ((digit - 1) % len_byte))) >>
                ((digit - 1) % len_byte);
        }

        /**
         * @brief update candidated digit mask
         * you should call this function before
         * solving problem.
        */
        void initializeMask();

        /**
         * @brief update the a row of mask.
         * @param row row number.
        */
        void update_row_mask(unsigned int row);

        /**
         * @brief update column mask
         * @@param col column number
        */
        void update_col_mask(unsigned int col);

        /**
         * @brief update mask trough block
         * @param block_y y axis of block
         * @param block_x x axis of block
        */
        void update_block_mask(unsigned int block_y,
            unsigned int block_x);

        /**
         * @brief fill blanks that only conains one
         * candidate.
         * @return is update?
        */
        bool fill();

        /**
         * @brief judge if a sudoku is completed
         * @return is the sudoku completed?
        */
        bool isCompleted();

        /**
         * @brief use i-excluding algorithm to further modify
         * mask.
         * @param i how many bits are referenced?
         * @return is any update?
        */
        bool excluding(const unsigned int& ie);

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

    inline void Grid::setMaskBit(const unsigned int& i, const byte& digit,
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

    void Grid::initializeMask()
    {
        byte buf_mask[mask_cell_len];
        /* scan lattice */
        /* row scan */
        std::printf("initialize row mask...\r\n");
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
        std::printf("initialize column mask...\r\n");
        /* use scalar to help implement parallelism */
        const unsigned int sizegrid = Size();
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
        std::printf("initialize block mask...\r\n");
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
    
    void Grid::update_row_mask(unsigned int row)
    {
        byte buf_mask[mask_cell_len];
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

    void Grid::update_col_mask(unsigned int col)
    {
        byte buf_mask[mask_cell_len];
        const unsigned int sizegrid = Size();
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

    void Grid::update_block_mask(unsigned int block_y,
        unsigned int block_x)
    {
        byte buf_mask[mask_cell_len];
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

    bool Grid::fill()
    {
        /* traversing lefttop to rightdown */
        const unsigned int sizegrid = length * length;
        byte tmpMask;
        unsigned int tmpLat;
        bool canFill;
        bool isUpdated = false;
        for (int i = 0; i < sizegrid; i++)
        {
            if (lattices[i] == 0)
            {
                tmpLat = 0;
                canFill = true;
                for (int j = 0; j < mask_cell_len && canFill; j++)
                    for (int k = 0; k < len_byte; k++)
                        if (mask[i * mask_cell_len + j] == (1 << k))
                        {
                            tmpLat += (k + 1) + j * len_byte;
                            canFill = true;
                            break;
                        }
                        else if (mask[i * mask_cell_len + j] != 0)
                            canFill = false;
                /* check tmpLat */
                if (tmpLat <= length && tmpLat >= 1)
                {
                    lattices[i] = tmpLat;
                    update_row_mask( i / length + 1);
                    update_col_mask(i % length + 1);
                    update_block_mask((i / length) / blocklength + 1,
                        (i % length) / blocklength + 1);
                    for (int j = 0; j < mask_cell_len; j++)
                        mask[i * mask_cell_len + j] = 0;
                    isUpdated = true;
                }
            }
        }
        return isUpdated;
    }

    bool Grid::isCompleted()
    {
        /* scan all lattice */
        const unsigned int sizegrid = Size();
        for (int i = 0; i < sizegrid; i++)
            if (lattices[i] == 0)
                return false;
        return true;
    }

    bool Grid::excluding(const unsigned int& ie)
    {
        if (ie < 2)
        {
            std::fprintf(stderr, "i-excluding: i should not less than 2\r\n");
            return false;
        }
        if (ie > length)
        {
            std::fprintf(stderr, "i-excluding: i should not larger than %d\r\n", length);
            return false;
        }
        bool isAnyUpdate = false;
        /* scan candidate, from begin to the end */
        const unsigned int sizegrid = Size();
        unsigned int bitcounter;
        const byte bitFilter = 0x01;
        byte clueMask[mask_cell_len];
        byte tmpByte;
        unsigned int cluecounter;
        unsigned int sIdx;
        unsigned int eIdx;
        bool tmpBool;
        for (int i = 0; i < sizegrid; i++)
        {
            /* filt non-empty lattices */
            if (lattices[i] != 0)
                continue;
            /* check if the mask is with no more than\\
            ie positive bits */
            bitcounter = 0;
            for (int j = 0; j < mask_cell_len; j++)
                for (int k = 0; k < len_byte; k++)
                    bitcounter += ((mask[i * mask_cell_len + j] &
                        (bitFilter << k)) != 0);
            /* filt those mask that bitcount is larger than ie */
            if (bitcounter > ie || bitcounter == 0)
                continue;

            for (int j = 0; j < mask_cell_len; j++)
                clueMask[j] = mask[i * mask_cell_len + j];
                
            /* row scan */
            sIdx= (i / length) * length;
            eIdx = sIdx + length;
            cluecounter = 0;
            for (int j = sIdx; j < eIdx; j++)
            {
                /* excluding mask = 0 */
                tmpByte = 0;
                for (int k = 0; k < mask_cell_len; k++)
                    tmpByte |= mask[j * mask_cell_len + k];
                if (tmpByte == 0)
                    continue;
                tmpBool = true;
                for (int k = 0; k < mask_cell_len && tmpBool; k++)
                    if ((clueMask[k] | mask[j * mask_cell_len + k]) !=
                        clueMask[k])
                        tmpBool = false;
                if (tmpBool)
                    cluecounter++;
                if (cluecounter >= ie)
                {
                    /* scan the row and eliminate non-clues */
                    for (int k = sIdx; k < eIdx; k++)
                    {
                        tmpByte = 0;
                        for (int l = 0; l < mask_cell_len; l++)
                            tmpByte |= mask[k * mask_cell_len + l];
                        if (tmpByte == 0)
                            continue;

                        tmpBool = true;
                        for (int l = 0; l < mask_cell_len && tmpBool; l++)
                            if ((clueMask[l] | mask[k * mask_cell_len + l]) !=
                                clueMask[l])
                                tmpBool = false;
                        /* found non-clue*/
                        if (!tmpBool)
                        {   
                            /* they must have intersection */
                            tmpByte = 0;
                            for (int l = 0; l < mask_cell_len; l++)
                                tmpByte |= (clueMask[l] & mask[k * mask_cell_len + l]);
                            if (tmpByte != 0)
                            {
                                isAnyUpdate = true;
                                for (int l = 0; l < mask_cell_len; l++)
                                    mask[k * mask_cell_len + l] &=
                                        (~clueMask[l]);
                            }
                        }
                    }
                    break;
                }
            }

            /* column scan */
            sIdx = i % length;
            eIdx = sizegrid - (length - (i % length)) + 1;
            cluecounter = 0;
            for (int j = sIdx; j < eIdx; j += length)
            {
                /* excluding mask = 0 */
                tmpByte = 0;
                for (int k = 0; k < mask_cell_len; k++)
                    tmpByte |= mask[j * mask_cell_len + k];
                if (tmpByte == 0)
                    continue;
                    
                tmpBool = true;
                for (int k = 0; k < mask_cell_len && tmpBool; k++)
                    if ((clueMask[k] | mask[j * mask_cell_len + k]) !=
                        clueMask[k])
                        tmpBool = false;
                if (tmpBool)
                    cluecounter++;
                    
                if (cluecounter >= ie)
                {
                    /* scan the column and eliminate non-clues */
                    for (int k = sIdx; k < eIdx; k += length)
                    {
                        tmpByte = 0;
                        for (int l = 0; l < mask_cell_len; l++)
                            tmpByte |= mask[k * mask_cell_len + l];
                        if (tmpByte == 0)
                            continue;

                        tmpBool = true;
                        for (int l = 0; l < mask_cell_len && tmpBool; l++)
                            if ((clueMask[l] | mask[k * mask_cell_len + l]) !=
                                clueMask[l])
                                tmpBool = false;
                            
                        /* found non-clue*/
                        if (!tmpBool)
                        {   
                            /* they must have intersection */
                            tmpByte = 0;
                            for (int l = 0; l < mask_cell_len; l++)
                                tmpByte |= (clueMask[l] & mask[k * mask_cell_len + l]);
                            if (tmpByte != 0)
                            {
                                isAnyUpdate = true;
                                for (int l = 0; l < mask_cell_len; l++)
                                    mask[k * mask_cell_len + l] &=
                                        (~clueMask[l]);
                            }
                        }
                    }
                    break;
                }
            }

            /* block scan */
            unsigned int block_y = (i / length) / blocklength + 1;
            unsigned int block_x = (i % length) / blocklength + 1;
            cluecounter = 0;
            for (int y = (block_y - 1) * blocklength;
                y < block_y * blocklength; y++)
                for (int x = (block_x - 1) * blocklength;
                    x < block_x * blocklength; x++)
                {
                    /* excluding mask = 0 */
                    tmpByte = 0;
                    for (int k = 0; k < mask_cell_len; k++)
                        tmpByte |= mask[y * length * mask_cell_len +
                            x * mask_cell_len + k];
                    if (tmpByte == 0)
                        continue;

                    tmpBool = true;
                    for (int k = 0; k < mask_cell_len && tmpBool; k++)
                        if ((clueMask[k] | mask[y * length * mask_cell_len +
                            x * mask_cell_len + k]) != clueMask[k])
                            tmpBool = false;
                    if (tmpBool)
                        cluecounter++;
                }
            if (cluecounter >= ie)
            {
                /* scan the block and eliminate non-clues */
                for (int y = (block_y - 1) * blocklength;
                    y < block_y * blocklength; y++)
                    for (int x = (block_x - 1) * blocklength;
                        x < block_x * blocklength; x++)
                    {
                        tmpByte = 0;
                        for (int l = 0; l < mask_cell_len; l++)
                            tmpByte |= mask[y * length * mask_cell_len +
                                x * mask_cell_len + l];
                        if (tmpByte == 0)
                            continue;

                        tmpBool = true;
                        for (int l = 0; l < mask_cell_len && tmpBool; l++)
                            if ((clueMask[l] | mask[y * length * mask_cell_len +
                                x * mask_cell_len + l]) != clueMask[l])
                                tmpBool = false;
                        /* found non-clue*/
                        if (!tmpBool)
                        {   
                            /* they must have intersection */
                            tmpByte = 0;
                            for (int l = 0; l < mask_cell_len; l++)
                                tmpByte |= (clueMask[l] & mask[y * length * mask_cell_len +
                                x * mask_cell_len + l]);
                            if (tmpByte != 0)
                            {
                                isAnyUpdate = true;
                                for (int l = 0; l < mask_cell_len; l++)
                                    mask[y * length * mask_cell_len +
                                        x * mask_cell_len + l] &= (~clueMask[l]);
                            }
                        }
                    }
            }
        }
        return isAnyUpdate;
    }
}