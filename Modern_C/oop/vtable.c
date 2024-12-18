#include "vtable.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ######################################################## Hash Table #######################
//  Using hash value to search correct function.
#define TABLE_CAPACITY 128

struct Hash_item
{
    unsigned int            m_key;
    polymorphism_print_type m_fn;
};

struct HashTable_pointer
{
    struct Hash_item **m_table;
    unsigned int       m_capacity;
    unsigned int       m_size;
};

typedef struct HashTable_pointer HashTable_pointer;

// Copy from Glib hash function.
static unsigned int              pointer_hash(const void **_ptr)
{
    unsigned int res = 5381;

    void        *temp;
    while ((temp = *_ptr++))
        res = res * 33 + (uintptr_t)(temp);

    return res % (TABLE_CAPACITY - 1);
}

// Hash Table OOP function
static HashTable_pointer *print_fn_table = NULL;

static struct Hash_item  *create_item(unsigned int _key, polymorphism_print_type v_fn)
{
    struct Hash_item *new_item = malloc(sizeof(struct Hash_item));
    if (!new_item)
        return NULL;

    new_item->m_key = _key;
    new_item->m_fn  = v_fn;

    return new_item;
}

static void create_table(int _size)
{
    if (_size <= 0)
        return;

    // create table
    HashTable_pointer *table = malloc(sizeof(HashTable_pointer));
    if (!table)
        return;

    print_fn_table             = table;
    // init table
    print_fn_table->m_size     = 0;
    print_fn_table->m_capacity = _size;
    print_fn_table->m_table    = malloc(_size * sizeof(struct Hash_item *));
    if (!print_fn_table->m_table)
        return;

    for (int i = 0; i < print_fn_table->m_capacity; i++)
        print_fn_table->m_table[i] = NULL;

    return;
}

static void delete_table()
{
    for (int i = 0; i < print_fn_table->m_capacity; i++)
    {
        // free every table item
        if (print_fn_table->m_table[i] != NULL)
            free(print_fn_table->m_table[i]);
    }

    // free table
    free(print_fn_table->m_table);
}

static void insert_fn_table(Text_base *_key, polymorphism_print_type v_fn)
{
    if (print_fn_table->m_table == NULL)
        return;

    // Check if item existed
    unsigned int index = pointer_hash(&_key->m_print);
    if (print_fn_table->m_table[index])
        return;

    // IF null, create new table item
    struct Hash_item *temp = create_item(index, v_fn);
    if (!temp)
        return;

    print_fn_table->m_table[index] = temp;

    if (print_fn_table->m_size < print_fn_table->m_capacity)
        print_fn_table->m_size++;
    else
        print_fn_table->m_size = print_fn_table->m_capacity;
}

static polymorphism_print_type lookfor_fn_table(Text_base *_key)
{
    unsigned int      hash = pointer_hash(&_key->m_print);
    struct Hash_item *temp = print_fn_table->m_table[hash];

    if (!temp)
        return NULL;

    return temp->m_fn;
}

// ######################################################## Function Type Verify #######################
//  Type Check
//  For runtime get type information, and Ensure that in the hash table has correct type function.
void fn_type_check(polymorphism_print_type _fn) { return; }

#define hashtable_init(object_ptr, fn)    \
    {                                     \
        fn_type_check(fn);                \
        if (!print_fn_table)              \
        {                                 \
            create_table(TABLE_CAPACITY); \
        }                                 \
        insert_fn_table(object_ptr, fn);  \
    }

// ######################################################## Function Set #######################
// default print function, connecting with hash table
void print_textbase(Text_base *_in)
{
    if (!print_fn_table)
    {
        hashtable_init(_in, _in->m_print);
        polymorphism_print_type fn = lookfor_fn_table(_in);
        fn(_in);
        return;
    }

    // Look  for in hashtable
    if (_in->m_print == NULL)
    {
        printf("======= Default Print End ======\n");
        printf("No other function found in table.\n");
    }
    else
    {
        polymorphism_print_type fn = lookfor_fn_table(_in);
        if (fn)
        {
            fn(_in);
        }
        else
        {
            insert_fn_table(_in, _in->m_print);
            polymorphism_print_type fn = lookfor_fn_table(_in);
            fn(_in);
        }
    }

    return;
}

void print_song(Text_base *_in)
{
    printf("\n----- Song -----\n");
    printf("Title: %s \n", _in->m_title);
    printf("Song text: %s \n------- Finish Song. \n", _in->m_item);
}

void print_html(Text_base *_in)
{
    printf("<HTML>------ Title -----<HTML>\n");
    printf("HTML == <%s>\n", _in->m_title);
    printf("<HTML>: %s :<HTML>\n", _in->m_item);
}

// ############################################# OOP Function ##############################################3//
Text_base *new_textbase(const char *_title, const char *_item, polymorphism_print_type _fn_ptr)
{

    Text_base *instance = (Text_base *)malloc(sizeof(Text_base));
    if (_fn_ptr)
    {
        *instance = (Text_base){.m_title = malloc(1024), .m_item = malloc(1024), .m_print = _fn_ptr};
        strcpy(instance->m_title, _title);
        strcpy(instance->m_item, _item);
    }
    else
    {
        *instance = (Text_base){.m_title = malloc(1024), .m_item = malloc(1024), .m_print = NULL};
        strcpy(instance->m_title, _title);
        strcpy(instance->m_item, _item);
    }

    return instance;
}

void delete_textbase(Text_base *_in)
{
    if (!_in)
        return;

    free(_in->m_item);
    free(_in->m_title);
    free(_in);
}

// deep copy
Text_base *copy_textbase(Text_base *_in)
{
    if (!_in)
        return NULL;

    Text_base *res = malloc(sizeof(Text_base));
    res->m_item    = malloc(1024);
    res->m_title   = malloc(1024);

    memcpy(res->m_title, _in->m_title, 1024);
    memcpy(res->m_item, _in->m_item, 1024);
    res->m_print = _in->m_print;

    return res;
}
