#ifndef PRIVATE__H
#define PRIVATE__H

struct box
{
    int   m_public;
    void *m_private_ptr;
};

typedef struct box box;

box               *new_box(int _id);
void               delete_box(box *_in);
void               print_box(box *_in);

#endif