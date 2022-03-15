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
	"LI0  LI1  LI2  LI3  LI4  LI5  LI6  LI7  LI8  LI9  LI10 LI11 LI12 LI13 LI14 LI15 "
	"LIB +---- LIW *LID /LLA +LGA +LSA +LEA *JPC *JP  *JPFC+JPF +JPBC+JPB +ORJP+AJP +"
	"LLW +LLD +LEW -LED -LLW4 LLW5 LLW6 LLW7 LLW8 LLW9 LLW10LLW11LLW12LLW13LLW14LLW15"
	"SLW +SLD +SEW -SED -SLW4 SLW5 SLW6 SLW7 SLW8 SLW9 SLW10SLW11SLW12SLW13SLW14SLW15"
	"LGW +LGD +LGW2 LGW3 LGW4 LGW5 LGW6 LGW7 LGW8 LGW9 LGW10LGW11LGW12LGW13LGW14LGW15"
	"SGW +SGD +SGW2 SGW3 SGW4 SGW5 SGW6 SGW7 SGW8 SGW9 SGW10SGW11SGW12SGW13SGW14SGW15"
	"LSW0 LSW1 LSW2 LSW3 LSW4 LSW5 LSW6 LSW7 LSW8 LSW9 LSW10LSW11LSW12LSW13LSW14LSW15"
	"SSW0 SSW1 SSW2 SSW3 SSW4 SSW5 SSW6 SSW7 SSW8 SSW9 SSW10SSW11SSW12SSW13SSW14SSW15"
	"LSW +LSD +LSD0 LXFW LSTA+LXB  LXW  LXD  DADD DSUB DMUL DDIV ---- ---- DSHL DSHR "
	"SSW +SSD +SSD0 SXFW TS   SXB  SXW  SXD  FADD FSUB FMUL FDIV FCMP FABS FNEG FFCT+"
	"READ WRT  DSKR DSKW TRK  UCHK SVC +SYS +ENTP+EXP  ULSS ULEQ UGTR UGEQ TRA +RDS +"
	"LODFWLODFDSTOR STOFVSTOT COPT DECS PCOP+UADD USUB UMUL UDIV UMOD ROR  SHL  SHR  "
	"FOR1?FOR2?ENTC*EXC  TRAP CHK  CHKZ CHKS EQL  NEQ  LSS  LEQ  GTR  GEQ  ABS  NEG  "
	"OR   XOR  AND  COM  IN   LIN  MSK  NOT  IADD ISUB IMUL IDIV IMOD BIT  NOP  MOVF "
	"MOV  CMP  DDT  REPL BBLT DCH  UNPK PACK GB  +GB1  ALLOCENTR+RTN  CLX -CLI +CLF  "
	"CLL +CLL1 CLL2 CLL3 CLL4 CLL5 CLL6 CLL7 CLL8 CLL9 CLL10CLL11CLL12CLL13CLL14CLL15";


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

    uint16_t i = w * 5;
    char c = mnem[i + 4];
    fprintf(ofd, " %c%c%c%c", mnem[i], mnem[i+1], mnem[i+2], mnem[i+3]);

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
            fprintf(ofd, "%c", c);
            break;
    }
    fprintf(ofd, "\n");
    return ofs;
}