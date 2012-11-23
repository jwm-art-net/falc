#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFSZ 100


/* just a start */


char* str_getnum(const char* str)
{
    const char* s1 = str;
    const char* s2;
    char* r = 0;
    int dp = 0;
    size_t len = 0;

    if (!str || !strlen(str))
        return 0;

    s1 = str;

    while(*s1 != '\0'
       &&(*s1 == ' ' || *s1 == '\t' || *s1 == '\n' || *s1 == '\r'))
    {
        ++s1;
    }

    s2 = s1;

    while(*s2 != '\0')
    {
        if (*s1 == ' ' || *s2 == '\t' || *s2 == '\n' || *s2 == '\r')
            break;
        else if (*s2 < '0' || *s2 > '9')
            return 0;
        ++s2;
    }

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

    if (*carry_chr < '0' || *carry_chr > '9')
        *carry_chr = '0';

    if (s > 9)
    {
        ++(*carry_chr);
        s -= 10;
    }

    return (s + '0');
}


char* add(const char* num1, const char* num2, char** carry)
{
    char* zero = "0";
    char* n1;
    char* n2;
    char *np1;
    char *np2;

    char cbuf[BUFSZ];
    char rbuf[BUFSZ];

    char* cp = cbuf + BUFSZ - 1;
    char* rp = rbuf + BUFSZ - 1;

    size_t l1, l2, l3, l4, mxl;

    if (!(n1 = str_getnum(num1)))
        return 0;

    if (!(n2 = str_getnum(num2)))
    {
        free(n1);
        return 0;
    }

    l1 = strlen(n1);
    l2 = strlen(n2);

    np1 = n1 + l1 - 1;
    np2 = n2 + l2 - 1;


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

        if (*(cp + 1) == '0')
            *(cp + 1) = ' ';

        if (np1 != zero && --np1 < n1)
            np1 = zero;

        if (np2 != zero && --np2 < n2)
            np2 = zero;

    } while (np1 != zero || np2 != zero);

    if (*cp > '0' && *cp <= '9')
    {
        *--rp = char_add(*cp, '0', cbuf);
    }
    else
        *cp = ' ';

    l3 = strlen(rp);
    l4 = strlen(cp);

    mxl = l1;
    if (mxl < l2)
        mxl = l2;

    if (mxl < l3)
        mxl = l3;

    if (mxl < l4)
        mxl = l4;

    printf("       %*s +\n", mxl, n1);
    printf("       %*s\n", mxl, n2);
    printf("result:%*s\n", mxl, rp);
    printf(" carry:%*s\n", mxl, cp);

    free(n1);
    free(n2);
}


int main(int argc, char** argv)
{

    char n1[BUFSZ];
    char n2[BUFSZ];
    char* r;

    printf("enter 1st number: ");
    fgets(n1, BUFSZ, stdin);

    printf("enter 2nd number: ");
    fgets(n2, BUFSZ, stdin);

    r = add(n1,n2,0);

    if (!r)
        printf("failed\n");

    return 0;
}
