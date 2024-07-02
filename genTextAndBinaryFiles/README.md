The files in this folder each generate text- and binary-formatted files, but for different data types:
- genChars.cpp generates char data
- genDoubles.cpp generates double data
- genInts.cpp generates int data
They are basically the same program; a typedef is used to define ItemType as the desired data type.
These can be used as a model for generating text and binary data files for other data types.
The provided `Makefile` should build all three...
