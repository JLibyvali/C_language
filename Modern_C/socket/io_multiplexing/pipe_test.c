#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUF_LEN 128

int main()
{
    int   pipe0[2];

    char  msg1[]       = "Hi it's stirng ONE.\n";
    char  msg2[]       = "Got it it's string TWO.\n";
    char  buf[BUF_LEN] = {};

    pid_t pid;
    // Create 2 pipes for data transmission.
    pipe(pipe0);

    pid = fork();

    // In child process
    if (pid == 0)
    {
        int   ret = write(pipe0[1], msg1, sizeof(msg1));
        pid_t id  = getpid();
        sleep(3);

        read(pipe0[0], buf, BUF_LEN);
        printf("[%d] Child process output: %s \n", id, buf);
        return 0;
    }

    // In parent process
    int   ret = read(pipe0[0], buf, BUF_LEN);
    pid_t id  = getpid();

    printf("[%d] Parent process output: %s \n", id, buf);
    write(pipe0[1], msg2, sizeof(msg2));
    
    sleep(4);

    return 0;
}
