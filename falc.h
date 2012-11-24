#ifndef FALC_H
#define FALC_H



#define BUFSZ 100



/* print character c repeatedly to stdout n times
*/
void    char_repeat(int c, int n);


/*  arithmetically add d1 and d2 and return result
    note: assumes ((isdigit(c1) && isdigit(c2)) == TRUE)
*/
int     digit_add(char d1, char d2, char* carry);


/*  return integer as string found in string str
    returns NULL if non numeric characters found
*/
char*   digitstr_from_str(const char* str);


/*  adds two integer digit strings,
    stores carry line in location pointed to by carry
    parameters n1 and n2 should both have been obtained
    by using digits_int_from_str
*/
char*   digitstr_add(const char* n1, const char* n2, char** carry);




#endif
