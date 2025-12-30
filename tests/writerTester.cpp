/* readerTester.cpp tests the ParallelReader template.
 *
 * Joel C. Adams, Calvin University, Fall 2023.
 *
 */

#include "DoubleWriterTester.h"
// #include "IntReaderTester.h"
#include "CharWriterTester.h"

int main(int argc, char** argv) {
   MPI_Init(&argc, &argv);

   DoubleWriterTester dwt;
   dwt.runTests();
/*
   IntWriterTester iwt;
   iwt.runTests();
   CharWriterTester cwt;
   cwt.runTests();
*/

   MPI_Finalize();
}

