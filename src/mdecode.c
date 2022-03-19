//=====================================================
// mdecode.c
// Main program
//
// Lilith M-Code Disassembler
//
// Guido Hoss, 14.03.2022
//
// Published by Guido Hoss under GNU Public License V3.
//=====================================================

#include "mdecode.h"


// Import table
char import[IMPORT_LEN][MODNAME_LEN];
uint16_t import_n;