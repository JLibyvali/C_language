#include "keyval.h"
#include <stdlib.h>
#include <strings.h>

keyval *new_keyval(char *_key, void *_val) {
  keyval *res = malloc(sizeof(keyval));
  *res = (keyval){.m_key = _key, .m_value = _val};
  return res;
}
keyval *copy_keyval(const keyval *_in) {
  keyval *res = malloc(sizeof(keyval));
  res = _in;
  return res;
}
void free_keyval(keyval *_in) { free(_in); }

int is_match(const keyval *_src, const char *_key) {
  return (!strcasecmp(_key, _src->m_key));
}