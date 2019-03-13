
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

  int initial, final, partial, total;
  initial = world_rank*(MAXARRAY/world_size);
  final = (world_rank + 1)*(MAXARRAY/world_size)-1;
  partial = 0;
  total = 0;

  if (world_rank == 0) {
    int i;
    // We initialize vector
    for (i = 0; i < MAXARRAY; i++) a[i] = i;

    for (int j = 1; j < world_size; j++){ 
       MPI_Send(
      /* data         = */ &a, 
      /* count        = */ MAXARRAY, 
      /* datatype     = */ MPI_INT, 
      /* destination  = */ j, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD);
    }
    printf("Suma en Proceso: %d Inicial: %d, Final: %d \n", world_rank, initial,final);
    fflush(stdout);

    for (int i = initial; i <= final; ++i)
    {
      partial = partial + i;
    }
    total = total + partial;

    for(int j = 1; j < world_size; j++){
      MPI_Recv(
      /* data         = */ &partial, 
      /* count        = */ 1, 
      /* datatype     = */ MPI_INT, 
      /* source       = */ j, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD, 
      /* status       = */ MPI_STATUS_IGNORE);
        total = total + partial;
    }

    printf("La suma de los elementos es: %d\n", total);

  } else {
    MPI_Recv(
      /* data         = */ &a, 
      /* count        = */ MAXARRAY, 
      /* datatype     = */ MPI_INT, 
      /* source       = */ 0, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD, 
      /* status       = */ MPI_STATUS_IGNORE);
    printf("Suma en Proceso: %d Inicial: %d, Final: %d \n", world_rank, initial,final);
    fflush(stdout);

      for (int i = initial; i <= final; ++i)
      {
        partial = partial + a[i];
      }

      MPI_Send(
      /* data         = */ &partial, 
      /* count        = */ 1, 
      /* datatype     = */ MPI_INT, 
      /* destination  = */ 0, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD);

  }
  MPI_Finalize();
}