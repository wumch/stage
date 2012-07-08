#include <iostream>
#include "/data/fsuggest/staging/ccpp/memory.h"

int main()
{
    char c[6] = "abcde";
    int a = *(int*)("abcdefg");
    if (CS_MEMCMP(c, &a, 4) == 0)
    {
        std::cout << "yes" << std::endl;
    }
    else
    {
        std::cout << "no" << std::endl;
    }
}

