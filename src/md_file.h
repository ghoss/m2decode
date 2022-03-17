//=====================================================
// md_file.h
//
// Lilith M-Code Disassembler
//
// Guido Hoss, 14.03.2022
//
// Published by Guido Hoss under GNU Public License V3.
//=====================================================

#ifndef _MD_FILE_H
#define _MD_FILE_H   1

#include "mdecode.h"

void md_decode_file(FILE *infd, FILE *outfd);

#endif