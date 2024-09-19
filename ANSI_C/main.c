#include "Prac2.h"
#include "Prac5.h"
#include "Prac6.h"
#include "Prac7.h"
#include <stdio.h>
#include <stdlib.h>

// Prac 2-6
char *hexString;

void Prac2_3(void) {
    // Prac 2-3
    hexString = (char *)malloc(100);
    printf("Input Hex number\n");
    scanf("%s", hexString);
    int len = strlen(hexString);
    printf("Result:D%d", Htoi(hexString, len));
}

// Prac 2-6
void Prac2_6(void) {
    int x, y, pos, n;
    printf("Input x, y, pos, n\n");
    scanf("%d%d%d%d", &x, &y, &pos, &n);
    printf("%s", setbits(x, pos, n, y));
}

// Prac 2-7
void Prac2_7(void) {
    int x, p, n;
    printf("Input x, p, n\n");
    scanf("%d%d%d", &x, &p, &n);
    printf("%s", invert(x, p, n));
}

// Prac 2-8
void Prac2_8(void) {
    int x, n;
    printf("Input x, n\n");
    scanf("%d%d", &x, &n);
    printf("%s", rightrot(x, n));
}

// Prac 4-4
void Prac4_4(void) {
    int type;
    double op2;
    char s[100];

    printf("Input one expression\n");
    scanf("%s", &s);

    while ((type = getop(s)) != EOF) {
        switch (type) {
            case '0':
                push(atof(s));
                break;
            case '+':
                push(pop() + pop());
                break;
            case '-':
                op2 = pop();
                push(pop() - op2);
                break;
            case '*':
                push(pop() * pop());
                break;
            case '/':
                op2 = pop();
                if (op2 != 0.0) {
                    push(pop() / op2);
                    break;
                } else {
                    printf("Error: zero divisor\n");
                }
                break;
            case '\n':
                printf("\t%.8g\n", pop());
                break;
            default:
                printf("Error: unknown command\n");
                break;
        }
    }
}

// Prac 4-14
// Marco function is useful
#define swap(t, a, b) \
    {                 \
        t tmp1;       \
        tmp1 = a;     \
        a    = b;     \
        b    = tmp1;  \
    }

// Prac 5-7
char *lineptr[MAXLINE];

int Prac5_7(void) {
    int nlines; // line num for input

    if ((nlines = readlines(lineptr, MAXLINE)) >=
        0) { // build one linked list for input string
        Qsort(lineptr, 0, nlines - 1);
        writelines(lineptr, nlines);
        return 0;
    } else {
        printf("Error: input too big\n");
        return 1;
    }
}

// Prac 5-10
void Prac5_10(int argc, char *argv[]) {
    int i;
    int s[100];
    memset(s, 0, sizeof(s));

    for (i = 0, --argc; argc > 0; argc--, i++) {
        s[i] = (int)(*++argv)[0];
    }

    int res = cal(s, i);
    printf("Res: %d\n", res);
}

char *lineptr2[1000];

// Prac 5-11
void Prac5_11(int argc, char *argv[]) { // sort any type string, with passing
                                        // the functional pointer as argyments.
    int nlines;
    int numeric = 0;

    if (argc > 1 && strcmp(argv[1], "-n") == 0) numeric = 1;
    if ((nlines = readlines(lineptr2, 1000)) >= 0) {
        Qsort2((void **)lineptr2, 0, nlines - 1,
               (int (*)(void *, void *))(
                   numeric ? numcmp : strcmp)); // pass the numcmp or strcmp to
                                                // initialize function pointer
        writelines(lineptr, nlines);
        return;
    } else {
        printf("Input too bit to sort\n");
    }
}

// Prac 6-2
// using self reference 'struct' , and the binary tree to count the key word
// nums
void Prac6_2(void) {
    char word[1000];
    struct key *p;

    while (getword(word, 1000) != EOF) {
        if (isalpha(word[0]))
            if ((p = binsearch(word, keytab, NKEYS)) != NULL) p->cnt++;
    }
    int f = 0;
    for (p = keytab; p < keytab + NKEYS; p++) {
        if (p->cnt > 0) f = 1;
        printf("Has result: %4d %s \n", p->cnt, p->word);

        if (f) printf("None result matched\n");
    }

    return;
}

// Prac6-4
void Prac6_4() {
    struct tnode *root;
    char word[100];

    root = NULL;

    while (getword(word, 100) != EOF)
        if (isalpha(word[0])) root = addTree(root, word);

    treeprint(root);
    return;
}

// Prac6-6
/**
 * @brief Using hash list to matching string
 */
void Prac6_6(void) {
    char *defn[] = {"1", "2", "3"};
    char *name[] = {"an", "bn", "cn"};

    int i = 3;
    struct nlist *p;
    while (i-- > 0) {
        if (install((*name)++, (*defn)++) == NULL)
            return;
        else
            continue;
    }
    char s[20];
    while (scanf("%s", &s) != EOF) {
        if ((p = lookup(s)) != NULL)
            printf("Existed:Define %s  %s\n", p->name, p->defn);
        else
            printf("Cannot find define\n");
    }
}

// Prac 7-3
/**
 * @brief implement printf using variable arguments
 *
 */
void Prac7_3() {

    int i    = 10;
    double d = 2.312312;
    char *s  = "TEST for Prac7\n";

    minprint("INT: %d DOUBLE: %f  String: %s", i, d, s);
}

// Prac7-5
/**
 * @brief file operation
 *
 * @param argc
 * @param argv
 * @return int
 */
int Prac7_5(int argc, char *argv[]) {
    FILE *fp;
    char *prog = argv[0];
    void filecopy(FILE * input, FILE * dst);

    if (argc == 1)
        filecopy(stdin, stdout);
    else {
        while (--argc > 0) {
            if ((fp = fopen(*++argv, "r")) == NULL) {
                fprintf(stderr, "%s: can't open %s\n", prog, *argv);
                exit(1);
            } else {
                filecopy(fp, stdout);
                fclose(fp);
            }
            return 0;
        }
    }

    if (ferror(stdout)) {
        fprintf(stderr, "%s: error writing stdout\n", prog);
        exit(2);
    }
	exit(0);
}

void filecopy(FILE *input, FILE *dst) {

    int c;
    while ((c = getc(input))) {
        putc(c, dst);
    }
}

int main(int argc, char *argv[]) {

    Prac7_5(argc, argv);
}