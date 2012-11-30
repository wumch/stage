
#pragma once

#include "meta.hpp"
#include <stdio.h>
#include <sys/stat.h>

namespace staging {

static uint64_t filesize(const char *path)
{
    unsigned long filesize = -1;
    struct stat statbuff;
    if(stat(path, &statbuff) < 0){
        return filesize;
    }else{
        filesize = statbuff.st_size;
    }
    return filesize;
}

}
