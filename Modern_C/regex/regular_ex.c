#include <assert.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *pattern[] = {"[0-9]{1,32}", "[a-zA-Z]{1,32}"};

#define ARRLEN(arr) (sizeof((arr))) / (sizeof(arr[0]))
#define BUFSIZE     256

int main()
{

    size_t  len         = ARRLEN(pattern);
    regex_t regexs[len] = {};

    int     i = 0, ret = 0;

    // Compile all regular expression.
    for (; i < len; i++)
    {
        ret = regcomp(&regexs[i], pattern[i], REG_EXTENDED);
        if (ret)
        {
            char msg[BUFSIZE];
            regerror(ret, &regexs[i], msg, ARRLEN(msg));
            printf("Regex compile failed:%s.\n", msg);
            assert(0);
        }
    }

    char *input = malloc(BUFSIZE * sizeof(char));
    scanf("%s", input);

    // Extract all token from input string.
    char *tokens[BUFSIZE] = {};
    char *tmp             = malloc(BUFSIZE);
    tmp                   = strtok(input, " ");

    tokens[0]             = tmp;
    i                     = 0;
    while (tmp)
    {
        tmp         = strtok(input, NULL);
        tokens[++i] = tmp;
    }

    // Execute all regular expression.
    int token_num          = i + 1;
    i                      = 0;
    regmatch_t pmatch[len] = {};
    for (; i < token_num; i++)
    {
        int regex_idx = 0;
        ret           = 0;
        for (; regex_idx < len; regex_idx++)
        {
            ret = regexec(&regexs[regex_idx], tokens[i], 1, &pmatch[regex_idx], 0);
            if (ret)
            {
                char msg[BUFSIZE];
                regerror(ret, &regexs[i], msg, BUFSIZE);
                printf("Regex Error:%s.\n", msg);
            }

            int gap = pmatch[regex_idx].rm_eo - pmatch[regex_idx].rm_so;
            printf("Matched expression:%*.s.\n", (gap), (tokens[i] + pmatch[regex_idx].rm_so));
        }
    }

    return 0;
}