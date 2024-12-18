#include "private.h"

#include "vtable.h"

#include <stdio.h>
#include <stdlib.h>

struct box_private
{
    int   m_size;
    char *m_msg;
};

box *new_box(int _id)
{
    // allocate memory
    box *res                    = malloc(sizeof(box));
    *res                        = (box){.m_public = _id, .m_private_ptr = malloc(sizeof(struct box_private))};

    struct box_private *private = res->m_private_ptr;
    private->m_size             = 33;
    private->m_msg              = malloc(100);
    private->m_msg              = "Hello";

    return res;
}

void delete_box(box *_in)
{
    if (!_in)
        return;

    free(_in->m_private_ptr);

    free(_in);
}

void print_box(box *_in)
{
    struct box_private *ptr = _in->m_private_ptr;

    printf("ID %d have  %d Message: %s \n", _in->m_public, ptr->m_size, ptr->m_msg);
}
