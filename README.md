# OO_MPI_IO
OO_MPI_IO provides a hierarchy of C++ templates that simplify parallel I/O by building on (and hiding the complexity of) MPI-IO. For ease of use, all functionality is provided by a single file OO_MPI_IO.h; just copy it into your project folder, add the directive #include "OO_MPI_IO.h" to your MPI source code, and you can begin using its ParallelReader or ParallelWriter abstractions in your project.

Usage example:
  ...
  #include "OO_MPI_IO.h"
  ...
  int main(int argc, char** argv) {
    ...
    int id = 0;
    int numProcs = 0;
    char* fileName = getCmdLineArgs(argc, argv);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(&id, MPI_COMM_WORLD);
    MPI_Comm_size(&numProcs, MPI_COMM_WORLD);
    ...
    ParallelReader<double> reader(fileName, MPI_DOUBLE, id, numProcs);
    std::vector<double> vec;
    reader.readChunk(vec);
    reader.close();
    // vec now contains this MPI process's "chunk" of the values in fileName
    for (int i = 0; i < vec.size(); ++i) {
       doSomethingWith(vec[i]);
    }
    ...
  }
