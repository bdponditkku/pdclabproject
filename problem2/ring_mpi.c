#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int message;
    int send_to, recv_from;

    MPI_Init(&argc, &argv);               // Initialize the MPI environment
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get the rank of the process
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get the total number of processes

    // Set up the ring: who to send to and receive from
    send_to = (rank + 1) % size;
    recv_from = (rank - 1 + size) % size;

    // Initialize the message with rank (can be anything)
    message = rank;

    // Use MPI_Sendrecv to send and receive in one call (avoids deadlock)
    int received_message;
    MPI_Sendrecv(&message, 1, MPI_INT, send_to, 0,
                 &received_message, 1, MPI_INT, recv_from, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("Process %d received message %d from process %d\n", rank, received_message, recv_from);

    MPI_Finalize(); // Finalize the MPI environment
    return 0;
}
