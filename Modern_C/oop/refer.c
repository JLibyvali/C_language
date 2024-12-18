#include "refer.h"

#include "debug.h"

#include <complex.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct str
{
    int           m_id;
    char         *m_msg;

    unsigned int *ref;
};

str *new_str(int _id)
{
    str *instance = malloc(sizeof(str));
    if (!instance)
        return NULL;

    *instance      = (str){.m_id = _id, .m_msg = malloc(1024), .ref = malloc(sizeof(unsigned int))};
    *instance->ref = 1;

    return instance;
}

str *copy_str(str *_in)
{
    if (!_in)
        return NULL;

    str *res   = malloc(sizeof(str));

    res->m_id  = _in->m_id;
    res->m_msg = malloc(1024);
    strcpy(res->m_msg, _in->m_msg);

    res->ref  = malloc(sizeof(unsigned int));
    *res->ref = 1;

    return res;
}

str *shared_str(str *_in)
{
    if (!_in)
        return NULL;

    // shallow copy
    str *res = malloc(sizeof(str));
    *res     = *_in;
    (*res->ref)++;

    return res;
}

void delete_str(str *_in)
{
    if (!_in)
        return;

    (*_in->ref)--;

    if (!(*_in->ref))
    {
        free(_in->m_msg);
        free(_in->ref);
    }

    free(_in);
}

int main(int argc, char *argv[])
{

    if (argc > 1 && strcmp(argv[1], "--valgrind") == 0)
    {
        printf(FMT("######################## Test Reference Count #######################\n", YELLOW));

        str *ins1 = new_str(11);
        strcpy(ins1->m_msg, "Hello Instance One");
        str *ins2 = new_str(22);
        strcpy(ins2->m_msg, "The INsatance Two.");

        printf("Test Deep copy.\n");
        str *ins1_cpy = copy_str(ins1);
        printf(
            "Ins1: \"%s\" cnt %d;\t Ins1_cpy: \"%s\" cnt %d .\n", ins1->m_msg, *ins1->ref, ins1_cpy->m_msg,
            *ins1_cpy->ref
        );

        printf("Test Shallow copy.\n");
        str *ins2_cpy = shared_str(ins2);

        printf(
            "Ins2: \"%s\" cnt %d;\t Ins2_cpy: \"%s\" cnt %d .\n", ins2->m_msg, *ins2->ref, ins2_cpy->m_msg,
            *ins2_cpy->ref
        );

        delete_str(ins1);
        delete_str(ins1_cpy);
        delete_str(ins2);
        printf("After deleted shared: Ins2_cpy: \"%s\" cnt %d .\n", ins2_cpy->m_msg, *ins2_cpy->ref);
        delete_str(ins2_cpy);

        return 0;
    }

    printf(FMT("--------------- Valgrind Test Reference Count Memory leak -------------\n", YELLOW));
    printf("Launch valgrind \n");

    FILE *res;
    char  cmd[150];
    snprintf(cmd, 150, "valgrind --leak-check=summary %s --valgrind \n", argv[0]);

    res = popen(cmd, "r");
    if (!res)
    {
        perror("popen");
        return -1;
    }

    char result[4096];
    while (fgets(result, 4096, res))
    {
        printf("%s", result);
    }

    printf("%s", result);

    fclose(res);
    return 0;
}