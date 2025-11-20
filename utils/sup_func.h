#pragma once

/*=====================================================================================*/

#define MAX_STR_LEN_ 1024 

#ifdef _WIN32
    #define CLS_ system("cls");
    typedef unsigned int u_int;
    typedef unsigned char u_char;
#else
    #define CLS_ system("clear");
#endif

#ifdef _DEBUG
    #define _IF_DEBUG(source) source
#else
    #define _IF_DEBUG(source)
#endif

/*=====================================================================================*/

long long FileByteCount ( const char* filename );
unsigned int djb2hash ( const char *str );
int skip_space ( char* ptr, size_t* pos );
char* my_strdup(const char* str);
int   double_cmp (double double_1, double double_2);