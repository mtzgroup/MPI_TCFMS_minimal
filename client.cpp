#include "mpi.h"
#include <stdio.h>

int main(int argc, char* argv[])
{    
    MPI_Init(&argc, &argv);
    
    int proc;
    MPI_Comm_size(MPI_COMM_WORLD, &proc);

    int mpi_id;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_id);
    
    FILE* tc_input = fopen("tc_input", "r");
    if (tc_input == NULL)
    {
        printf("File tc_input cannot open!\n"); fflush(stdout);
        return 1;
    }

    char server_name[20] = { 0 };
    fscanf(tc_input, "&tc\nserver_name = %s", server_name);
    
    fclose(tc_input);
    printf("server_name = %s\n", server_name);

    char port_name[256];
    int ierr = MPI_Lookup_name(server_name, MPI_INFO_NULL, port_name);
    if (ierr != MPI_SUCCESS)
    {
        printf("Error on MPI_Lookup_name\n"); fflush(stdout);
        return 1;
    }
    printf("port_name = %s\n", port_name);

    int newcomm;
    MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &newcomm);

    MPI_Status mpi_status;
    int buffer_size = 32;
    char bufchars[buffer_size];
    for (int i = 0; i < buffer_size; i++)
        bufchars[i] = 64 - i;
    MPI_Send(bufchars, buffer_size, MPI_CHARACTER, 0, 2, newcomm);
    
    MPI_Finalize();

    printf("Done!\n");

    return 0;
}