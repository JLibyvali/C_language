#include "debug.h"

#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

// posix_madvise()/madvise() is used to give advice or directions to kernel about the address ranges of another
// process or of the calling process. To improve performance
void display(void *_addr, size_t _len)
{
    long          pagesize  = sysconf(_SC_PAGESIZE);
    size_t        page_nums = (_len + pagesize - 1) / pagesize;
    unsigned char page_info[page_nums];

    if (mincore(_addr, _len, page_info) == -1)
        return;

    printf("In page division unit, Addr %10p: ", _addr);
    for (int i = 0; i < page_nums; i++)
    {
        if (i % 32 == 0)
            printf("\n");
        if (i % 8 == 0)
            printf("  ");

        printf("%c", (page_info[i] & 1) ? 'Y' : 'N');
    }

    printf("\n");
}

int main()
{
    long   pagesize = sysconf(_SC_PAGESIZE);
    size_t allbytes = 128 * pagesize;

    char  *addr     = mmap(NULL, allbytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED)
        error("mmap()");

    printf(FMT("############ TEST MADV_DONTNEED ############# After used, pages will be discarded \n", YELLOW));
    posix_madvise(addr, allbytes, POSIX_MADV_DONTNEED);

    display(addr, allbytes);
    snprintf(addr, 128, "Hello World It's first use of pages.\n");
    snprintf(addr + (4 * pagesize), 128, "Use another Pages.\n");
    display(addr, allbytes);

    fprintf(stdout, "%*.s", 128, addr);

    return 0;
}