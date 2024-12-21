#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

// print all mmap pages information
void displaymincore(void *_addr, int len)
{
    unsigned char *vec;

    long           pagesize  = sysconf(_SC_PAGESIZE);
    int            page_nums = (len + pagesize - 1) / pagesize;
    vec                      = malloc(page_nums);

    if (vec == NULL)
        return;

    if (mincore(_addr, len, vec) == -1)
        return;

    for (int i = 0; i < page_nums; i++)
    {
        if (i % 64 == 0)
            printf("%s%10p: ", (i == 0) ? "" : "\n", _addr + (i * pagesize));

        if (i % 8 == 0)
            printf("  ");

        printf("%c", (vec[i] & 1) ? 'L' : 'U');
    }
    printf("\n");
}

int main()
{

    long  pagesize  = sysconf(_SC_PAGESIZE);
    int   all_pages = 32 * pagesize, group_pages = 8 * pagesize, lock_step = 3 * pagesize;

    char *addr = mmap(NULL, all_pages, PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED)
        error("mmap()");

    printf(FMT("Before mlock() \n", YELLOW));
    displaymincore(addr, all_pages);

    // lock pages
    for (int i = 0; i + lock_step < all_pages; i += group_pages)
    {
        if (mlock(addr + i, lock_step) == -1)
            error("mlock()");
    }

    printf(FMT("After mlock() \n", YELLOW));
    displaymincore(addr, all_pages);

    printf(
        FMT("\"U\" means page not reserve in RAM, \"L\" means page reserve in RAM. \nBefore mlock show that all "
            "un-reserve in RAM case `lazy swap reservation` policy, which only allocate page when it really need.",
            YELLOW)
    );

    return 0;
}