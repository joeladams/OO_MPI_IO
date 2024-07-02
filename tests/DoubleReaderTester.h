/* DoubleReaderTester.h declares the class that tests ParallelReader
 *   using double values.
 *
 * @author: Joel C. Adams, Calvin University, Fall 2023
 */

#include <iostream>                // cout, ...
#include <fstream>                 // ifstream, ofstream, fstream
#include <mpi.h>                   // MPI types
#include "../OO_MPI_IO.h"          // ParallelReader
using namespace std;

class DoubleReaderTester {
public:
  DoubleReaderTester();
  void runTests();
  void runFileTests(const ParallelReader<double>& reader);
  void runChunkTests(const ParallelReader<double>& reader);
  void runReadTests(ParallelReader<double>& reader);
private:
   const int MASTER = 0;
   int id;
   int numProcs;
};

DoubleReaderTester::DoubleReaderTester() {
   MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
   MPI_Comm_rank(MPI_COMM_WORLD, &id);
}


void DoubleReaderTester::runTests() {
   if (id == MASTER) cout << "\nTesting ParallelReader using doubles...\n"
                          << flush;
   
   ParallelReader<double> 
     reader("./files/5doubles.bin", MPI_DOUBLE, id, numProcs);

   runFileTests(reader);
   runReadTests(reader);
   runChunkTests(reader);

   if (id == MASTER) cout << "All double tests passed!\n" << endl;
}

void DoubleReaderTester::
runFileTests(const ParallelReader<double>& reader) {
   if (id == MASTER) cout << "- Running getter tests..." << flush;
   MPI_Barrier(MPI_COMM_WORLD);

   assert( reader.getRank() == id );
   assert( reader.getNumProcs() == numProcs );
   assert( reader.getFileName() == "./files/5doubles.bin" );
   assert( reader.getItemSize() == 8 );          // 8 bytes in a double

   MPI_Barrier(MPI_COMM_WORLD);
   if (id == MASTER) cout << " Passed! " << endl;
   MPI_Barrier(MPI_COMM_WORLD);
}

void DoubleReaderTester::
runChunkTests(const ParallelReader<double>& reader) {
   if (id == MASTER) cout << "- Running chunk-related tests... "  << flush;
   MPI_Barrier(MPI_COMM_WORLD);

   switch (numProcs) {
     case 1:
           assert( reader.getChunkSize() == 5 );
//           cout << " 0a " << flush;
           assert( reader.getFirstItemOffset() == 0 );
           assert( reader.getFirstByteOffset() == 0 );
//           cout << " 0b " << flush;
           break;
     case 2:
           switch (id) {
             case 0: assert( reader.getChunkSize() == 3 );
//                     cout << " 0a " << flush;
                     assert( reader.getFirstItemOffset() == 0 );
                     assert( reader.getFirstByteOffset() == 0 );
//                     cout << " 0b " << flush;
                     break;
             case 1: assert( reader.getChunkSize() == 2 );
//                     cout << " 0c " << flush;
                     assert( reader.getFirstItemOffset() == 3 );
                     assert( reader.getFirstByteOffset() == 24 );
//                     cout << " 0d " << flush;
                     break;
           }
           break;
     case 3:
           switch (id) {
             case 0: assert( reader.getChunkSize() == 2 );
//                     cout << " 0a " << flush;
                     assert( reader.getFirstItemOffset() == 0 );
                     assert( reader.getFirstByteOffset() == 0 );
//                     cout << " 0b " << flush;
                     break;
             case 1: assert( reader.getChunkSize() == 2 );
//                     cout << " 0c " << flush;
                     assert( reader.getFirstItemOffset() == 2 );
                     assert( reader.getFirstByteOffset() == 16 );
//                     cout << " 0d " << flush;
                     break;
             case 2: assert( reader.getChunkSize() == 1 );
//                     cout << " 0e " << flush;
                     assert( reader.getFirstItemOffset() == 4 );
                     assert( reader.getFirstByteOffset() == 32 );
//                     cout << " 0f " << flush;
                     break;
           }
           break;
     case 4:
           switch (id) {
             case 0: assert( reader.getChunkSize() == 2 );
//                     cout << " 0a " << flush;
                     assert( reader.getFirstItemOffset() == 0 );
                     assert( reader.getFirstByteOffset() == 0 );
//                     cout << " 0b " << flush;
                     break;
             case 1: assert( reader.getChunkSize() == 1 );
//                     cout << " 0c " << flush;
                     assert( reader.getFirstItemOffset() == 2 );
                     assert( reader.getFirstByteOffset() == 16 );
//                     cout << " 0d " << flush;
                     break;
             case 2: assert( reader.getChunkSize() == 1 );
//                     cout << " 0e " << flush;
                     assert( reader.getFirstItemOffset() == 3 );
                     assert( reader.getFirstByteOffset() == 24 );
//                     cout << " 0f " << flush;
                     break;
             case 3: assert( reader.getChunkSize() == 1 );
//                     cout << " 0g " << flush;
                     assert( reader.getFirstItemOffset() == 4 );
                     assert( reader.getFirstByteOffset() == 32 );
//                     cout << " 0h " << flush;
                     break;
           }
           break;
      case 5:
           switch (id) {
             case 0:
               assert( reader.getChunkSize() == 1 );
//               cout << " 0a " << flush;
               assert( reader.getFirstItemOffset() == 0 );
               assert( reader.getFirstByteOffset() == 0 );
//               cout << " 0b " << flush;
               break;
             case 1:
               assert( reader.getChunkSize() == 1 );
//               cout << " 0c " << flush;
               assert (reader.getFirstItemOffset() == 1 );
               assert (reader.getFirstByteOffset() == 8 );
//               cout << " 0d " << flush;
               break;
             case 2:
               assert( reader.getChunkSize() == 1 );
//               cout << " 0e " << flush;
               assert (reader.getFirstItemOffset() == 2 );
               assert (reader.getFirstByteOffset() == 16 );
//               cout << " 0f " << flush;
               break;
             case 3:
               assert( reader.getChunkSize() == 1 );
//               cout << " 0g " << flush;
               assert (reader.getFirstItemOffset() == 3 );
               assert (reader.getFirstByteOffset() == 24 );
//               cout << " 0h " << flush;
               break;
             case 4:
               assert( reader.getChunkSize() == 1 );
//               cout << " 0i " << flush;
               assert (reader.getFirstItemOffset() == 4 );
               assert (reader.getFirstByteOffset() == 32 );
//               cout << " 0j " << flush;
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

int approximatelyEqual(double v1, double v2) {
   const double THRESHOLD = 0.0000000000001;

   return abs(v2-v1) < THRESHOLD;
}


void DoubleReaderTester::
runReadTests(ParallelReader<double>& reader) {
   MPI_Barrier(MPI_COMM_WORLD);
   if (id == MASTER) cout << "- Running read() tests... " << flush;

   vector<double> v1 = reader.readChunk();

   assert( reader.getFileSize() == 40 );         // 8 x 5
   assert( reader.getNumItemsInFile() == 5 );
   const int SIZE = reader.getNumItemsInFile();
   vector<double> v2;
   double dVal;
   ifstream fin("files/5doubles.txt");
   assert( fin.is_open() );
   for (int i = 0; i < SIZE; ++i) {
      fin >> dVal;
      v2.push_back(dVal);
   } 
   fin.close();

   long start = -1, stop = -1;
   getChunkStartStopValues(id, numProcs, SIZE, start, stop);
// printf("\n\nv1.size(): %lu, (stop-start): %ld\n\n", v1.size(), (stop-start));
   assert( v1.size() == (stop - start) );
   int j = start;
   for (int i = 0; i < v1.size(); ++i) {
      assert( approximatelyEqual(v1[i], v2[j]) );
//        printf("Process %d: v1[%d]: %lf; v2[%d]: %lf\n",
//                      id, i, v1[i], i, v2[i]);
      ++j;
   }

   MPI_Barrier(MPI_COMM_WORLD);
   if (id == MASTER) cout << " Passed!" << endl;
}

