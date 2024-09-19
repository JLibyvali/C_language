//
// Created by jlibyvali on 24-5-14.
//

#ifndef C_FROMK_R_PRAC6_H
#define C_FROMK_R_PRAC6_H

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prac6-2
#define NKEYS (sizeof keytab / sizeof(struct key))

struct key {
  char *word;
  int cnt;
} keytab[] = {
    {"auto", 0},
    {"break", 0},
    {"awas", 0},
    {"pioiu", 0},

};

struct key *binsearch(char *, struct key *, int);

int getword(char *, int);

// Prac 6 - 4
struct tnode {
  char *word;
  int cnt;

  struct tnode *left;
  struct tnode *right;
};

struct tnode *talloc(void);

char *Strdup(char *);

struct tnode *addTree(struct tnode *, char *);

void treeprint(struct tnode *);

// Prac6-6

/**@brief Using hash list to implement the '#define'
 *
 * */

struct nlist {
  struct nlist *next;
  char *name;
  char *defn;
};
struct nlist *hashtab[100];

/**@brief Calculate hash value
 *
 * */
unsigned hash(char *s) {
  unsigned res;

  for (res = 0; *s != '\0'; s++) res = *s + 31 * res;
  return res % 101;
}

/**@brief search name in list
 * */
struct nlist *lookup(char *name) {
  struct nlist *np;

  for (np = hashtab[hash(name)]; np != NULL; np = np->next) {
    if (strcmp(name, np->name) == 0) return np;
  }

  return NULL;
}

struct nlist *install(char *name, char *dfn) {
  struct nlist *np;
  unsigned hashvalue;

  if ((np = lookup(name)) == NULL) {
    if ((np = (struct nlist *)malloc(sizeof(struct nlist))) == NULL ||
        (np->name = Strdup(name)) == NULL) {
      return NULL;
    }

    hashvalue = hash(name);
    np->next = hashtab[hashvalue];
    hashtab[hashvalue] = np;
  } else
    free((void *)np->defn);

  if (((np->defn) = Strdup(dfn)) == NULL) return NULL;

  return np;
}

struct nlist *undef(char *name) {
  struct nlist *p, *res;

  if ((p = lookup(name)) == NULL) {
    return NULL;
  } else {
    if ((res = (struct nlist *)malloc(sizeof(struct nlist))) != NULL) {
      res->name = p->name;
      res->defn = p->defn;
      res->next = NULL;
    } else {
      return NULL;
    }

    free((void *)p->defn);
    free((void *)p->name);
    hashtab[hash(name)] = NULL;

    return res;
  }
}

///////////////////////Prac6-2
int getch(void);

void ungetch(int c);

struct key *binsearch(char *word, struct key *tab, int n) {
  int cond;
  struct key *low = &tab[0];
  struct key *high = &tab[n];
  struct key *mid;

  while (low < high) {             // yes it's two pointer
    mid = low + (high - low) / 2;  // two same array pointer can subtraction,
                                   // calculate the length and divided 2.

    if ((cond = strcmp(word, mid->word)) < 0)
      high = mid;
    else if (cond > 0)
      low = ++mid;
    else
      return mid;
  }

  return NULL;
}

/**@brief read character from input
 * @return first char, EOF or itself(if it's not character).
 *
 * */
int getword(char *word, int lim) {
  int c;
  char *w = word;

  while (isspace(c = getch()));

  if (c != EOF) *w++ = c;

  if (!isalpha(c)) {
    *w = '\0';
    return c;
  }

  for (; --lim > 0; w++) {
    if (!isalnum(*w = getch())) {
      ungetch(*w);
      break;
    }
  }

  *w = '\0';
  return word[0];
}

////////////////////Prac6-4
struct tnode *talloc(void) {
  return (struct tnode *)malloc(sizeof(struct tnode));
}

char *Strdup(char *s) {
  char *p;
  if ((p = (char *)malloc(strlen(s) + 1)) != NULL) {
    strcpy(p, s);
    return p;
  }
}

/**
 * @brief This kind of tree is easily get unbanlanced.
 *
 * */
struct tnode *addTree(struct tnode *p, char *word) {
  int cnt;

  if (p == NULL) {
    p = talloc();
    p->word = Strdup(word);
    p->cnt = 1;
    p->left = p->right = NULL;
  } else if ((cnt = strcmp(word, p->word)) == 0)
    p->cnt++;
  else if (cnt < 0)
    p->left = addTree(p->left, word);
  else
    p->right = addTree(p->right, word);

  return p;
}

void treeprint(struct tnode *p) {
  if (p != NULL) {
    treeprint(p->left);
    printf("%4d %s\n", p->cnt, p->word);
    treeprint(p->right);
  }
}

#endif  // C_FROMK_R_PRAC6_H
