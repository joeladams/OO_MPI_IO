The files in this folder each generate text- and binary-formatted files, but for different data types:
- `genChars.cpp` generates char data
- `genDoubles.cpp` generates double data
- `genInts.cpp` generates int data

The provided `Makefile` should build all three programs. Once built, a command such as:

    ./genDoubles 1000000 1M_doubles

will generate two files:
- `1M_doubles.bin`, a binary file containing 1 million binary-formatted double values
- `1M_doubles.txt`, a text file containing 1000000 on its first line, followed by 1 million text-formatted double values, each on its own line.

These three programs are basically the same program, with a typedef being used to set ItemType to the desired data type, and format-strings tweaked as needed for that data type.

If a different data type is desired, use these as a model.
