#ifndef FALC_H
#define FALC_H


#define BUFSZ 100


typedef struct digitstr digitstr;


digitstr*   digitstr_new(void);
digitstr*   digitstr_new_from_string(const char*);
digitstr*   digitstr_new_from_int(int);

void        digitstr_free(digitstr*);

const char* digitstr_get_value(const digitstr*);
const char* digitstr_get_carry(const digitstr*);

int         digitstr_is_zero(const digitstr*);
int         digitstr_is_nonzero(const digitstr*);


digitstr*   digitstr_add(digitstr* a, digitstr* b);


/* arr must be NULL terminated */
digitstr*   digitstr_array_sum(digitstr** arr);

#endif
