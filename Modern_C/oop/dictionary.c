#include "dictionary.h"

#include "keyval.h"

#include <stdio.h>
#include <stdlib.h>

void *m_dic_keynotfound = NULL;

dic  *new_dic(void)
{
    static int nfound;
    dic       *res = malloc(sizeof(dic));

    if (!m_dic_keynotfound)
        m_dic_keynotfound = &nfound;

    *res = (dic){};

    return res;
}

dic *copy_dic(dic *_in)
{
    dic *res = new_dic();
    for (int i = 0; i < _in->m_length; i++)
    {
    }
}

void free_dic(dic *_in)
{
    for (int i = 0; i < _in->m_length; i++)
    {
        free_keyval(_in->m_pairs[i]);
    }

    free(_in);
}

// Help function to implement dictionary method
static void dic_add_keyval(dic *_in, keyval *_kv)
{
    _in->m_length++;
    _in->m_pairs                    = realloc(_in->m_pairs, sizeof(keyval *) * _in->m_length);
    _in->m_pairs[_in->m_length - 1] = _kv;
}

void dic_add(const dic *_in, const char *_key)
{
    if (!_key)
    {
        fprintf(stderr, "Input NULL pointer of _key.\n");
        abort();
    }

    dic_add(_in, _key);
}