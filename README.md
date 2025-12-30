# OO_MPI_IO

<img align="right" src="/assets/images/OO_MPI_IO.png" alt="The OO_MPI_IO hierarchy" 
      width="246" height="274" >

To simplify parallel I/O, **OO_MPI_IO** provides a modest C++ hierarchy of three class-templates that hide the complexity of MPI-IO. 
For ease of use, all functionality is provided by a single file *OO_MPI_IO.h*; to use it, 
just download and copy it into your project folder, add the directive `#include "OO_MPI_IO.h"` to your MPI source code, 
and you can begin using its `ParallelReader` or `ParallelWriter` abstractions in your project. 

The following diagram illustrates parallel input:
a group of P processing elements (PEs: MPI processes or OpenMP threads)
collectively divide a binary input file *F.bin* into "chunks";
each process then simultaneously reads its chunk in parallel
with the other processes (or threads):

<img src="/assets/images/ParallelInput.png" alt="Reading from a file in parallel" 
      width="350" height="125" >

Each PE can then examine its "chunk" of the data independently,
producing a partial result.
Afterwards, the PEs can combine those partial results into a total result
using a collective operation such as a reduction, gather, etc.

Note: OO_MPI_IO expects data files (especially numeric data)
to be in binary format. 
See the folder *genTextAndBinaryFiles* for programs
that illustrate how to generate such files. 
See also the folder *tests* for examples that show how to use OO_MPI_IO.

MPI usage example:

    ...
    #include "OO_MPI_IO.h"
    ...
    int main(int argc, char** argv) {
      ...
      int id = 0;
      int P = 0;
      MPI_Init(&argc, &argv);
      MPI_Comm_rank(&id, MPI_COMM_WORLD);
      MPI_Comm_size(&P, MPI_COMM_WORLD);
      char* inFileName = getCmdLineArgs(argc, argv);
      ...
      
      ParallelReader<double> reader(inFileName, MPI_DOUBLE, id, P);
      std::vector<double> vec = reader.readChunk();
      reader.close();
      
      // vec now contains this process's "chunk" of fileName's values
      // so the P PEs can now process their chunks in parallel
      for (int i = 0; i < vec.size(); ++i) {
         doSomethingWith(vec[i]);
      }
      ...
    }

OpenMP usage example:

    ...
    #include "OO_MPI_IO.h"
    ...
    int main(int argc, char** argv) {
      ...
      #pragma omp parallel
      {
         int id = omp_get_thread_num();
         int P = omp_get_num_threads();
         char* inFileName = getCmdLineArgs(argc, argv);
         ...
      
         ParallelReader<double> reader(inFileName, MPI_DOUBLE, id, P);
         std::vector<double> vec = reader.readChunk();
         reader.close();
      
         // vec now contains this thread's "chunk" of fileName's values
         // so the P PEs can now process their chunks in parallel
         for (int i = 0; i < vec.size(); ++i) {
            doSomethingWith(vec[i]);
         }
         ...
      }
    }

