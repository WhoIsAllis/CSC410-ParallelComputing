// Convert sequential sums to parallel using processes and pipes.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_PROCESSES 4

int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 3) 
    {
        fprintf(stderr, "Usage: %s <N> [processes]\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int processes = (argc == 3) ? atoi(argv[2]) : NUM_PROCESSES;

    if (N <= 0 || processes < 1 || processes > NUM_PROCESSES) 
    {
        fprintf(stderr, "N must be positive and processes must be between 1 and %d\n", NUM_PROCESSES);
        return 1;
    }

    int *arr = malloc((size_t) N * sizeof(int));
    if (!arr) 
    {
        perror("malloc");
        return 1;
    }

    for (int i = 0; i < N; i++) 
    {
        arr[i] = i + 1;
    }

    int pipes[NUM_PROCESSES][2];
    int children = 0;
    int error = 0;

    // Create one pipe for each child before creating any child processes.
    for (int i = 0; i < processes; i++) 
    {
        if (pipe(pipes[i]) < 0) 
        {
            perror("pipe");
            for (int j = 0; j < i; j++) 
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            free(arr);
            return 1;
        }
    }

    // Create children. Each child sums one approximately equal chunk.
    for (int i = 0; i < processes; i++) 
    {
        pid_t pid = fork();

        if (pid < 0) 
        {
            perror("fork");
            error = 1;
            break;
        }

        if (pid == 0) 
        {
            long long partial_sum = 0;
            int start = i * N / processes;
            int end = (i + 1) * N / processes;

            // A child only writes to its own pipe.
            for (int j = 0; j < processes; j++) 
            {
                close(pipes[j][0]);
                if (j != i) 
                {
                    close(pipes[j][1]);
                }
            }

            for (int j = start; j < end; j++) 
            {
                partial_sum += arr[j];
            }

            if (write(pipes[i][1], &partial_sum, sizeof(partial_sum)) != sizeof(partial_sum)) 
            {
                perror("write");
                close(pipes[i][1]);
                _exit(1);
            }

            close(pipes[i][1]);
            _exit(0);
        }

        children++;
    }

    // The parent only reads from the pipes.
    for (int i = 0; i < processes; i++) 
    {
        close(pipes[i][1]);
    }

    // Wait for every child before collecting its partial sum.
    for (int i = 0; i < children; i++) 
    {
        if (wait(NULL) < 0) 
        {
            perror("wait");
            error = 1;
        }
    }

    long long total = 0;
    for (int i = 0; i < children; i++) 
    {
        long long partial_sum;

        if (read(pipes[i][0], &partial_sum, sizeof(partial_sum)) != sizeof(partial_sum)) 
        {
            perror("read");
            error = 1;
        } else 
        {
            total += partial_sum;
        }

        close(pipes[i][0]);
    }

    // Close read ends that belong to children that were not created.
    for (int i = children; i < processes; i++) 
    {
        close(pipes[i][0]);
    }

    if (!error) 
    {
        printf("Total Sum = %lld\n", total);
    }

    free(arr);
    return error;
}
