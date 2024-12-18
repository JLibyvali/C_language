#ifndef VTABLE__H
#define VTABLE__H

// Hash table, convert function-pointer to hash value and search/insert into hash vtable.
// Glib has hash function, the input key is: pointer value.

// Type
struct Text_base
{
    char *m_title;
    char *m_item;
    // A dispatch function pointer, like CPP Vtable Struct.
    void (*m_print)(struct Text_base *_in);
};

typedef struct Text_base Text_base;
// dispatch function
typedef void (*polymorphism_print_type)(Text_base *);


// print function set
void        print_textbase(Text_base *_in);
void        print_song(Text_base *_in);
void        print_html(Text_base *_in);

// basic oop method. but need a reference counter better.
Text_base  *new_textbase(const char *_title, const char *_item, polymorphism_print_type _fn_ptr);
void        delete_textbase(Text_base *_in);
Text_base  *copy_textbase(Text_base *_in);

#endif