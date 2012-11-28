#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFSZ 100


#include "falc.h"


typedef struct digitstr
{
    char* str;      /* the number string */
    char* p;        /* a handy pointer */
    char* carry;    /* carry string, if applicable */

} digitstr;


static char* zero = "0";


digitstr* digitstr_new(void)
{
    digitstr* ds = malloc(sizeof(*ds));

    if (!ds)
        return 0;

    ds->str = strdup("0");
    ds->p = 0;
    ds->carry = 0;

    return ds;
}


static digitstr* ds_new_from_result(const char* r, const char* c)
{
    digitstr* ds = malloc(sizeof(*ds));

    if (!ds)
        return 0;

    ds->str = strdup(r);
    ds->carry = strdup(c);
    ds->p = 0;

    return ds;
}


digitstr* digitstr_new_from_string(const char* str)
{
    const char* s1 = str;
    const char* s2;
    digitstr* ds = 0;

    if (!str)
        return digitstr_new();

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

    /* skip all leading zeros... */
    for (; *s1 == '0'; ++s1);

    /* ...unless number is zero */
    if (!isdigit(*s1))
        --s1;

    
    if (!(ds = malloc(sizeof(*ds))))
        return 0;

    if (!(ds->str = malloc((s2 - s1) + 1)))
    {
        free(ds);
        return 0;
    }

    strncpy(ds->str, s1, (s2 - s1));
    *(ds->str + (s2 - s1)) = '\0';

    ds->p = 0;
    ds->carry = 0;

    return ds;
}


digitstr* digitstr_new_from_int(int n)
{
    char buf[BUFSZ];
    snprintf(buf, BUFSZ, "%d", n);
    return digitstr_new_from_string(buf);
}


void digitstr_free(digitstr* ds)
{
    if (!ds)
        return;

    free(ds->str);
    free(ds->carry);
    ds->str = 0;
    ds->p = 0;
    ds->carry = 0;
    free(ds);
}


const char* digitstr_get_value(const digitstr* ds)
{
    return ds->str;
}


const char* digitstr_get_carry(const digitstr* ds)
{
    return ds->carry;
}


int digitstr_is_nonzero(const digitstr* ds)
{
    return !(!ds || *ds->str == '0');
}


int digitstr_is_zero(const digitstr* ds)
{
    return (!ds || *ds->str == '0');
}


static int digit_add(char d1, char d2, char* carry_chr)
{
    int n1 = (d1 - '0');
    int n2 = (d2 - '0');
    int s = n1 + n2;

    if (!isdigit(*carry_chr))
        *carry_chr = '0';

    if (s > 9)
    {
        ++(*carry_chr);
        s -= 10;
    }

    return (s + '0');
}


digitstr* digitstr_add(digitstr* a, digitstr* b)
{
    char cbuf[BUFSZ];
    char rbuf[BUFSZ];

    char* cp = cbuf + BUFSZ - 1;
    char* rp = rbuf + BUFSZ - 1;

    a->p = a->str + strlen(a->str)  - 1;
    b->p = b->str + strlen(b->str)  - 1;

    *rp = *cp = '\0';
    --cp;

    *cp = '0';

    do
    {
        char r;
        --rp;
        --cp;
        *rp = *cp = '0';
        r = digit_add(*a->p, *b->p, cp);
        *rp = digit_add(r, *(cp + 1), cp);

        /* erase old zeros from carry line */
        if (*(cp + 1) == '0')
            *(cp + 1) = ' ';

        if (a->p != zero && --a->p < a->str)
            a->p = zero;

        if (b->p != zero && --b->p < b->str)
            b->p = zero;

    } while (a->p != zero || b->p != zero);

    /* won't use isdigit due to test required: */
    if (*cp > '0' && *cp <= '9')
    {
        *--rp = digit_add(*cp, '0', cbuf);
    }
    else /* skip 1st zero in carry line */
        ++cp;

    return ds_new_from_result(rp, cp);
}


digitstr* digitstr_array_sum(digitstr** arr)
{
    int count = 0;
    int i;

    char cbuf[BUFSZ] = {0};
    char rbuf[BUFSZ] = {0};

    char* cp = cbuf + BUFSZ - 1;
    char* rp = rbuf + BUFSZ - 1;
    int done_count = 0;

    for (; arr[count] != 0; )
        ++count;

    if (!count)
        return 0;

    for (i = 0; i < count; ++i)
    {
        for (arr[i]->p = arr[i]->str;
             *(arr[i]->p + 1) != '\0'; ++arr[i]->p);
    }

    *rp = *cp = '\0';
    --cp;
    *cp = '0';

    do
    {
        char r = '0';
        --rp;
        --cp;
        *rp = *cp = '0';

        for (i = 0; i < count; ++i)
        {
            if (arr[i]->p != zero)
            {
                r = digit_add(r, *arr[i]->p, cp);

                if (--arr[i]->p < arr[i]->str)
                {
                    arr[i]->p = zero;
                    done_count++;
                }
            }
        }

        *rp = digit_add(r, *(cp + 1), cp);


        /* erase old zeros from carry line */
        if (*(cp + 1) == '0')
            *(cp + 1) = ' ';

    } while (done_count < count);

    /* won't use isdigit due to test required: */
    if (*cp > '0' && *cp <= '9')
    {
        --rp;
        /* use first character in cbuf as temp */
        *rp = digit_add(*cp, '0', cbuf);
    }
    else /* skip 1st zero in carry line */
        ++cp;

    return ds_new_from_result(rp, cp);
}

