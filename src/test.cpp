#include "CSVreader.h"
#include "element.h"
#include "FileHandler.h"

#include <cstdio>
#include <string>

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

/* test for file handler */
void test2()
{
    std::printf("start test2...\r\n");
    std::string content =
        sds::readText("bin/example/001.csv");
    
    std::printf("%s\r\n", content.c_str());
    /* test Lexer */
    char gridMatrix[81];
    char tmpChar;
    sds::CSVLexer csvlexer(content);
    csvlexer.resetIdx();
    int idx = 0;
    int contentIdx = 0;

    while (idx < 81)
    {
        tmpChar = csvlexer.getCSVTok();
        if (tmpChar != sds::CSVTokType::tok_empty &&
            tmpChar != sds::CSVTokType::tok_invalid)
        {
            gridMatrix[idx] = tmpChar;
            idx++;
        }
        contentIdx++;
        if (contentIdx > content.size())
        {
            std::fprintf(stderr, "no enough digits!\r\n");
            break;
        }
    }

    std::printf("print csv matrix:\r\n");
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
            std::printf("%d\t", gridMatrix[i * 9 + j]);
        std::printf("\r\n");
    }
}

/* test for csv reader */
void test3()
{
    sds::Grid* grid = sds::CSVtoGrid("bin/example/001.csv");
    if (grid == nullptr)
        return;
    std::printf("print csv matrix:\r\n");
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
            std::printf("%d\t", (*grid)(i, j));
        std::printf("\r\n");
    }
    std::printf("grid length = %d\r\n", grid->Length());
    /* test update mask */
    grid->updateMask();
    std::printf("print digit mask:\r\n");
    for (char digit = 1; digit <= grid->Length(); digit++)
    {
        std::printf("digit %d mask:\r\n", digit);
        for (int i = 0; i < grid->Length(); i++)
        {
            for (int j = 0; j < grid->Length(); j++)
                std::printf("%d\t", (*grid)(i, j, digit));
            std::printf("\r\n");
        }
    }
    
}