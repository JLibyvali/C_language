#include "reflection.h"

#include <stdio.h>
#include <string.h>

// Function set
void text_cb(void *_param) { printf("#### TEXT CALLBACK ###: %s \n", (char *)_param); }

void google_cb(void *_param) { printf("#### GOOGLE CALLBACK #####: %s \n", (char *)_param); }

int  main()
{

    printf(RED "Reflection Section Range [ %p <---> %p ] \n" NONE, __start_reflection_custom, __stop_reflection_custom);
    printf("#################### Dynamic Reflection #################\n");
    char str[4096];
    REFLECTION_REG(text, text_cb);
    REFLECTION_REG(google, google_cb);

    while (1)
    {
        printf("INPUT REFLECTION name.\n");
        scanf("%s", str);

        if (!strcmp(str, "q"))
            break;

        invoke_ref(str, (void *)"HELP HELP");
    }
}