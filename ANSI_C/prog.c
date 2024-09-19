
#include "Prac8.h"

// Prac 8-3 file operation

void Prac8_3() {

    FILE *fp;
    char *f = "./test.txt";
    char s[]= "This is the first line writed into file";
    int len = sizeof(s) / sizeof(s[0]);

    if ((fp = fopen(f, "w")) == NULL)
        return;
    else
        write(fp->fd, s, len-1);
    return;
}

// Prac 8-7 malloc

void Prac8_7(){

    char *s;
    FILE* fp;
    if((s = (char*)malloc(100)) == NULL)
        return;

    if ((fp = fopen("./1.txt", "w")) == NULL)
        return;
    else
        write(fp->fd, s, 100);

    return;
}

int main(int argc, char *argv[]) {
    Prac8_7();
}