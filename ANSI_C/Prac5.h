#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

//Prac 5-7
#define  MAXLINE 1000

int readlines(char *lineptr[], int maxlines);    // read input lines, storage in 'lineptr[lineNum]'
void writelines(char *lineptr[], int nlines);    // output lines
void Qsort(char *v[], int left, int right);    // Quick sort
void swap(char *v[], int i, int j);

int Getline(char *s, int limit);   // read one line once, return length

// Prac 5-10
int *STACK2;    // calculate expression using stack
int *HEAD;

void push2(int c);

int pop2(void);

void printStack(void);

int cal(int *s, int len);


// Prac 5-11
void Qsort2(void *v[], int left, int right, int (*comp)(void *, void *));

void swap2(void *v[], int left, int right);

int numcmp(char *, char *);

///////////////////////////Prac 5-7
char *alloc(int len) {
    return ((char *) malloc(len));
}

int Getline(char *s, int limit) {

    int c, i;
    i = 0;

    while (--limit > 0 && (c = getchar()) != EOF && c != '\n')
        s[i++] = c;
    if (c == '\n')
        s[i++] = c; // read in '\n'
    s[i] = '\0';
    return i;
}

int readlines(char *lineptr[], int maxlines) {
    int len, nlines;
    char *p, line[MAXLINE];

    nlines = 0;
    while ((len = Getline(line, MAXLINE)) > 0)
        if (nlines >= maxlines || (p = alloc(len)) == NULL)
            return -1;
        else {
            line[len - 1] = '\0'; //delete new line
            strcpy(p, line);
            lineptr[nlines++] = p;
        }
    return nlines;
}

void writelines(char *lineptr[], int nlines) {
    int i;
    printf("Output:\n");
    for (i = 0; i < nlines; i++) {
        printf("%s\n", *lineptr++);
    }

}

void swap(char *v[], int i, int j) {
    char *tmp;
    tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
}


void Qsort(char *v[], int left, int right) {

    int i, last;
    void swap(char *v[], int i, int j);

    if (left >= right)
        return;

    swap(v, left, right);
    last = left;

    for (i = left + 1; i <= right; i++) {
        if (strcmp(v[i], v[left]) < 0)
            swap(v, ++last, i);
    }

    swap(v, left, last);
    Qsort(v, left, last - 1);
    Qsort(v, last + 1, right);
}

/////////////////////////Prac 5-10

void push2(int c) {
    *STACK2++ = c;
}

int pop2(void) {
    if (*--STACK2 != '-')
        return (*STACK2);
    else
        return '?';
}

void printStack(void) {
    int *start = HEAD, *cur = STACK2;

    if (*--cur == '-') {
        printf("Stack is empty\n");
    } else {
        while (start != STACK2) {
            printf("%d\n", *start++);
        }
    }
}

int cal(int *s, int len) {
    STACK2 = (int *) malloc(2000 * sizeof(int));    // malloc heap for stack
    if (STACK2 == NULL) {
        printf("Malloc failed \n");
        return -1;
    }
    memset(STACK2, 0, 1000);
    HEAD = STACK2;

    int i, tmp;
    for (i = 0; i < len; i++) {
        if (isdigit(s[i])) {
            push2(s[i] - 48);   // push the num into stack
            printStack();
        } else {
            switch (s[i]) { // ascii num for "+-*/"
                case 43:
                    push2(pop2() + pop2());
                    printStack();
                    break;
                case 45:
                    tmp = pop2();
                    push2(pop2() - tmp);
                    break;
                case 42:
                    push2(pop2() * pop2());
                    printStack();
                    break;
                case 47:
                    tmp = pop2();
                    if (tmp != 0) {
                        push2(pop2() / tmp);
                        break;
                    } else {
                        printf("Error: zero divisor\n");
                    }
                    break;
                default:
                    printf("Error: unknown command\n");
                    break;
            }
        }

    }
    int ans = (*--STACK2);
    free(HEAD);

    return ans;


}


//////////////////////Prac 5-11
void Qsort2(void *v[], int left, int right, int (*comp)(void *, void *)) {  // using the void* to handle any type data
    int i, last;
    if (left >= right)
        return;

    swap2(v, left, right);

    last = left;
    for (i = left + 1; i <= right; ++i) {
        if ((*comp)(v[i], v[left]) < 0) // 'comp' is passed function
            swap2(v, ++last, i);
    }

    swap2(v, left, last);
    Qsort2(v, left, last - 1, comp);
    Qsort2(v, last + 1, right, comp);

}

void swap2(void *v[], int left, int right) {
    void *tmp;
    tmp = v[left];
    v[left] = v[right];
    v[right] = tmp;
}

int numcmp(char * s1, char * s2){

    double  v1,v2;
    v1 = atof(s1);
    v2 = atof(s2);

    if(v1 < v2)
        return -1;
    else if (v1 > v2)
        return 1;
    else
        return 0;
}
