#pragma once

#include <stdlib.h>

typedef struct Tree Tree_t;
typedef struct TreeNode TreeNode_t;

/*=====================================================================================*/

typedef enum {

    DIFF_OK = 0,
    MEM_ALLOC_ERR = 1,

} DiffErr_t;

typedef enum {

    ADD = 0,
    MULT = 1,
    SUB = 2,
    DIV = 3,

} Oper_t;

/*=====================================================================================*/

typedef struct {

    Oper_t code;
    const char* f_name;
    const char* name;
    size_t hash;
    int is_sing;

} OperInstr_t;

static OperInstr_t OperInstructions[] = {
    {Oper_t::ADD,  "ADD",  "+", 0, 0},
    {Oper_t::MULT, "MULT", "*", 0, 0},
    {Oper_t::SUB,  "SUB",  "-", 0, 0},
    {Oper_t::DIV,  "DIV",  "/", 0, 0},
};

/*=====================================================================================*/

typedef struct {

    int    size;
    int    num;
    char** buff;

} NameTable_t;

typedef struct {

    int op_num;

    Tree_t* tree;

    OperInstr_t* def_op_instr;
    OperInstr_t* sort_op_instr;

    NameTable_t name_table;

} Diff_t;

#define DIFF_CTOR(name) Diff_t name = { \
    .op_num = 0,                        \
    .tree = nullptr                     \
    .def_op_instr = nullptr,            \
    .sort_op_instr = nullptr,           \
    .name_table = {                     \
        .size = 0,                      \
        .num = 0,                       \
        .buff = nullptr,                \
    },                                  \
};

/*=====================================================================================*/

int       HashBinSearch    ( OperInstr_t* Instr, int instr_size, int hash );
void      SortInstrByHash  ( Diff_t* diff );
void      HashAndCopyInstr ( OperInstr_t* Instr_Def, Diff_t* diff );
int       compare          ( const void* cmd_1, const void* cmd_2 );
DiffErr_t DiffCtor         ( Diff_t* diff );
void      DiffDtor         ( Diff_t* diff );
DiffErr_t AddToNameTable ( Diff_t* diff, const char* name );

