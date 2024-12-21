#include "debug.h"

#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

// Just map a private file
char file_path[] = {"/home/jlibyvali/Developer/projetcs/VSCode/C_language/Modern_C/IPC/memory/data.type"};

int  main()
{
    printf("#################### Test mmap() ########################\n");
    int         ffd = -1;
    struct stat fst;
    long        page_size = sysconf(_SC_PAGESIZE);

    // open file
    ffd                   = open(file_path, O_RDONLY);
    if (ffd == -1)
        error("open()");

    if (fstat(ffd, &fst) == -1)
        error("fstat()");

    void *addr = mmap(NULL, fst.st_size, PROT_READ, MAP_PRIVATE, ffd, 0);
    if (addr == MAP_FAILED)
        error("mmap()");

    printf(FMT("mmaped data: \n", YELLOW));
    fwrite(addr, fst.st_size, 1, stdout);

    printf(FMT("\nunmaped() data: \n", YELLOW));
    munmap(addr, fst.st_size);
    addr = NULL;

    printf(FMT("\nJust mmap() with offset. \n", YELLOW));
    addr = mmap(NULL, fst.st_size, PROT_READ, MAP_PRIVATE, ffd, page_size);
    if (addr == MAP_FAILED)
        error("mmap()");
    fwrite(addr + page_size, fst.st_size, 1, stdout);
    munmap(addr + page_size, fst.st_size);
    addr = NULL;
    close(ffd);
    ffd = -1;

    printf("\n#################### Test Private mmaped visiblity after fork() ########################\n");
    ffd         = open(file_path, O_RDWR);
    addr        = mmap(NULL, fst.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, ffd, 0);

    pid_t child = fork();

    if (child)
    {

        // Make some changes on maped memory.
        close(ffd);
        printf(FMT("Changes on parent process .\n", YELLOW));
        snprintf((char *)(addr + 20), 100, "\nFFFFF Some Changes From Parent FFFFF\n");
        fwrite(addr, fst.st_size, 1, stdout);
        wait(NULL);
        printf(FMT("\nAFter child process finished .\n", YELLOW));
        fwrite(addr, fst.st_size, 1, stdout);
    }
    else
    {
        close(ffd);
        printf(FMT("Changes on Child process .\n", YELLOW));
        snprintf((char *)(addr + 20), 100, "\nXXXXXX Changes From Child Process .XXXXXXXX\n");
        fwrite(addr, fst.st_size, 1, stdout);
        munmap(addr, fst.st_size);
        _exit(0);
    }
    
    return 0;
}