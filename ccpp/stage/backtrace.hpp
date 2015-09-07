
#pragma once

#include "meta.hpp"
#include <unistd.h>
#include <cxxabi.h>
#include <execinfo.h>
#include <cstdlib>
#include <iostream>
#include <vector>

namespace stage
{

class Backtrace
{
    std::size_t depth, collected;

    void** frames;

    char** infos;

public:
    explicit Backtrace(std::size_t _depth):
        depth(_depth), frames(NULL), infos(NULL)
    {}

    void dump()
    {
        frames = new void*[depth];
        collected = backtrace(frames, depth);
        if (frames == NULL || collected < 1)
        {
            CS_ERR("no backtraces");
        }
        else
        {
            infos = backtrace_symbols(frames, collected);
            delete[] frames;

            if (infos == NULL)
            {
                CS_ERR("no backtraces");
            }
            else
            {
                for (int i = 1, end = collected - 2; i < end; ++i)
                {
                    char *mangled_name = 0, *offset_begin = 0, *offset_end = 0;

                    // find parantheses and +address offset surrounding mangled name
                    for (char *p = infos[i]; *p; ++p)
                    {
                        if (*p == '(')
                        {
                            mangled_name = p;
                        }
                        else if (*p == '+')
                        {
                            offset_begin = p;
                        }
                        else if (*p == ')')
                        {
                            offset_end = p;
                            break;
                        }
                    }

                    if (mangled_name && offset_begin && offset_end && mangled_name < offset_begin)
                    {
                        *mangled_name++ = '\0';
                        *offset_begin++ = '\0';
                        *offset_end++ = '\0';

                        int status;
                        char * real_name = abi::__cxa_demangle(mangled_name, 0, 0, &status);

                        // if demangling is successful, output the demangled function name
                        if (status == 0)
                        {
                            std::cerr << "[" << CS_OC_RED(i) << "]: "
                                      << CS_OC_BLUE(real_name) << " +" << offset_begin << offset_end
                                      << std::endl;

                        }
                        // otherwise, output the mangled function name
                        else
                        {
                            std::cerr << "[" << CS_OC_RED(i) << "]: "
                                      << CS_OC_BLUE(mangled_name << "()") << " +" << offset_begin << offset_end
                                      << std::endl;
                        }
                        free(real_name);
                    }
                    // otherwise, print the whole line
                    else
                    {
                        std::cerr << "[bt]: (" << i << ") " << infos[i] << std::endl;
                    }
//                    free(infos);
                }
            }
        }
    }
};

#define CS_BACKTRACE(depth)     \
do {                            \
    stage::Backtrace bt(depth); \
    bt.dump();           \
} while(false);

}
