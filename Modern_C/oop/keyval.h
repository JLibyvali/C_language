#ifndef KEYVAL__H
#define KEYVAL__H

// define a key-val oop class
struct keyval {
  char *m_key;
  void *m_value;
};
typedef struct keyval keyval;
// Emulate class ctor about
keyval *new_keyval(char *_key, void *_val);
keyval *copy_keyval(const keyval *_in);
void free_keyval(keyval *_in);

// like class data method
int is_match(const keyval *_src, const char *_key);

#endif