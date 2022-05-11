#include "mpi.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int initialized;
    MPI_Initialized(&initialized);
    if(!initialized)
    {
        printf("Error on MPI_Initialized\n"); fflush(stdout);
        return 1;
    }

    int size_mpi;
    MPI_Comm_size(MPI_COMM_WORLD, &size_mpi);
    // Number of MPI processes in for all MPI interfaces should be always 1
    if (size_mpi != 1)
    {
        printf("MPI size != 1\n"); fflush(stdout);
        return 1;
    }

    char port_name[MPI_MAX_PORT_NAME];
    MPI_Open_port(MPI_INFO_NULL, port_name);
    // establishes a port at which the server may be contacted.
    printf("port_name = %s\n", port_name); fflush(stdout);

    char terachem_port_name[1024];
    if (argc != 2)
    {
        printf("Need a command line argument as terachem_port_name\n"); fflush(stdout);
        return 1;
    }
    strcpy(terachem_port_name, argv[1]);
    MPI_Publish_name(terachem_port_name, MPI_INFO_NULL, port_name);
    printf("terachem_port_name = %s\n", terachem_port_name); fflush(stdout);

    printf("Waiting to accept MPI communication from MPI client.\n"); fflush(stdout);
    
    MPI_Comm client;
    MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &client);
    printf("MPI communication accepted.\n");
    MPI_Unpublish_name(terachem_port_name, MPI_INFO_NULL, port_name);
    printf("client = %d\n", client); fflush(stdout);

    MPI_Status mpi_status;
    int buffer_size = 32;
    char bufchars[buffer_size];
    MPI_Recv(bufchars, buffer_size, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, client, &mpi_status);
    const int MPI_TAG_ERROR = 13;
    if (mpi_status.MPI_TAG == MPI_TAG_ERROR)
    {
        printf("Error on MPI_Recv\n"); fflush(stdout);
        return 1;
    }

    for (int i = 0; i < buffer_size; i++)
        printf("%d ", bufchars[i]);
    printf("\n"); fflush(stdout);
    
    MPI_Comm_free(&client);
    MPI_Close_port(port_name);
    MPI_Finalize();

    printf("Done!\n");

    return 0;
}