//=====================================================
// md_opc.c
// M-Code opcode output
//
// Lilith M-Code Disassembler
//
// Guido Hoss, 14.03.2022
//
// Published by Guido Hoss under GNU Public License V3.
//=====================================================

#include "md_fileio.h"
#include "md_opc.h"


// M-Code mnemonics table
//
char *mnem = 
	"LI0 LI1 LI2 LI3 LI4 LI5 LI6 LI7 LI8 LI9 LI10LI11LI12LI13LI14LI15"
	"LIB+--- LIW*LID/LLA+LGA+LSA+LEA*JPC*JP *JFC+JPF+JBC+JPB+ORJ+AJP+"
	"LLW+LLD+LEW-LED-LLW4LLW5LLW6LLW7LLW8LLW9LLWaLLWbLLWcLLWdLLWeLLWf"
	"SLW+SLD+SEW-SED-SLW4SLW5SLW6SLW7SLW8SLW9SLWaSLWbSLWcSLWdSLWeSLWf"
	"LGW+LGD+LGW2LGW3LGW4LGW5LGW6LGW7LGW8LGW9LGWaLGWbLGWcLGWdLGWeLGWf"
	"SGW+SGD+SGW2SGW3SGW4SGW5SGW6SGW7SGW8SGW9SGWaSGWbSGWcSGWdSGWeSGWf"
	"LSW0LSW1LSW2LSW3LSW4LSW5LSW6LSW7LSW8LSW9LSWaLSWbLSWcLSWdLSWeLSWf"
	"SSW0SSW1SSW2SSW3SSW4SSW5SSW6SSW7SSW8SSW9SSWaSSWbSSWcSSWdSSWeSSWf"
	"LSW+LSD+LSD0LXFWLST+LXB LXW LXD DADDDSUBDMULDDIV--- --- DSHLDSHR"
	"SSW+SSD+SSD0SXFWTS  SXB SXW SXD FADDFSUBFMULFDIVFCMPFABSFNEGFCT+"
	"READWRT DSKRDSKWSTRKUCHKHOOKSYS+ENP+EXP ULSSULEQUGTRUGEQTRA+RDS+"
	"LDFWLDFDSTORSTFVSTOTCOPTDECSPCP+UADDUSUBUMULUDIVUMODROR SHL SHR "
	"FR1?FR2?ENC*EXC TRAPCHK CHKZCHKSEQL NEQ ILSSILEQIGTRIGEQABS NEG "
	"OR  XOR AND COM IN  LIN MSK NOT IADDISUBIMULIDIV--- BIT NOP MOVF"
	"MOV CMP DDT REPLBBLTDCH UNPKPACKGB +GB1 ALOCENT+RTN CLX-CLI+CLF "
	"CLL+CLL1CLL2CLL3CLL4CLL5CLL6CLL7CLL8CLL9CLLaCLLbCLLcCLLdCLLeCLLf";


// md_opcode()
// Print an opcode and its arguments
// Return the number of bytes consumed by the opcode
//
uint8_t md_opcode(FILE *infd, FILE *ofd, uint8_t w)
{
    uint8_t ofs = 1;

    void pr_byte()
    {
        fprintf(ofd, " %03o", md_rbyte(infd));
        ofs += 1;
    }

    void pr_word()
    {
        fprintf(ofd, " %06o", md_rword(infd));
        ofs += 2;
    }

    uint16_t i = w << 2;
    char c = mnem[i + 3];
    fprintf(ofd, " %c%c%c", mnem[i], mnem[i+1], mnem[i+2]);

    switch (c)
    {
        case '+' :
            pr_byte();
            break;

        case '-' :
            pr_byte();
            pr_byte();
            break;

        case '*' :
            pr_word();
            break;

        case '/' :
            pr_word();
            pr_word();
            break;

        case '?' :
            pr_byte();
            pr_word();
            break;

        default :
            if (c >= 'a')
                fprintf(ofd, "%d", (c - 'a') + 10);
            else
                fprintf(ofd, "%c", c);
            break;
    }
    fprintf(ofd, "\n");
    return ofs;
}