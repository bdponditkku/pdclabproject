#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

#define TOKEN 1
#define NO_TOKEN 0

int main(int argc, char* argv[]) {
    int rank, size, token = NO_TOKEN;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;

    // P0 starts with the token
    if (rank == 0) {
        token = TOKEN;
    }

    while (1) {
        if (token == TOKEN) {
            printf("Process %d: I have the token. Accessing printer...\n", rank);
            sleep(2); // Simulate printing
            printf("Process %d: Done printing. Passing token to Process %d\n", rank, next);
            token = NO_TOKEN;
            MPI_Send(&token, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        } else {
            MPI_Recv(&token, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &status);
        }
        sleep(1);
    }

    MPI_Finalize();
    return 0;
}
