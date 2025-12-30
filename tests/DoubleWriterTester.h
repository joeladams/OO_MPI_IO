/* DoubleWriterTester.h declares the class that tests ParallelWriter
 *   using double values.
 *
 * @author: Joel C. Adams, Calvin University, Fall 2023
 */

#include <iostream>                // cout, ...
#include <fstream>                 // ifstream, ofstream, fstream
#include <mpi.h>                   // MPI types
#include <cassert>                 // assert()
#include "../OO_MPI_IO.h"          // ParallelReader, ParallelWriter
using namespace std;

class DoubleWriterTester {
public:
  DoubleWriterTester();
  void runTests();
  void runFileTests(const ParallelWriter<double>& reader);
  void runChunkTests(const ParallelWriter<double>& reader);
  void runWriteTests(ParallelWriter<double>& reader);
private:
   const int MASTER = 0;
   int id;
   int numProcs;
};

DoubleWriterTester::DoubleWriterTester() {
   MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
   MPI_Comm_rank(MPI_COMM_WORLD, &id);
}


void DoubleWriterTester::runTests() {
   if (id == MASTER) cout << "\nTesting ParallelWriter using doubles...\n"
                          << flush;
   
   ParallelWriter<double> 
     writer("./files/6doubles.bin", MPI_DOUBLE, id, numProcs);

   runFileTests(writer);
   runWriteTests(writer);
   runChunkTests(writer);

   if (id == MASTER) cout << "All double tests passed!\n" << endl;
}

void DoubleWriterTester::
runFileTests(const ParallelWriter<double>& writer) {
   if (id == MASTER) cout << "- Running getter tests..." << flush;
   MPI_Barrier(MPI_COMM_WORLD);

   assert( writer.getID() == id );
   assert( writer.getNumPEs() == numProcs );
   assert( writer.getFileName() == "./files/6doubles.bin" );
   assert( writer.getItemSize() == 8 );          // 8 bytes in a double

   MPI_Barrier(MPI_COMM_WORLD);
   if (id == MASTER) cout << " Passed! " << endl;
   MPI_Barrier(MPI_COMM_WORLD);
}

int approximatelyEqual(double v1, double v2) {
   const double THRESHOLD = 0.0000000000001;

   return abs(v2-v1) < THRESHOLD;
}


void DoubleWriterTester::
runWriteTests(ParallelWriter<double>& writer) {
   MPI_Barrier(MPI_COMM_WORLD);
   if (id == MASTER) cout << "- Running write() tests... " << flush;

   const int SIZE = 6;
   long start = -1, stop = -1;
   getChunkStartStopValues(id, numProcs, SIZE, start, stop);
   int chunkSize = stop - start;
   vector<double> v1(chunkSize);
   double dVal = (id+1) * 11.1;
   for (int i = 0; i < chunkSize; ++i) {
      v1[i] = dVal;
      dVal += 11.1;
   }
   writer.writeChunk(v1);
   
   assert( writer.getFileSize() == 48 );         // 8 x 6
   assert( writer.getNumItemsInFile() == 6 );

   ParallelReader<double> pReader( writer.getFileName(),
                                    MPI_DOUBLE, id, numProcs );
   vector<double> v2 = pReader.readChunk();

   assert( v2.size() == v1.size() );
   for (int i = 0; i < v2.size(); ++i) {
      assert( v2[i] == v1[i] );
   }

   MPI_Barrier(MPI_COMM_WORLD);
   if (id == MASTER) cout << " Passed!" << endl;
}

void DoubleWriterTester::
runChunkTests(const ParallelWriter<double>& writer) {
   if (id == MASTER) cout << "- Running chunk-related tests... "  << flush;
   MPI_Barrier(MPI_COMM_WORLD);

   switch (numProcs) {
     case 1:   // 6
           assert( writer.getChunkSize() == 6 );
//           cout << " 0a " << flush;
           assert( writer.getFirstItemOffset() == 0 );
           assert( writer.getFirstByteOffset() == 0 );
//           cout << " 0b " << flush;
           break;
     case 2:   // 3,3
           switch (id) {
             case 0: assert( writer.getChunkSize() == 3 );
//                     cout << " 0a " << flush;
                     assert( writer.getFirstItemOffset() == 0 );
                     assert( writer.getFirstByteOffset() == 0 );
//                     cout << " 0b " << flush;
                     break;
             case 1: assert( writer.getChunkSize() == 3 );
//                     cout << " 0c " << flush;
                     assert( writer.getFirstItemOffset() == 3 );
                     assert( writer.getFirstByteOffset() == 24 );
//                     cout << " 0d " << flush;
                     break;
           }
           break;
     case 3:   // 2,2,2
           switch (id) {
             case 0: assert( writer.getChunkSize() == 2 );
//                     cout << " 0a " << flush;
                     assert( writer.getFirstItemOffset() == 0 );
                     assert( writer.getFirstByteOffset() == 0 );
//                     cout << " 0b " << flush;
                     break;
             case 1: assert( writer.getChunkSize() == 2 );
//                     cout << " 0c " << flush;
                     assert( writer.getFirstItemOffset() == 2 );
                     assert( writer.getFirstByteOffset() == 16 );
//                     cout << " 0d " << flush;
                     break;
             case 2: assert( writer.getChunkSize() == 2 );
//                     cout << " 0e " << flush;
                     assert( writer.getFirstItemOffset() == 4 );
                     assert( writer.getFirstByteOffset() == 32 );
//                     cout << " 0f " << flush;
                     break;
           }
           break;
     case 4:   // 2,2,1,1
           switch (id) {
             case 0: assert( writer.getChunkSize() == 2 );
//                     cout << " 0a " << flush;
                     assert( writer.getFirstItemOffset() == 0 );
                     assert( writer.getFirstByteOffset() == 0 );
//                     cout << " 0b " << flush;
                     break;
             case 1: assert( writer.getChunkSize() == 2 );
//                     cout << " 0c " << flush;
                     assert( writer.getFirstItemOffset() == 2 );
                     assert( writer.getFirstByteOffset() == 16 );
//                     cout << " 0d " << flush;
                     break;
             case 2: assert( writer.getChunkSize() == 1 );
//                     cout << " 0e " << flush;
                     assert( writer.getFirstItemOffset() == 4 );
                     assert( writer.getFirstByteOffset() == 32 );
//                     cout << " 0f " << flush;
                     break;
             case 3: assert( writer.getChunkSize() == 1 );
//                     cout << " 0g " << flush;
                     assert( writer.getFirstItemOffset() == 5 );
                     assert( writer.getFirstByteOffset() == 40 );
//                     cout << " 0h " << flush;
                     break;
           }
           break;
      case 5:   // 2,1,1,1,1
           switch (id) {
             case 0:
               assert( writer.getChunkSize() == 2 );
               assert( writer.getFirstItemOffset() == 0 );
               assert( writer.getFirstByteOffset() == 0 );
               break;
             case 1:
               assert( writer.getChunkSize() == 1 );
               assert (writer.getFirstItemOffset() == 2 );
               assert (writer.getFirstByteOffset() == 16 );
               break;
             case 2:
               assert( writer.getChunkSize() == 1 );
               assert (writer.getFirstItemOffset() == 3 );
               assert (writer.getFirstByteOffset() == 24 );
               break;
             case 3:
               assert( writer.getChunkSize() == 1 );
               assert (writer.getFirstItemOffset() == 4 );
               assert (writer.getFirstByteOffset() == 32 );
               break;
             case 4:
               assert( writer.getChunkSize() == 1 );
               assert (writer.getFirstItemOffset() == 5 );
               assert (writer.getFirstByteOffset() == 40 );
               break;
             default:
               cerr << "\n*** Switch 2: flow should not reach here\n\n";
               MPI_Abort(MPI_COMM_WORLD, 2);
           }
           break;
      case 6:   // 1,1,1,1,1
           switch (id) {
             case 0:
               assert( writer.getChunkSize() == 1 );
               assert( writer.getFirstItemOffset() == 0 );
               assert( writer.getFirstByteOffset() == 0 );
               break;
             case 1:
               assert( writer.getChunkSize() == 1 );
               assert (writer.getFirstItemOffset() == 1 );
               assert (writer.getFirstByteOffset() == 8 );
               break;
             case 2:
               assert( writer.getChunkSize() == 1 );
               assert (writer.getFirstItemOffset() == 2 );
               assert (writer.getFirstByteOffset() == 16 );
               break;
             case 3:
               assert( writer.getChunkSize() == 1 );
               assert (writer.getFirstItemOffset() == 3 );
               assert (writer.getFirstByteOffset() == 24 );
               break;
             case 4:
               assert( writer.getChunkSize() == 1 );
               assert (writer.getFirstItemOffset() == 4 );
               assert (writer.getFirstByteOffset() == 32 );
               break;
             case 5:
               assert( writer.getChunkSize() == 1 );
               assert (writer.getFirstItemOffset() == 5 );
               assert (writer.getFirstByteOffset() == 40 );
               break;
             default:
               cerr << "\n*** Switch 2: flow should not reach here\n\n";
               MPI_Abort(MPI_COMM_WORLD, 2);
           }
           break;
      default:
           cerr << "\n*** Switch: flow should never reach here\n\n";
           MPI_Abort(MPI_COMM_WORLD, 1);
   }

   MPI_Barrier(MPI_COMM_WORLD);
   if (id == MASTER) cout << " Passed! " << endl;

}

