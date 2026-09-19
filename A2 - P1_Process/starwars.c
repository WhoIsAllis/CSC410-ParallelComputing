#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_CHARACTERS 4

// Starting shield power level
int shield_power = 50;

int main(void)
{
    const char *characters[] = {"Luke", "Han", "Chewbacca", "Leia"};
    const int adjustments[] = {25, 20, 30, 15};
    int children = 0;
    int error = 0;

    printf("Millennium Falcon: Initial shield power level: %d%%\n\n", shield_power);
    fflush(stdout);

    // Create 4 child processes - 4 different characters adjusting shield power
    for (int i = 0; i < NUM_CHARACTERS; i++)
    {
        pid_t pid = fork();

        // Check if process creation failed
        if (pid < 0)
        {
            perror("fork");
            error = 1;
            break;
        }

        if (pid == 0)
        {
            printf("%s: Adjusting shield power.\n", characters[i]);
            shield_power += adjustments[i];
            printf("%s: Shield power level now at %d%%\n.", characters[i], shield_power);
            return 0;
        }

        children++;
    }

    // Make parent process wait for all child processes to complete
    for (int i = 0; i < children; i++)
    {
        if (wait(NULL) < 0)
        {
            perror("wait");
            error = 1;
        }
    }


    // Parent process reports final state
    printf("\nFinal shield power level on the Millennium Falcon: %d%%\n", shield_power);
    printf("\nMay the forks be with you!\n");
    return error;
}
