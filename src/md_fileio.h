//=====================================================
// md_fileio.h
//
// Lilith M-Code Disassembler
//
// Guido Hoss, 14.03.2022
//
// Published by Guido Hoss under GNU Public License V3.
//=====================================================

#ifndef _MD_FILEIO_H
#define _MD_FILEIO_H   1

#include "mdecode.h"

void md_skip(FILE *infd, uint16_t n);
uint16_t md_rword(FILE *infd);
uint8_t md_rbyte(FILE *infd);

#endif