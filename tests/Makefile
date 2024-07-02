PROG1  = readerTester
PROG2  = writerTester
SRC1   = $(PROG1).cpp
SRC2   = $(PROG2).cpp
INCL1  = ../OO_MPI_IO.h \
          DoubleReaderTester.h \
          IntReaderTester.h \
          CharReaderTester.h
INCL2  = ../OO_MPI_IO.h \
          DoubleWriterTester.h \
          CharWriterTester.h

CC     = mpic++
CFLAGS = -Wall -ansi -pedantic -std=c++11 
LFLAGS1 = -o $(PROG1) 
LFLAGS2 = -o $(PROG2) 

all: $(PROG1) $(PROG2)

$(PROG1): $(SRC1) $(INCL1)
	$(CC) $(CFLAGS) $(SRC1) $(LFLAGS1)

$(PROG2): $(SRC2) $(INCL2)
	$(CC) $(CFLAGS) $(SRC2) $(LFLAGS2)

clean:
	rm -f $(PROG1) $(PROG2) a.out *~ *# *.o

