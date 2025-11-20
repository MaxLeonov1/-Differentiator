#pragma once

#include <stdlib.h>

#include "errors.h"

typedef struct Tree Tree_t;
typedef struct TreeNode TreeNode_t;

/*=====================================================================================*/

typedef enum {

    DIFF_OK = 0,
    DIFF_MEM_ALLOC_ERR = 2,

} DiffErr_t;

typedef enum {

    ADD  = 0,
    MULT = 1,
    SUB  = 2,
    DIV  = 3,
    SIN  = 4,
    COS  = 5,
    TG   = 6,
    CTG  = 7,
    DEG  = 8,
    LN   = 9,

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
    {Oper_t::ADD,  "ADD",  "+",   0, 0},
    {Oper_t::MULT, "MULT", "*",   0, 0},
    {Oper_t::SUB,  "SUB",  "-",   0, 0},
    {Oper_t::DIV,  "DIV",  "/",   0, 0},
    {Oper_t::SIN,  "SIN",  "sin", 0, 1},
    {Oper_t::COS,  "COS",  "cos", 0, 1},
    {Oper_t::TG,   "TG",   "tg",  0, 1},
    {Oper_t::CTG,  "CTG",  "ctg", 0, 1},
    {Oper_t::DEG,  "DEG",  "^",   0, 0},
    {Oper_t::LN,   "LN",   "ln",  0, 1},

};

/*=====================================================================================*/

typedef struct {

    double val;
    char*  name;

} Var_t;

typedef struct {

    int    size;
    int    num;
    Var_t* buff;

} NameTable_t;

typedef struct {

    int op_num;

    int tree_num;
    Tree_t** forest;

    OperInstr_t* def_op_instr;
    OperInstr_t* sort_op_instr;

    NameTable_t name_table;

} Diff_t;

#define DIFF_INIT(name) Diff_t name = { \
    .op_num = 0,                        \
    .tree_num = 0,                      \
    .forest = nullptr,                  \
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
TreeErr_t AddToNameTable   ( Diff_t* diff, const char* name );

