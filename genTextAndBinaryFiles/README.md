The files in this folder each generate text- and binary-formatted files, but for different data types:
- genChars.cpp generates char data
- genDoubles.cpp generates double data
- genInts.cpp generates int data

The provided `Makefile` should build all three programs. Once built, a command such as:
    ./genDoubles 1000000 1Mdoubles
will generate two files:
- 1Mdoubles.bin, containing 1 million double values in binary format
- 1Mdoubles.txt, containing 1 million double values in text format

These three programs are basically the same program, with a typedef being used to set ItemType to the desired data type.
If a different data type is desired, use these as a model.
