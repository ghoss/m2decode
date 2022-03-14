//=====================================================
// md_fileio.h
//
// Lilith M-Code Disassembler
//
// Guido Hoss, 14.03.2022
//
// Published by Guido Hoss under GNU Public License V3.
//=====================================================

#ifndef _MDFILEIO_H
#define _MDFILEIO_H   1

#include "mdecode.h"

void md_skip(FILE *infd, uint16_t n);
uint16_t md_rword(FILE *infd);
uint8_t md_rbyte(FILE *infd);

#endif