#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_LEN(arr) (int)(sizeof((arr)) / sizeof((arr)[0]))

enum {
  TK_NOTYPE = 256, // space
  TK_EQ,
  TK_LPARENT,
  TK_RPARENT,
  TK_MUL,
  TK_DIV,
  TK_PLUS,
  TK_SUB,
  TK_NUM

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {{"\\(", TK_LPARENT}, // (
             {"\\)", TK_RPARENT}, // )
             {" +", TK_NOTYPE},   // spaces
             {"\\*", TK_MUL},     // multiplication.
             {"/", TK_DIV},       // division.
             {"\\+", TK_PLUS},    // plus
             {"-", TK_SUB},       // subtraction.
             {"==", TK_EQ},       // equal
             {"[0-9]{1,31}", TK_NUM}};

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] = {};
static int nr_token = 0;

int main() {
  char *input = "124134141*(1441-4234)+1311 / 231231 ==";
  int len = ARRAY_LEN(rules);

  regex_t regex[len] = {};
  regmatch_t pmatch;

  int retcode, i = 0;
  char error[256];

  char *temp = input;
  while (i < len) {
    retcode = regcomp(&regex[i], rules[i].regex, REG_EXTENDED);

    if (retcode) {
      regerror(retcode, &regex[i], error, 256);
      printf("Error regex:%s,message:%s", rules[i].regex, error);
      return -1;
    }

    retcode = regexec(&regex[i], temp, 1, &pmatch, 0);
    if (!retcode) {

      int off = pmatch.rm_eo - pmatch.rm_so;
      printf("Substr:\"%.*s\".\n", off, temp + pmatch.rm_so);
    }

    temp = input + len;

    i++;
    regfree(&regex[i]);
    // printf("TEST i:%d, len:%d\n", i, len);
  }

  return 0;
}
