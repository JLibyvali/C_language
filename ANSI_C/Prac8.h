#include "syscall.h"
#include <fcntl.h>
#include <unistd.h>

#define PERMS 0666 // define file privilege

// Prac8.5 Implement fopen,fflush,fseek, and malloc , free

/*
    Definition
 */

/* file operation
 */
#define NULL     0
#define EOF      (-1)
#define BUFSIZE  1024
#define OPEN_MAX 20

typedef struct _iobuf { // FILE struct
    int cnt;            // characters left
    char *ptr;          // next character position
    char *base;         // location of buffer
    int flag;           // mode of file access
    int fd;             // file descriptor
} FILE;

// For storaging FILE descriptor
extern FILE _iob[OPEN_MAX]; // name beginning with '_' meaning just used for standard library
#define stdin  (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2])

enum _flags {
    _READ  = 01,
    _WRITE = 02,
    _UNBUF = 04, // file is unbuffered
    _EOF   = 010,
    _ERR   = 020,
};

int _fillbuf(FILE *);
int _flushbuf(int, FILE *);
FILE *fopen(char *, char *);
enum _flags fclose(FILE *);
int fseek(FILE *, long, int);

/*malloc and free
 */
/*
size: truly size allocated
pointer to next: struct s  ptr
pointer return to user:

|-----------------------|------|--------------------|
| point to next block   | size |@@@@@@@@@@@@@@@@@@@@|
|-----------------------|------|--------------------|
        |                      |                       
        |                  pointer returnd to user
        |
    Header pointer to next
 */

typedef long Align;
union header { // describe the block in list
    struct {
        union header *ptr;
        unsigned size;
    } s;
    Align x; // will not use, just for alignment
};
typedef union header Header;

static Header base;          // empty list to get started, to find a place in head
static Header *freep = NULL; // start of free list
void *malloc(unsigned nbytes);
void free(void *ap);

/*
   Implementation
 */

/*  Malloc and Free
 */

/**
 * @brief morecore is used for ask memory from system, case it cost a lot.
 We don't wanna it invoked too many
 * @return Header*
 */
static Header *morecore(unsigned);

void *malloc(unsigned nbytes) {
    Header *p, *prevp;
    unsigned nunits;
    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;

    if ((prevp = freep) == NULL) { // no free list yet
        base.s.ptr = freep = prevp = &base;
        base.s.size                = 0;
    } // initialize one null free-block list, point to itself

    for (p = prevp->s.ptr;; prevp = p, p = p->s.ptr) {
            sfreep(freep);
        if (p->s.size >= nunits) { // big enough

            if (p->s.size == nunits) // if just fit
                prevp->s.ptr = p->s.ptr;
            else {
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }

            freep = prevp;
            return (void *)(p + 1); // truly size one bigger than num
        }
        if (p == freep)
            if ((p = morecore(nunits)) == NULL) // after set field"size", morecore free other memory
                return NULL;
    }
}

#define NALLOC 1024
static Header *morecore(unsigned nu) {

    char *cp;
    Header *up;

    if (nu < NALLOC)
        nu = NALLOC;

    cp = (char *)sbrk(nu * sizeof(Header)); // if sucess, sbrk() return void* address
    if (cp == (char *)-1)
        return NULL;

    up = (Header *)cp;

    up->s.size = nu;
    free((void *)(up + 1));
    return freep;
}

/**
 * @brief   free unoccupied block, try to merge them, and make update the free-block pointer, using that merged bock for user
 * @param ap 
freep == p                                                                                               (p->s.ptr)                                                                                                                     
|                                                                                                         |
|-----------------------|------|--------------------||-----------------------|------|--------------------||-----------------------|------|--------------------||-----------------------|------|--------------------|
| point to next block   | size |@@@@@@@@@@@@@@@@@@@@|| point to next block   | size |@@@@@@@@@@@@@@@@@@@@|| point to next block   | size |@@@@@@@@@@@@@@@@@@@@|| point to next block   | size |@@@@@@@@@@@@@@@@@@@@|
|-----------------------|------|--------------------||-----------------------|------|--------------------||-----------------------|------|--------------------||-----------------------|------|--------------------|
        |                      |                                                    |                        
        |                  pointer returnd to user                                  |
        |                                                                           |
    Header pointer to next                                                          bp
 */
void free(void *ap) {

    Header *bp, *p;

    bp = (Header *)ap - 1; // point to block that need free

    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr) { // traverse the free-block list, make sure that 'bp' insert at start/end
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr)) 
            break;  // find one block between 
    }

    // depend p at start/end
    if (bp + bp->s.size == p->s.ptr) { // try join to upper neighbor-block
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    } else {
        bp->s.ptr = p->s.ptr;
    }
    
    if (p + p->s.size == bp) { // try join to lower neighbor-block
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    } else {
        p->s.ptr = bp;
    }
    
    freep = p;
}



/* File operation
 */
int _fillbuf(FILE *fp) {

    int bufsize;
    if ((fp->flag & (_READ | _EOF | _ERR)) != _READ)
        return EOF;

    bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZE;

    if (fp->base == NULL)                                 // no buffer yet
        if ((fp->base = (char *)malloc(bufsize)) == NULL) // fp->base now point to memory buffer
            return EOF;

    fp->ptr = fp->base;                       // next character postion is the buffer
    fp->cnt = read(fp->fd, fp->ptr, bufsize); // systemcall

    if (--fp->cnt < 0) { // if read file failure
        if (fp->cnt == -1)
            fp->flag |= _EOF;
        else
            fp->flag |= _ERR;

        fp->cnt = 0;
        return EOF;
    }
    return (unsigned char)*fp->ptr++; // return first character
}

FILE _iob[OPEN_MAX] = {
    // stdin, stdout, stderr initialize:cnt, ptr,base,flag,fd
    {0, (char *)0, (char *)0, _READ, 0},
    {0, (char *)0, (char *)0, _WRITE, 1},
    {0, (char *)0, (char *)0, _WRITE | _UNBUF, 2} // stderr no buffer
};

FILE *fopen(char *name, char *mode) {

    int fd;
    FILE *fp;

    if (*mode != 'r' && *mode != 'w' && *mode != 'a') // only three mode
        return NULL;

    for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
        if ((fp->flag & (_READ | _WRITE)) == 0)
            break; // found free io buffer

    if (fp >= _iob + OPEN_MAX)
        return NULL;

    if (*mode == 'w')
        fd = creat(name, PERMS); // create file with privilege
    else if (*mode == 'a') {
        if ((fd = open(name, O_WRONLY, 0)) == -1)
            fd = creat(name, PERMS); // if file not existed
        lseek(fd, 0L, 2);            // go to position
    } else {
        fd = open(name, O_RDONLY, 0); // *mode == 'r', read only
    }

    if (fd == -1) // if 'fd' above excepted
        return NULL;

    fp->fd   = fd;
    fp->cnt  = 0;
    fp->base = NULL;
    fp->flag = (*mode == 'r') ? _READ : _WRITE;

    return fp;
}

