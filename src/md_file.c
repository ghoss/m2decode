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
    uint16_t w, n, a;
    uint16_t vers;
    bool proc_section = true;
    bool eof = false;

    // Parse all sections
    while (! eof) {

        // Read next word
        w = md_rword(infd);
        eof = feof(infd);

        switch (w)
        {
        case 0200 :
            // Start of file
            md_expect(infd, 1);
            md_rword(infd);
            break;
        
        case 0201 :
            // Module section
            vers = md_rword(infd);
            fprintf(ofd, "MODULE ");
            md_rname(infd, ofd);

            // Skip bytes following filename in later versions
            if (vers == 0x11)
                md_skip(infd, 6);

            w = md_rword(infd);
            fprintf(ofd, "\n  DataSize: %07o (%d bytes)", w, w);
            w = md_rword(infd);
            fprintf(ofd, "\n  CodeSize: %07o (%d bytes)\n", w, w);
            md_rword(infd);
            break;

        case 0202 :
            // Import section
            fprintf(ofd, "IMPORTS\n");
            n = md_rword(infd);
            a = 1;
            while (n > 0)
            {
                fprintf(ofd, "  %03o: ", a);
                md_rname(infd, ofd);
                n -= 11;
                a++;
            }
            break;

        case 0204 :
            // Data sections
            fprintf(ofd, "DATA [G]\n");
            n = md_rword(infd);
            a = md_rword(infd);
            while (n-- > 1)
            {
                fprintf(ofd, "%7d: %07o\n", a++, md_rword(infd));
            }
            break;

        case 0203 :
            if (proc_section)
            {
                // Procedure entry point section
                n = md_rword(infd);
                a = 0;
                w = md_rword(infd);

                fprintf(ofd, "PROCEDURE #%03o\n", w);
                while (n-- > 1)
                {
                    fprintf(ofd, "%7d: %07o\n", a, md_rword(infd));
                    a ++;
                }
            }
            else {
                n = md_rword(infd);
                a = md_rword(infd);
                n = (a + n - 1) << 1;
                a <<= 1;
                fprintf(ofd, "CODE [F]\n");

                while (a < n)
                {
                    uint16_t ofs = md_opcode(infd, ofd, a, md_rbyte(infd));
                    a += ofs;
                }
            }
            proc_section = ! proc_section;
            break;

        case 0205 :
            // Relocation section
            fprintf(ofd, "FIXUPS\n");
            n = md_rword(infd);

            while (n-- > 0)
            {
                w = md_rword(infd);
                fprintf(ofd, "  %07o\n", w);
            }
            break;

        default :
            // No more readable sections
            eof = true;
            break;
        }

        // End of section
        if (! eof)
            fprintf(ofd, "\n");
    };
}
