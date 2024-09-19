#include <stdio.h>
#include <stdarg.h>


//Prac7-4
void minprint(char *fmt,...);














/////////////////////////////////////////Prac7
void minprint(char *fmt,...){
  va_list ap;
  char *p, *sval;
  int ival;
  double dval;

  va_start(ap, fmt); // marco for initialize 'ap' point to first arg

  for (p = fmt; *p; p++) {
    if (*p != '%') {
      putchar(*p);
      continue;
    }
    switch (*++p) {
      case 'd':
        ival = va_arg(ap, int); // va_arg return one argument, and make ap point to next
        printf("%d", ival);
        break;
      case 'f':
        dval = va_arg(ap, double);
        printf("%f", dval);
        break;
      case 's':
        for (sval = va_arg(ap, char *); *sval; sval++)
          putchar(*sval);
        break;
      default:
        putchar(*p);
        break;
    }
  }

  va_end(ap);
}
