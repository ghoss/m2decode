//=====================================================
// md_fileio.c
//
// Lilith M-Code Disassembler
//
// Guido Hoss, 14.03.2022
//
// Published by Guido Hoss under GNU Public License V3.
//=====================================================

#include "md_fileio.h"


// md_skip()
// Skip a number of bytes in input stream
//
void md_skip(FILE *infd, uint16_t n)
{
    fseek(infd, n, SEEK_CUR);
}


// md_rword()
// Read a 16-bit word from input
//
uint16_t md_rword(FILE *infd)
{
    uint16_t wr;

    if (fread(&wr, sizeof(uint16_t), 1, infd) != 1)
        error(1, 1, "Could not read from input file");

    // Swap byte order
    return ((wr >> 8) | (wr << 8));
}


// md_rbyte()
// Read a byte from input
//
uint8_t md_rbyte(FILE *infd)
{
    uint16_t b;

    if (fread(&b, 1, 1, infd) != 1)
        error(1, 1, "Could not read from input file");

    return b;
}