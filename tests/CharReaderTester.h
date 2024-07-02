/* CharReaderTester.h declares the class that tests ParallelReader
 *   using char values.
 *
 * @author: Joel C. Adams, Calvin University, Fall 2023
 */

#include <iostream>                // cout, ...
#include <fstream>                 // ifstream, ofstream, fstream
#include <mpi.h>                   // MPI types
#include "../OO_MPI_IO.h"          // ParallelReader
using namespace std;

class CharReaderTester {
public:
  CharReaderTester();
  void runTests();
  void runFileTests(const ParallelReader<char>& reader);
  void runReadTests(ParallelReader<char>& reader);
  void runChunkTests(const ParallelReader<char>& reader);
private:
   const int MASTER = 0;
   int id;
   int numProcs;
};

CharReaderTester::CharReaderTester() {
   MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
   MPI_Comm_rank(MPI_COMM_WORLD, &id);
}


void CharReaderTester::runTests() {
   if (id == MASTER) cout << "\nTesting ParallelReader using chars...\n"
                          << flush;
   
   ParallelReader<char> 
     reader("./files/5chars.bin", MPI_CHAR, id, numProcs);

   runFileTests(reader);
   runReadTests(reader);
   runChunkTests(reader);

   if (id == MASTER) cout << "All char tests passed!\n" << endl;
}

void CharReaderTester::
runFileTests(const ParallelReader<char>& reader) {
   if (id == MASTER) cout << "- Running getter tests..." << flush;
   MPI_Barrier(MPI_COMM_WORLD);

   assert( reader.getRank() == id );
   assert( reader.getNumProcs() == numProcs );
   assert( reader.getFileName() == "./files/5chars.bin" );
   assert( reader.getItemSize() == 1 );          // 1 byte in a char

   MPI_Barrier(MPI_COMM_WORLD);
   if (id == MASTER) cout << " Passed! " << endl;
   MPI_Barrier(MPI_COMM_WORLD);
}

void CharReaderTester::
runChunkTests(const ParallelReader<char>& reader) {
   if (id == MASTER) cout << "- Running chunk-related tests... "  << flush;
   MPI_Barrier(MPI_COMM_WORLD);

   switch (numProcs) {
     case 1:
           assert( reader.getChunkSize() == 5 );
           assert( reader.getFirstItemOffset() == 0 );
           assert( reader.getFirstByteOffset() == 0 );
           break;
     case 2:
           switch (id) {
             case 0: assert( reader.getChunkSize() == 3 );
                     assert( reader.getFirstItemOffset() == 0 );
                     assert( reader.getFirstByteOffset() == 0 );
                     break;
             case 1: assert( reader.getChunkSize() == 2 );
                     assert( reader.getFirstItemOffset() == 3 );
                     assert( reader.getFirstByteOffset() == 3 );
                     break;
           }
           break;
     case 3:
           switch (id) {
             case 0: assert( reader.getChunkSize() == 2 );
                     assert( reader.getFirstItemOffset() == 0 );
                     assert( reader.getFirstByteOffset() == 0 );
                     break;
             case 1: assert( reader.getChunkSize() == 2 );
                     assert( reader.getFirstItemOffset() == 2 );
                     assert( reader.getFirstByteOffset() == 2 );
                     break;
             case 2: assert( reader.getChunkSize() == 1 );
                     assert( reader.getFirstItemOffset() == 4 );
                     assert( reader.getFirstByteOffset() == 4 );
                     break;
           }
           break;
     case 4:
           switch (id) {
             case 0: assert( reader.getChunkSize() == 2 );
                     assert( reader.getFirstItemOffset() == 0 );
                     assert( reader.getFirstByteOffset() == 0 );
                     break;
             case 1: assert( reader.getChunkSize() == 1 );
                     assert( reader.getFirstItemOffset() == 2 );
                     assert( reader.getFirstByteOffset() == 2 );
                     break;
             case 2: assert( reader.getChunkSize() == 1 );
                     assert( reader.getFirstItemOffset() == 3 );
                     assert( reader.getFirstByteOffset() == 3 );
                     break;
             case 3: assert( reader.getChunkSize() == 1 );
                     assert( reader.getFirstItemOffset() == 4 );
                     assert( reader.getFirstByteOffset() == 4 );
                     break;
           }
           break;
      case 5:
           switch (id) {
             case 0:
               assert( reader.getChunkSize() == 1 );
               assert( reader.getFirstItemOffset() == 0 );
               assert( reader.getFirstByteOffset() == 0 );
               break;
             case 1:
               assert( reader.getChunkSize() == 1 );
               assert (reader.getFirstItemOffset() == 1 );
               assert (reader.getFirstByteOffset() == 1 );
               break;
             case 2:
               assert( reader.getChunkSize() == 1 );
               assert (reader.getFirstItemOffset() == 2 );
               assert (reader.getFirstByteOffset() == 2 );
               break;
             case 3:
               assert( reader.getChunkSize() == 1 );
               assert (reader.getFirstItemOffset() == 3 );
               assert (reader.getFirstByteOffset() == 3 );
               break;
             case 4:
               assert( reader.getChunkSize() == 1 );
               assert (reader.getFirstItemOffset() == 4 );
               assert (reader.getFirstByteOffset() == 4 );
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



void CharReaderTester::
runReadTests(ParallelReader<char>& reader) {
   MPI_Barrier(MPI_COMM_WORLD);
   if (id == MASTER) cout << "- Running read() tests... " << flush;

   vector<char> v1 = reader.readChunk();

   assert( reader.getFileSize() == 5 );          // 1 x 5
   assert( reader.getNumItemsInFile() == 5 );
   const int SIZE = reader.getNumItemsInFile();
   vector<char> v2;
   char cVal;
   ifstream fin("files/5chars.txt");
   assert( fin.is_open() );
   for (int i = 0; i < SIZE; ++i) {
      fin >> cVal;
      v2.push_back(cVal);
   } 
   fin.close();

   long start = -1, stop = -1;
   getChunkStartStopValues(id, numProcs, SIZE, start, stop);
// printf("\n\nv1.size(): %lu, (stop-start): %ld\n\n", v1.size(), (stop-start));
   assert( v1.size() == (stop - start) );
   int j = start;
   for (int i = 0; i < v1.size(); ++i) {
      assert( v1[i] == v2[j] );
//        printf("Process %d: v1[%d]: %lf; v2[%d]: %lf\n",
//                      id, i, v1[i], i, v2[i]);
      ++j;
   }

   MPI_Barrier(MPI_COMM_WORLD);
   if (id == MASTER) cout << " Passed!" << endl;
}

