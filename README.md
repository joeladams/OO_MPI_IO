# OO_MPI_IO

<img align="right" src="/assets/images/OO_MPI_IO.png" alt="The OO_MPI_IO hierarchy" 
      width="246" height="274" >

To simplify parallel I/O, **OO_MPI_IO** provides a minimal hierarchy of three C++ class-templates that build on MPI-IO to hide its complexity. 
For ease of use, all functionality is provided by a single file *OO_MPI_IO.h*; to use it, 
just download and copy it into your project folder, add the directive `#include "OO_MPI_IO.h"` to your MPI source code, 
and you can begin using its `ParallelReader` or `ParallelWriter` abstractions in your project. 

The following diagram illustrates parallel input: a group of P MPI processes collectively divide a binary input file *F.bin* 
into "chunks"; each process then reads its chunk in parallel with the other processes:

<img src="/assets/images/ParallelInput.png" alt="Reading from a file in parallel" 
      width="350" height="125" >

Each process can then process its "chunk" of the data independently, after which those partial results can be combined into a total result using an MPI collective operation such as `MPI_Reduce()` or `MPI_Gather()` or ...

Note: OO_MPI_IO expects data files (especially numeric data) to be in binary format. 
See the folder *genTextAndBinaryFiles* for programs that illustrate how to generate such files. 
See also the folder *tests* for examples that show how to use OO_MPI_IO.

Usage example:

    ...
    #include "OO_MPI_IO.h"
    ...
    int main(int argc, char** argv) {
      ...
      int id = 0;
      int numProcs = 0;
      MPI_Init(&argc, &argv);
      MPI_Comm_rank(&id, MPI_COMM_WORLD);
      MPI_Comm_size(&numProcs, MPI_COMM_WORLD);
      char* inFileName = getCmdLineArgs(argc, argv);
      ...
      ParallelReader<double> reader(inFileName, MPI_DOUBLE, id, numProcs);
      std::vector<double> vec = reader.readChunk();
      reader.close();
      // vec now contains this MPI process's "chunk" of fileName's values
      for (int i = 0; i < vec.size(); ++i) {
         doSomethingWith(vec[i]);
      }
      ...
    }

