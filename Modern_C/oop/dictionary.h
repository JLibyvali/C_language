#ifndef DICTIONARY__H
#define DICTIONARY__H

#include "keyval.h"

#include <stdio.h>

struct dic
{
    keyval **m_pairs;
    size_t   m_length;
};
typedef struct dic dic;

dic               *new_dic(void);
dic               *copy_dic(dic *_in);
void               free_dic(dic *_in);

void              *dic_find(const dic *_in, const char *_key);
void               dic_add(dic *_in, char *_key, void *_value);

extern void       *m_dic_keynotfound;

#endif