#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/utils.c"
#include <likwid.h>
#include "teste.h"

#define typeof(var) _Generic( (var),\
char: "Char",\
int: "Integer",\
float: "Float",\
char *: "String",\
void *: "Pointer",\
default: "Undefined")


int main()
{
     int x = 0/0;
    void *p = NULL;
    char *s = "text";
    printf("%s\n%s\n%s\n", typeof(x), typeof(p), typeof(s));
}
