
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXARRAY 20

void imprimirvector(int V[],int MA, int pr){
  printf("Vector en proceso %d:",pr);
  for (int i = 0; i < MA; i++){
    printf("%d ",V[i]);
  }
  printf("\n");
  fflush(stdout);
}

int main(int argc, char** argv) {
  int a[MAXARRAY];
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

  int number;
  if (world_rank == 0) {
    int i;
    // We initialize vector
    for (i = 0; i < MAXARRAY; i++) a[i] = i;
    MPI_Send(
      /* data         = */ &a, 
      /* count        = */ MAXARRAY, 
      /* datatype     = */ MPI_INT, 
      /* destination  = */ 1, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD);

    MPI_Recv(
      /* data         = */ &a, 
      /* count        = */ MAXARRAY, 
      /* datatype     = */ MPI_INT, 
      /* source       = */ 1, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD, 
      /* status       = */ MPI_STATUS_IGNORE);
    printf("[0] Vector recibido\n");
    imprimirvector(a,MAXARRAY,0);
  } else if (world_rank == 1) {
    printf("[1] Vector inicial\n");
    int i;
    imprimirvector(a,MAXARRAY,1);
    printf("[1] Recibiendo el vector\n");
    fflush(stdout);
    MPI_Recv(
      /* data         = */ &a, 
      /* count        = */ MAXARRAY, 
      /* datatype     = */ MPI_INT, 
      /* source       = */ 0, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD, 
      /* status       = */ MPI_STATUS_IGNORE);
    printf("[1] Vector recibido\n");
    fflush(stdout);
    imprimirvector(a,MAXARRAY,1);
    for (i = 0; i < MAXARRAY; i++){ 
      a[i] = a[i]*a[i];
    }
        MPI_Send(
      /* data         = */ &a, 
      /* count        = */ MAXARRAY, 
      /* datatype     = */ MPI_INT, 
      /* destination  = */ 0, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD);
    printf("[1] Enviando Vector de vuelta\n");
    fflush(stdout);

  }
  MPI_Finalize();
}