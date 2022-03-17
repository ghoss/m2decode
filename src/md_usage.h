//=====================================================
// md_usage.h
// Help description
//
// Lilith M-Code Disassembler
//
// Guido Hoss, 14.03.2022
//
// Published by Guido Hoss under GNU Public License V3.
//=====================================================

#ifndef _MD_USAGE_H
#define _MD_USAGE_H   1

#include "mdecode.h"

#ifndef VERSION_BUILD_DATE
#define VERSION_BUILD_DATE "?"
#endif
#define PKG "mdec"


void md_usage();
void md_version();

#endif