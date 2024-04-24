#define _DEFAULT_SOURCE

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    if (world_size != 2)
    {
        printf("Run: mpirun -n 2 exefile\n");
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);


    if (world_rank == 0)
    {
        FILE *file = fopen("test.txt", "r");
        if (file == NULL)
        {
            perror("Error opening file for reading");
            MPI_Finalize();
            exit(EXIT_FAILURE);
        }

        fseek(file, 0, SEEK_END);
        long filesize = ftell(file);
        fseek(file, 0, SEEK_SET);

        char *buf = (char *)malloc(sizeof(char) * filesize);
        fread(buf, 1, filesize, file);
        fclose(file);

        MPI_Send(buf, filesize, MPI_BYTE, 1, 1234, MPI_COMM_WORLD);
        printf("Server sent file successfully!\n");
    }

    else if (world_rank == 1)
    {
        MPI_Status stat;
        MPI_Probe(0, 1234, MPI_COMM_WORLD, &stat);
        int filesize;
        MPI_Get_count(&stat, MPI_BYTE, &filesize);
        char *buf = (char *)malloc(sizeof(char) * filesize);

        MPI_Recv(buf, filesize, MPI_BYTE, 0, 1234, MPI_COMM_WORLD, &stat);
        FILE *file = fopen("test2.txt", "w");
        if (file == NULL)
        {
            perror("Error opening file for writing");
            MPI_Finalize();
            exit(EXIT_FAILURE);
        }
        printf("Saved to test2.txt\n");

        fwrite(buf, 1, filesize, file);
        fclose(file);
        free(buf);
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}