#include <stdio.h>
#include "/data/fsuggest/staging/ccpp/memory.h"

int main()
{
    char c[6] = "abcde";
    int a = *(int*)("abcdefg");
    if (CS_MEMCMP(c, &a, 4) == 0)
    {
        printf("yes\n");
    }
    else
    {
        printf("no\n");
    }
}

