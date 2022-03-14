//=====================================================
// md_file.c
//
// Lilith M-Code Disassembler
//
// Guido Hoss, 14.03.2022
//
// Published by Guido Hoss under GNU Public License V3.
//=====================================================

#include "md_fileio.h"
#include "md_opc.h"
#include "md_file.h"


// md_rname()
// Read a name string from input
//
void md_rname(FILE *infd, FILE *ofd)
{
    fprintf(ofd, "  ");
    for (int i = 0; i < 16; i ++)
    {
        char c = md_rbyte(infd);
        fprintf(ofd, "%c", (c < ' ') ? ' ' : c);
    }

    fprintf(ofd, "  (");
    for (int i = 0; i < 3; i ++)
        fprintf(ofd, " %04X", md_rword(infd));

    fprintf(ofd, " )\n");
}


// md_expect()
// Expect a word from the input file and stop if no match
//
void md_expect(FILE *infd, uint16_t w)
{
    uint16_t wr = md_rword(infd);

    if (wr != w)
        error(1, 0, "Input stream error: expected byte %d, got %d", w, wr);
}


// md_decode_file()
// Decode the specified input file infd and write
// output to ofd
//
void md_decode_file(FILE *infd, FILE *ofd)
{
    uint16_t w;
    uint16_t vers;

    md_expect(infd, 0200);
    md_expect(infd, 1);
    w = md_rword(infd);

    // Header section
    md_expect(infd, 0201);
    vers = md_rword(infd);
    fprintf(ofd, "HEADER\n");
    md_rname(infd, ofd);

    // Skip bytes following filename in later versions
    if (vers == 0x11)
        md_skip(infd, 6);

    w = md_rword(infd);
    fprintf(ofd, "\n  DataSize: %07o (%d)", w, w);
    w = md_rword(infd);
    fprintf(ofd, "\n  CodeSize: %07o (%d)\n\n", w, w);
    md_rword(infd);

    // Import section
    w = md_rword(infd);
    if (w == 0202)
    {
        fprintf(ofd, "IMPORTS\n");
        uint16_t n = md_rword(infd);
        while (n > 0)
        {
            md_rname(infd, ofd);
            n -= 11;
        }
        w = md_rword(infd);
        fprintf(ofd, "\n");
    }

    // Data sections
    while (w == 0204)
    {
        fprintf(ofd, "DATA\n");
        uint16_t n = md_rword(infd);
        uint16_t a = md_rword(infd);
        while (n-- > 1)
        {
            fprintf(ofd, "%7d: %07o\n", a++, md_rword(infd));
        }
        w = md_rword(infd);
        fprintf(ofd, "\n");
    }

    // Entries section
    if (w == 0203)
    {
        fprintf(ofd, "PROCEDURES\n");
        uint16_t n = md_rword(infd);
        uint16_t a = 0;

        md_rword(infd);
        // md_expect(infd, 0);
        while (n-- > 1)
        {
            fprintf(ofd, "%7d: %07o\n", a, md_rword(infd));
            a ++;
        }
        w = md_rword(infd);
        fprintf(ofd, "\n");
    }

    // Code section
    if (w == 0203)
    {
        fprintf(ofd, "CODE\n");
        uint16_t n = md_rword(infd);
        uint16_t a = md_rword(infd);
        
        n = (a + n - 1) << 1;
        a <<= 1;
        while (a < n)
        {
            fprintf(ofd, "  %07o  ", a);
            a += md_opcode(infd, ofd, md_rbyte(infd));
        }
        w = md_rword(infd);
        fprintf(ofd, "\n");
    }

    // Relocation section
    if (w == 0205)
    {
        fprintf(ofd, "RELOCATION\n");
        uint16_t n = md_rword(infd);

        while (n-- > 0)
        {
            uint16_t w = md_rword(infd);
            fprintf(ofd, "  %07o\n", w);
        }
        fprintf(ofd, "\n");
    }
}