#=====================================================
# mdecode
# Lilith M-Code Disassembler
#
# Guido Hoss, 14.03.2022
#
# Published by Guido Hoss under GNU Public License V3.
#=====================================================

CFLAGS += -DVERSION_BUILD_DATE=\""$(shell date +'%F')"\"

bin_PROGRAMS = mdec

mdec_SOURCES = \
	md_main.c \
	md_usage.c md_usage.h \
	md_file.c md_file.h \
	md_fileio.c md_fileio.h \
	md_opc.c md_opc.h \
	mdecode.h