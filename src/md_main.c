//=====================================================
// md_main.c
// Main program
//
// Lilith M-Code Disassembler
//
// Guido Hoss, 14.03.2022
//
// Published by Guido Hoss under GNU Public License V3.
//=====================================================

#include "md_usage.h"
#include "md_file.h"


// main()
// Main function
//
void main(int argc, char *argv[])
{
	char c;
	char *in_file = NULL;
	FILE *in_fd = NULL;
	char *out_file = NULL;
	FILE *out_fd;

	// Parse command line options
	opterr = 0;
	while ((c = getopt (argc, argv, "Vho:")) != -1)
	{
		switch (c)
		{
		case 'h' :
			// Help information
			md_usage();
			exit(0);

		case 'V' :
			// Version information
			md_version();
			exit(0);
			
		case 'o' :
			// Output filename
			out_file = optarg;
			break;

		case '?' :
			error(1, 0,
				"Unrecognized option (run \"" PKG " -h\" for help)."
			);
			break;

		default :
			break;
        }
    }

	// Get input file
	if (optind < argc)
	{
		in_file = argv[optind];
		if (! (in_fd = fopen(in_file, "r")))
			error(1, errno, "Can't open image file '%s'", in_file);
	}
	else
	{
		// No filename given; read from standard input
		in_fd = stdin;
	}

	// Determine output file
	if ((out_file != NULL) && (*out_file != '\0'))
	{
		if (! (out_fd = fopen(out_file, "w")))
			error(1, errno, "Can't create output file '%s'", out_file);
	}
	else
	{
		// Output to stdout by default
		out_fd = stdout;
	}

	// Start disassembly
	md_decode_file(in_fd, out_fd);

	// Close input and output files
	fclose(in_fd);
	fclose(out_fd);
}