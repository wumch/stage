#include <stdio.h>

#define INT_TYPE(size) (size == 4 ? int : short)

int main()
{
    INT_TYPE(4) a = 9;
    printf("%d\n", a);
}

