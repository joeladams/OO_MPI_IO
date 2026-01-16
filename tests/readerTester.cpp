/* readerTester.cpp tests the ParallelReader template.
 *
 * Joel C. Adams, Calvin University, Fall 2023.
 *
 * Usage: mpirun -np <P> ./readerTester
 *         where P=1, 2, or 3.
 */

#include "DoubleReaderTester.h"
#include "IntReaderTester.h"
#include "CharReaderTester.h"

int main(int argc, char** argv) {
   MPI_Init(&argc, &argv);

   DoubleReaderTester drt;
   drt.runTests();

   IntReaderTester irt;
   irt.runTests();

   CharReaderTester crt;
   crt.runTests();

   MPI_Finalize();
}

