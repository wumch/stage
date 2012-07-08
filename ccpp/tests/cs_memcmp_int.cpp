#include <iostream>
#include "/data/fsuggest/staging/ccpp/memory.h"

int main()
{
    char c[6] = "abcde";
    int a = *(int*)("abcdefg");
    if (*(int*)c == a)
    {
        std::cout << "yes" << std::endl;
    }
    else
    {
        std::cout << "no" << std::endl;
    }
}

