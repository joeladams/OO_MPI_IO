/* CharWriterTester.h declares the class that tests ParallelWriter
 *   using char values.
 *
 * @author: Joel C. Adams, Calvin University, Fall 2023
 */

#include <iostream>                // cout, ...
#include <fstream>                 // ifstream, ofstream, fstream
#include <mpi.h>                   // MPI types
#include "../OO_MPI_IO.h"          // ParallelWriter
using namespace std;

class CharWriterTester {
public:
  CharWriterTester();
  void runTests();
  void runFileTests(const ParallelWriter<char>& writer);
  void runWriteTests(ParallelWriter<char>& writer);
  void runChunkTests(const ParallelWriter<char>& writer);
private:
   const int MASTER = 0;
   int id;
   int numProcs;
};

CharWriterTester::CharWriterTester() {
   MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
   MPI_Comm_rank(MPI_COMM_WORLD, &id);
}


void CharWriterTester::runTests() {
   if (id == MASTER) cout << "\nTesting ParallelWriter using chars...\n"
                          << flush;
   
   ParallelWriter<char> 
     writer("./files/6chars.bin", MPI_CHAR, id, numProcs);

   runFileTests(writer);
   runWriteTests(writer);
   runChunkTests(writer);

   if (id == MASTER) cout << "All char tests passed!\n" << endl;
}

void CharWriterTester::
runFileTests(const ParallelWriter<char>& writer) {
   if (id == MASTER) cout << "- Running getter tests..." << flush;
   MPI_Barrier(MPI_COMM_WORLD);

   assert( writer.getRank() == id );
   assert( writer.getNumProcs() == numProcs );
   assert( writer.getFileName() == "./files/6chars.bin" );
   assert( writer.getItemSize() == 1 );          // 1 byte in a char

   MPI_Barrier(MPI_COMM_WORLD);
   if (id == MASTER) cout << " Passed! " << endl;
   MPI_Barrier(MPI_COMM_WORLD);
}


void CharWriterTester::
runWriteTests(ParallelWriter<char>& writer) {
   MPI_Barrier(MPI_COMM_WORLD);
   if (id == MASTER) cout << "- Running write() tests... " << flush;

   const int SIZE = 6;
   long start = -1, stop = -1;
   getChunkStartStopValues(id, numProcs, SIZE, start, stop);
   int chunkSize = stop - start;
   vector<char> v1(chunkSize);
   char cVal = 'a';
   for (int i = 0; i < chunkSize; ++i) {
      v1[i] = cVal;
      cVal += 2;
   }
   writer.writeChunk(v1);
   
   assert( writer.getFileSize() == 6 );         // 1 x 6
   assert( writer.getNumItemsInFile() == 6 );

   ParallelReader<char> pReader( writer.getFileName(),
                                    MPI_CHAR, id, numProcs );
   vector<char> v2 = pReader.readChunk();

   assert( v2.size() == v1.size() );
   for (int i = 0; i < v2.size(); ++i) {
      assert( v2[i] == v1[i] );
   }

   MPI_Barrier(MPI_COMM_WORLD);
   if (id == MASTER) cout << " Passed!" << endl;
}


void CharWriterTester::
runChunkTests(const ParallelWriter<char>& writer) {
   if (id == MASTER) cout << "- Running chunk-related tests... "  << flush;
   MPI_Barrier(MPI_COMM_WORLD);

   switch (numProcs) {
     case 1:   // 6
           assert( writer.getChunkSize() == 6 );
           assert( writer.getFirstItemOffset() == 0 );
           assert( writer.getFirstByteOffset() == 0 );
           break;
     case 2:   // 3,3
           switch (id) {
             case 0: assert( writer.getChunkSize() == 3 );
                     assert( writer.getFirstItemOffset() == 0 );
                     assert( writer.getFirstByteOffset() == 0 );
                     break;
             case 1: assert( writer.getChunkSize() == 3 );
                     assert( writer.getFirstItemOffset() == 3 );
                     assert( writer.getFirstByteOffset() == 3 );
                     break;
           }
           break;
     case 3:   // 2,2,2
           switch (id) {
             case 0: assert( writer.getChunkSize() == 2 );
                     assert( writer.getFirstItemOffset() == 0 );
                     assert( writer.getFirstByteOffset() == 0 );
                     break;
             case 1: assert( writer.getChunkSize() == 2 );
                     assert( writer.getFirstItemOffset() == 2 );
                     assert( writer.getFirstByteOffset() == 2 );
                     break;
             case 2: assert( writer.getChunkSize() == 2 );
                     assert( writer.getFirstItemOffset() == 4 );
                     assert( writer.getFirstByteOffset() == 4 );
                     break;
           }
           break;
     case 4:   // 2,2,1,1
           switch (id) {
             case 0: assert( writer.getChunkSize() == 2 );
                     assert( writer.getFirstItemOffset() == 0 );
                     assert( writer.getFirstByteOffset() == 0 );
                     break;
             case 1: assert( writer.getChunkSize() == 2 );
                     assert( writer.getFirstItemOffset() == 2 );
                     assert( writer.getFirstByteOffset() == 2 );
                     break;
             case 2: assert( writer.getChunkSize() == 1 );
                     assert( writer.getFirstItemOffset() == 4 );
                     assert( writer.getFirstByteOffset() == 4 );
                     break;
             case 3: assert( writer.getChunkSize() == 1 );
                     assert( writer.getFirstItemOffset() == 5 );
                     assert( writer.getFirstByteOffset() == 5 );
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
               assert (writer.getFirstByteOffset() == 2 );
               break;
             case 2:
               assert( writer.getChunkSize() == 1 );
               assert (writer.getFirstItemOffset() == 3 );
               assert (writer.getFirstByteOffset() == 3 );
               break;
             case 3:
               assert( writer.getChunkSize() == 1 );
               assert (writer.getFirstItemOffset() == 4 );
               assert (writer.getFirstByteOffset() == 4 );
               break;
             case 4:
               assert( writer.getChunkSize() == 1 );
               assert (writer.getFirstItemOffset() == 5 );
               assert (writer.getFirstByteOffset() == 5 );
               break;
             default:
               cerr << "\n*** Switch 2: flow should not reach here\n\n";
               MPI_Abort(MPI_COMM_WORLD, 2);
           }
           break;
      case 6:   // 1,1,1,1,1,1
           switch (id) {
             case 0:
               assert( writer.getChunkSize() == 1 );
               assert( writer.getFirstItemOffset() == 0 );
               assert( writer.getFirstByteOffset() == 0 );
               break;
             case 1:
               assert( writer.getChunkSize() == 1 );
               assert (writer.getFirstItemOffset() == 1 );
               assert (writer.getFirstByteOffset() == 1 );
               break;
             case 2:
               assert( writer.getChunkSize() == 1 );
               assert (writer.getFirstItemOffset() == 2 );
               assert (writer.getFirstByteOffset() == 2 );
               break;
             case 3:
               assert( writer.getChunkSize() == 1 );
               assert (writer.getFirstItemOffset() == 3 );
               assert (writer.getFirstByteOffset() == 3 );
               break;
             case 4:
               assert( writer.getChunkSize() == 1 );
               assert (writer.getFirstItemOffset() == 4 );
               assert (writer.getFirstByteOffset() == 4 );
               break;
             case 5:
               assert( writer.getChunkSize() == 1 );
               assert (writer.getFirstItemOffset() == 5 );
               assert (writer.getFirstByteOffset() == 5 );
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



