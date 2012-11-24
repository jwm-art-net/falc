#include "falc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char** argv)
{
    int failed = 0;
    char str1[BUFSZ];
    char str2[BUFSZ];
    char* r = 0;
    char* c = 0;

    int rl;

    char* n1 = 0;
    char* n2 = 0;

    printf("enter 1st number: ");
    fgets(str1, BUFSZ, stdin);

    printf("enter 2nd number: ");
    fgets(str2, BUFSZ, stdin);

    n1 = digitstr_from_str(str1);

    if (!n1)
        goto fail;

    n2 = digitstr_from_str(str2);

    if (!n2)
        goto fail;

    r = digitstr_add(n1, n2, &c);

    if (!r)
        goto fail;

    rl = (int)strlen(r);
    printf("\n%*s +\n", rl, n1);
    printf("%*s\n",   rl, n2);
    char_repeat('-', rl);
    printf("\n%*s\n",   rl, r);
    char_repeat('-', rl);
    printf("\n%*s\n",   rl, c);

finish:
    free(n1);
    free(n2);
    free(r);
    free(c);

    if (failed)
        printf("failed\n");

    return failed;

fail:
    failed = -1;
    goto finish;
}
