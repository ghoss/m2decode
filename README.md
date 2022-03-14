# Lilith Modula-2 M-Code Disassembler
This is a disassembler (written in C) to decode Lilith M-Code object (.OBJ) files.

It is derived from Niklaus Wirth's original decoder which he wrote in 1984 for the Modula-2 Single Pass Compiler. The source code for this compiler, along with the original Modula-2 sources for the decoder, was discovered in 2021.

The 1984 version of the decoder is somewhat less complex than an earlier version written by Ch. Jacobi in 1981 for the Modula-2 Multipass Compiler.

## Caveat
This code is still work in progress. It will already run on many object files, but will not produce the correct output for most of them yet.

## Compiling And Installation
1. Download the .tar.gz packages from the "[Releases](https://github.com/good-sushi/mdecode/releases)" page.
2. Extract and build:
    ```
    $ tar xzf mdecode-x.y.tar.gz
    $ cd mdecode-x.y
    $ ./configure
    $ make && make install
    ```

## Usage
```
USAGE: mdec [-hV] [-o output_file] [object_file]

-h	Show this help information
-V	Show version information

-o	Decode to output_file (default: STDOUT)

object_file is the filename of a Lilith M-Code (OBJ) file.
(Reads from standard input if no filename given.)
```

## Examples
* ```mdec test.OBJ```

  Decodes the object file **test.OBJ** and writes the disassembled result to standard output.

* ```mdec -o test.txt test.OBJ```

  As above, but writes the output to the file **test.txt** instead.