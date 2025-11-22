#pragma once

#include <sys/stat.h>

#include "dump_tex.h"
#include "tree_dump.h"



const char* GetGlobalLogDir();



#ifdef _WIN32
    #include <direct.h>
    #define _MKDIR(name) _mkdir(name)
    #define PATH_SEP '\\'
#else
    #define _MKDIR(name) mkdir(name, FILE_MODE_)
    #define PATH_SEP '/'
#endif