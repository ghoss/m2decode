//=====================================================
// md_opc.h
//
// Lilith M-Code Disassembler
//
// Guido Hoss, 14.03.2022
//
// Published by Guido Hoss under GNU Public License V3.
//=====================================================

#ifndef _MDOPC_H
#define _MDOPC_H   1

#include "mdecode.h"

uint8_t md_opcode(FILE *infd, FILE *ofd, uint8_t w);

#endif