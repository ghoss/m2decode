//=====================================================
// mdecode.h
// General definitions
//
// Lilith M-Code Disassembler
//
// Guido Hoss, 14.03.2022
//
// Published by Guido Hoss under GNU Public License V3.
//=====================================================

#ifndef _MDECODE_H
#define _MDECODE_H   1

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <stdbool.h>

// Import table
#define IMPORT_LEN	99		// Max. entries in import table
#define MODNAME_LEN	17		// Max. length of module names + 1

extern char import[IMPORT_LEN][MODNAME_LEN];
extern uint16_t import_n;

// Output shorthand
#define OUT(...)		fprintf(ofd, __VA_ARGS__);

#endif