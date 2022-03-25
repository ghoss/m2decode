//=====================================================
// md_usage.c
// Help description
//
// Lilith M-Code Disassembler
//
// Guido Hoss, 14.03.2022
//
// Published by Guido Hoss under GNU Public License V3.
//=====================================================

#include "md_usage.h"


// md_version()
// Show version information
//
void md_version()
{
    fprintf(stderr, 
    	PACKAGE " v" PACKAGE_VERSION 
		" by " PACKAGE_BUGREPORT" (" VERSION_BUILD_DATE ")\n"
		"Modula-2 M-Code Disassembler\n\n"
	);
}


// md_usage()
// Show usage information
//
void md_usage()
{
    fprintf(stderr,
        "USAGE: " PKG " [-hV] [-o output_file] [object_file]\n\n"
        "-h\tShow this help information\n"
        "-V\tShow version information\n\n"
        "-o\tDecode to output_file (default: STDOUT)\n\n"
        "object_file is the filename of a Lilith M-Code (OBJ) file.\n"
        "(Reads from standard input if no filename given.)\n\n"
    );
}