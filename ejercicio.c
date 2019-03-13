#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // We are assuming at least 2 processes for this task
  if (world_size < 2) {
    fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  char value;
  if (world_rank == 0) {
    // If we are rank 0, set the value to -1 and send it to process 1
    value = 'a';
    MPI_Send(
      /* data         = */ &value, 
      /* count        = */ 1, 
      /* datatype     = */ MPI_CHAR, 
      /* destination  = */ 1, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD);
    printf("Process 0 send char value '%c' to process 1\n", value);
    fflush(stdout);

    MPI_Recv(
      /* data         = */ &value, 
      /* count        = */ 1, 
      /* datatype     = */ MPI_CHAR, 
      /* source       = */ 1, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD, 
      /* status       = */ MPI_STATUS_IGNORE);
    printf("Process 0 received value %c from process 1\n", value);
    fflush(stdout);

  } else if (world_rank == 1) {
    MPI_Recv(
      /* data         = */ &value, 
      /* count        = */ 1, 
      /* datatype     = */ MPI_CHAR, 
      /* source       = */ 0, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD, 
      /* status       = */ MPI_STATUS_IGNORE);
    printf("Process 1 received value %c from process 0\n", value);
    fflush(stdout);

    if(isupper(value)){
    value = tolower(value);
    }else{
    value = toupper(value);      
    }

    MPI_Send(
      /* data         = */ &value, 
      /* count        = */ 1, 
      /* datatype     = */ MPI_CHAR, 
      /* destination  = */ 0, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD);
    printf("Process 1 send char value '%c' to process 0\n", value);
    fflush(stdout);
  }
  MPI_Finalize();
}
