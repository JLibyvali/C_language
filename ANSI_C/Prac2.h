#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// Prac 2-3
int Htoi(const char *s, const int len);

// Prac 2-6
char *setbits(int x, int p, int n, int y);

// Prac 2-7
char *invert(int x, int p, int n);

// Prac 2-8
char *rightrot(int x, int n);


// Prac 4-4
int getch(void);

void ungetch(int);

void push(double f);

int getop(char []);

double pop(void);

void print(void);

double copy(void);

void exchange(void);

void clear(void);


////////////////////////////////////2-3
int Htoi(const char *s, const int len) {

    int i, j;
    double n = 0;

    for (i = len - 1, j = 0; i >= 0; i--, j++) {
        if (s[i] == 'x' || s[i] == 'X')
            break;
        if (isdigit(s[i]))
            n += pow(16, j) * (s[i] - '0');
        if (isalpha(s[i]))
            n += pow(16, j) * (((isupper(s[i])) ? s[i] - 'A' : s[i] - 'a') + 10);
    }
    return (int) n;
}

////////////////////////////////2-6
char *setbits(int x, int p, int n, int y) {

    int i, j;
    static char bitsx[32], bitsy[32];
    for (i = 0; i < 32; i++) {
        bitsx[i] = (x & 1) ? '1' : '0';
        x >>= 1;
    }

    for (i = 0; i < 32; i++) {
        bitsy[i] = (y & 1) ? '1' : '0';
        y >>= 1;
    }

    for (i = p, j = 0; j < n; i++, j++) {
        bitsx[i] = bitsy[j];
    }

    return bitsx;
}


////////////////////////////////////////////2-7
char *invert(int x, int p, int n) {

    int i, j;
    static char bitsx[32];
    for (i = 0; i < 32; i++) {
        bitsx[i] = (x & 1) ? '1' : '0';
        x >>= 1;
    }

    for (i = p, j = 0; j < n; i++, j++) {
        bitsx[i] = ((bitsx[i] - '0') & 1) ? '0' : '1';
    }

    return bitsx;
}


/////////////////////////////////////////2-8
char *rightrot(int x, int n) {

    int i, j, len = 0;
    static char bitsx[32];
    static char res[32];

    for (i = 0; i < 32; i++) {
        bitsx[i] = (x & 1) ? '1' : '0';
        x >>= 1;
        len++;
    }

    for (i = 0, j = len; i < n; i++, j++) {
        bitsx[j] = bitsx[i];
    }

    for (i = 0, j = n; i < len; i++, j++) {
        res[i] = bitsx[j];
    }

    return res;
}

//////////////////////////////////////4-4
static int sp = 0;    // stack position
static double STACK[200];
char buf[200];
int bufp = 0;

int getch(void) {
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {
    if (bufp >= 200)
        printf("ungetch: too many characters\n");
    else
        buf[--bufp] = c;
}

void push(double f) {

    if (sp < 200) {
        STACK[sp++] = f;
    } else {
        printf("Stack overflow\n");
    }
}

double pop(void) {
    if (sp > 0) {
        return STACK[--sp];
    } else {
        printf("Stack empty\n");
        return -1.0;
    }
}

void print(void) {

    if (sp < 0) {
        printf("Stack empty\n");
    } else {
        int tmp = sp;
        for (tmp; tmp >= 0; tmp++) {
            printf("Stack element: %f\t", STACK[--tmp]);
        }
    }

    return;

}

double copy(void) {
    double res;
    int tmp = sp;
    if (sp < 0) {
        printf("Stack empty\n");
        return -1.0;
    } else
        res = STACK[--tmp];

    return res;
}

void exchange(void){

    double op1,op2;
    op1  = pop();
    op2 = pop();
    push(op1);
    push(op2);

    return;

}

void clear(void){

    if(sp < 0){
        printf("Stack has been empty\n");
        return;
    }else{
        for(sp;sp>=0;){
            STACK[--sp] = 0.0;
        }
    }
    return ;
}

int getop(char s[]) {

    int i, c;
    while ((s[0] = c = getch()) == ' ' || c == '\t');   // skipped all space
    s[1] = '\0';

    if (!isdigit(c) && c != '.')
        return c;   // if it's not char

    i = 0;

    if (isdigit(c))
        while (isdigit(s[++i] = c = getch()));

    if (c == '.')
        while (isdigit(s[++i] = c = getch()));

    s[i] = '\0';

    if (c != EOF)
        ungetch(c);

    return '\0';    // marked number had been collected
}
