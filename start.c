#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFSZ 100




/* just a start */

void chr_repeat(int c, int n)
{
    while(n-- > 0)
        putchar(c);
}


char* str_getnum(const char* str)
{
    const char* s1 = str;
    const char* s2;
    char* r = 0;

    if (!str)
        return 0;

    s1 = str;

    /* skip whitespace */
    while(*s1 != '\0' && isspace(*s1))
        ++s1;

    s2 = s1;

    /* test for non numeric characters found */
    while(*s2 != '\0')
    {
        if (isspace(*s2))
            break;
        else if (!isdigit(*s2))
            return 0;
        ++s2;
    }

    /* zero length string test without using strlen */
    if (s1 == s2)
        return 0;

    /* skip leading zeros... */
    for (; *s1 == '0'; ++s1);

    /* ...unless number is zero */
    if (!isdigit(*s1))
        --s1;

    r = malloc((s2 - s1) + 1);
    strncpy(r, s1, (s2 - s1));
    *(r + (s2 - s1)) = '\0';

    return r;
}


int char_add(char chr1, char chr2, char* carry_chr)
{
    int c1 = (chr1 - '0');
    int c2 = (chr2 - '0');
    int s = c1 + c2;

    if (!isdigit(*carry_chr))
        *carry_chr = '0';

    if (s > 9)
    {
        ++(*carry_chr);
        s -= 10;
    }

    return (s + '0');
}


char* add(const char* n1, const char* n2, char** carry)
{
    char* zero = "0";
    const char *np1;
    const char *np2;

    char cbuf[BUFSZ];
    char rbuf[BUFSZ];

    char* cp = cbuf + BUFSZ - 1;
    char* rp = rbuf + BUFSZ - 1;

    np1 = n1 + strlen(n1) - 1;
    np2 = n2 + strlen(n2) - 1;

    *rp = *cp = '\0';
    --cp;

    *cp = '0';

    do
    {
        char r;
        --rp;
        --cp;
        *rp = *cp = '0';
        r = char_add(*np1, *np2, cp);
        *rp = char_add(r, *(cp + 1), cp);

        /* erase old zeros from carry line */
        if (*(cp + 1) == '0')
            *(cp + 1) = ' ';

        if (np1 != zero && --np1 < n1)
            np1 = zero;

        if (np2 != zero && --np2 < n2)
            np2 = zero;

    } while (np1 != zero || np2 != zero);

    /* won't use isdigit due to test required: */
    if (*cp > '0' && *cp <= '9')
    {
        *--rp = char_add(*cp, '0', cbuf);
    }
    else /* skip 1st zero in carry line */
        ++cp;

    *carry = strdup(cp);
    return strdup(rp);
}


int main(int argc, char** argv)
{
    int failed = 0;
    char str1[BUFSZ];
    char str2[BUFSZ];
    char* r;
    char* c;

    int rl;

    char* n1 = 0;
    char* n2 = 0;

    printf("enter 1st number: ");
    fgets(str1, BUFSZ, stdin);

    printf("enter 2nd number: ");
    fgets(str2, BUFSZ, stdin);

    n1 = str_getnum(str1);

    if (!n1)
        goto fail;

    n2 = str_getnum(str2);

    if (!n2)
        goto fail;

    r = add(n1, n2, &c);

    if (!r)
        goto fail;

    rl = (int)strlen(r);
    printf("\n%*s +\n", rl, n1);
    printf("%*s\n",   rl, n2);
    chr_repeat('-', rl);
    printf("\n%*s\n",   rl, r);
    chr_repeat('-', rl);
    printf("\n%*s\n",   rl, c);

finish:
    free(n1);
    free(n2);

    if (failed)
        printf("failed\n");

    return failed;

fail:
    failed = -1;
    goto finish;
}
