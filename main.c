#include "falc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define COL_RESULT
#define COL_CARRY
/*
#define COL_RESULT "\033[1;37m"
#define COL_CARRY  "\033[1;30m"
*/
#define COL_RESET  "\033[0m"


const char* ordigits[] = { "zeroth",
                           "first",   "second", "third",
                           "fourth",  "fifth",  "sixth",
                           "seventh", "eighth", "nineth" };


void char_repeat(int c, int n)
{
    while(n-- > 0)
        putchar(c);
}


digitstr** get_user_nums(int* count)
{
    char buf[BUFSZ];
    digitstr* ds[10];
    digitstr** nums = 0;
    int n = 0;

    *count = 0;

    printf("enter between one and ten numbers, zero to finish.\n"
           "press [return] after each number.\n");

    do
    {
        printf("enter %s number:\t", ordigits[n + 1]);
        fgets(buf, BUFSZ, stdin);

        if ((ds[n] = digitstr_new_from_string(buf)))
        {
            if (digitstr_is_zero(ds[n]))
            {
                digitstr_free(ds[n]);
                ds[n] = 0;
                break;
            }
        }
    } while (++n < 9);

    if (n == 9)
        ds[n] = 0;

    nums = malloc(sizeof(digitstr*) * (n + 1));

    for(n = 0; ds[n] != 0; ++n)
        nums[n] = ds[n];

    nums[n] = 0;

    *count = n;

    return nums;
}


int main(int argc, char** argv)
{
    digitstr** nums;
    digitstr** nptr;
    int count = 0;

    if (!(nums = get_user_nums(&count)))
    {
        printf("fail\n");
        return -1;
    }

    nptr = nums;

    if (*nums != 0)
    {
        if (count > 1)
        {
            digitstr* sum = digitstr_array_sum(nums);
            const char* val = digitstr_get_value(sum);
            int l = (int)strlen(val);
            digitstr* pp = nums[count / 2 - (count % 2 ? 0 : 1)];

            for (nptr = nums; *nptr != 0; ++nptr)
            {
                printf((*nptr == pp ? "%*s +\n" : "%*s\n"), l,
                        digitstr_get_value(*nptr));
            }

            char_repeat('-', l);
            printf(COL_RESULT "\n%*s\n",   l, val);
            printf(COL_RESET);
            char_repeat('-', l);
            printf(COL_CARRY "\n%*s\n",   l, digitstr_get_carry(sum));
            printf(COL_RESET);

            digitstr_free(sum);
        }

        for (nptr = nums; *nptr != 0; ++nptr)
            digitstr_free(*nptr);
    }

    free(nums);

    return 0;
}

