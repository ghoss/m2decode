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
void md_rname(FILE *infd, char *modname)
{
    for (int i = 0; i < MODNAME_LEN - 1; i ++)
    {
        *(modname + i) = md_rbyte(infd);
    }
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
            OUT("HEADER v.%d", vers)
            break;
        }
        
        case 0201 : {
            // Module section
            n = md_rword(infd);
			char modname[MODNAME_LEN];
            md_rname(infd, &modname[0]);
            OUT("MODULE %s (%d bytes),", &modname, n << 1)

			OUT(" key = ")
			for (int i = 0; i < 3; i ++)
				OUT("%04X", md_rword(infd))
			OUT("\n")

            // Skip bytes following filename in later versions
            if (n == 0x11)
                md_skip(infd, 6);

            uint16_t nd = md_rword(infd) << 1;
            uint16_t nc = md_rword(infd) << 1;
            decl_data += nd;
            decl_code += nc;

            OUT(
                "\n  DataSize: %6d bytes\n"
                "  CodeSize: %6d bytes\n", 
                nd, nc
            )
            md_rword(infd);
            break;
        }

        case 0202 :
            // Import section
            OUT("IMPORTS\n")
            n = md_rword(infd);
			import_n = n;
            a = 1;
            while (n > 0)
            {
                md_rname(infd, &(import[a][0]));
                OUT("%4d: %-16s", a, &(import[a][0]))
				OUT("  (")
				for (int i = 0; i < 3; i ++)
					OUT("%04X", md_rword(infd));
				OUT(")\n")

                n -= 11;
                a++;
            }
            break;

        case 0204 : {
                // Data sections
                n = md_rword(infd);
                a = md_rword(infd);
                n --;
                OUT("DATA (%d bytes)\n", n << 1)

                uint16_t num = 0;
                while (n-- > 0)
                {
                    if (num % 8 == 0)
                        OUT("\n  %07o", a)
                    a ++;
                    num ++;

                    w = md_rword(infd);
                    OUT("  %04x", w)
                }
                OUT("\n")
                break;
            }

        case 0203 :
            if (proc_section)
            {
                // Procedure entry point section
                n = md_rword(infd);
                a = 0;
                w = md_rword(infd);

                OUT("PROCEDURE #%03o", w)
                if (n > 2)
                {
                    uint16_t extra = (n - 2) * 2;
                    OUT("  (%d bytes for extra entries)", extra)
                    total_code += (n - 2) * 2;
                }
                OUT("\n")

                while (n-- > 1)
                {
                    OUT("%7d: %07o\n", a, md_rword(infd))
                    a ++;
                }
            }
            else {
                n = md_rword(infd) << 1;
                a = md_rword(infd) << 1;
                OUT("CODE (%d bytes)\n", n)
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
            OUT("FIXUPS\n")
            n = md_rword(infd);

            while (n-- > 0)
            {
                w = md_rword(infd);
                OUT("  %07o\n", w)
            }
            break;

        default :
            // No more readable sections
            eof = true;
            break;
        }

        // End of section
        if (! eof)
            OUT("\n")
    };

    // Final stats
    OUT(
        "STATS\n" 
        "  CodeSize: %6d / %6d\n"
		"  (effective / declared bytes)\n",
        total_code, decl_code
    );
}
