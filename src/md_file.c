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
    uint32_t total_code = 0;	// effective code size
    uint32_t decl_code = 0;		// declared code size
    uint32_t total_data = 0;	// effective data size
	uint32_t decl_data = 0;		// declared data size
    bool proc_section = true;
    bool eof = false;

    // Parse all sections
    while (! eof) {

        // Read next word
        w = md_rword(infd);
        eof = feof(infd);

        switch (w)
        {
        case 0xC1 :
            // Alternate start of file, sometimes encountered
            md_rword(infd);
            break;

        case 0200 : {
            // Start of file
            md_expect(infd, 1);
            vers = md_rword(infd);
            fprintf(ofd, "HEADER v.%d", vers);
            break;
        }
        
        case 0201 : {
            // Module section
            n = md_rword(infd);
            fprintf(ofd, "MODULE (%d bytes): ", n << 1);
            md_rname(infd, ofd);

            // Skip bytes following filename in later versions
            if (n == 0x11)
                md_skip(infd, 6);

            uint16_t nd = md_rword(infd) << 1;
            uint16_t nc = md_rword(infd) << 1;
            decl_data += nd;
            decl_code += nc;

            fprintf(ofd, 
                "\n  DataSize: %6d bytes\n"
                "  CodeSize: %6d bytes\n", 
                nd, nc
            );
            md_rword(infd);
            break;
        }

        case 0202 :
            // Import section
            fprintf(ofd, "IMPORTS\n");
            n = md_rword(infd);
            a = 1;
            while (n > 0)
            {
                fprintf(ofd, "%4d: ", a);
                md_rname(infd, ofd);
                n -= 11;
                a++;
            }
            break;

        case 0204 : {
                // Data sections
                n = md_rword(infd);
                a = md_rword(infd);
                n --;
                fprintf(ofd, "DATA (%d bytes)\n", n << 1);
				total_data += n << 1;

                uint16_t num = 0;
                while (n-- > 0)
                {
                    if (num % 8 == 0)
                        fprintf(ofd, "\n  %07o", a);
                    a ++;
                    num ++;

                    w = md_rword(infd);
                    fprintf(ofd, "  %04x", w);
                }
                fprintf(ofd, "\n");
                break;
            }

        case 0203 :
            if (proc_section)
            {
                // Procedure entry point section
                n = md_rword(infd);
                a = 0;
                w = md_rword(infd);

                fprintf(ofd, "PROCEDURE #%03o", w);
                if (n > 2)
                {
                    uint16_t extra = (n - 2) * 2;
                    fprintf(ofd, 
                        "  (%d bytes for extra entries)", extra
                    );
                    total_code += (n - 2) * 2;
                }
                fprintf(ofd, "\n");

                while (n-- > 1)
                {
                    fprintf(ofd, "%7d: %07o\n", a, md_rword(infd));
                    a ++;
                }
            }
            else {
                n = md_rword(infd) << 1;
                a = md_rword(infd) << 1;
                fprintf(ofd, "CODE (%d bytes)\n", n);
                total_code += n;
                n = a + n - 2;

                while (a < n)
                {
                    uint16_t ofs = md_opcode(infd, ofd, a, md_rbyte(infd));
                    a += ofs;
                };
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

    // Final stats
    fprintf(ofd,
        "STATS\n" 
        "  CodeSize: %6d / %6d\n"
		"  DataSize: %6d / %6d\n"
		"  (effective / declared bytes)\n",
        total_code, decl_code,
		total_data, decl_data
    );
}
