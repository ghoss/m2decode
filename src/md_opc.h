//=====================================================
// md_opc.h
//
// Lilith M-Code Disassembler
//
// Guido Hoss, 14.03.2022
//
// Published by Guido Hoss under GNU Public License V3.
//=====================================================

#ifndef _MD_OPC_H
#define _MD_OPC_H   1

#include "mdecode.h"

uint16_t md_opcode(FILE *infd, FILE *ofd, uint16_t pc, uint8_t mcode);

#endif