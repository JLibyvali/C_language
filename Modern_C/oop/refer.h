#ifndef REFER__H
#define REFER__H

struct str;
typedef struct str str;

str               *new_str(int _id);
str               *copy_str(str *_in);
str               *shared_str(str *_in);
void               delete_str(str *_in);

#endif