# OO_MPI_IO

<img align="right" src="/assets/images/OO_MPI_IO.png" alt="The OO_MPI_IO hierarchy" 
      width="246" height="274" >

OO_MPI_IO provides a simple hierarchy of three C++ class-templates that simplify parallel I/O by building on (and hiding the complexity of) MPI-IO. For ease of use, all functionality is provided by a single file OO_MPI_IO.h; just copy it into your project folder, add the directive #include "OO_MPI_IO.h" to your MPI source code, and you can begin using its ParallelReader or ParallelWriter abstractions in your project. To illustrate, we might visualize P MPI processes each "chunking" a binary input file *F.bin* and then reading those chunks in parallel, as shown below:

<img src="/assets/images/ParallelInput.png" alt="Reading from a file in parallel" 
      width="350" height="125" >

Note: OO_MPI_IO expects files (especially numeric data) to be binary-formatted. See the folder `genTextAndBinaryFiles` for programs that illustrate how to generate such files. See also the folder `tests` for examples that show how to use OO_MPI_IO.

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

