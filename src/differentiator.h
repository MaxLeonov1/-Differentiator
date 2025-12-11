#pragma once

#include <stdlib.h>

#include "errors.h"

typedef struct Tree Tree_t;
typedef struct TreeNode TreeNode_t;

#define MAX_EQ_NUM_ 10

/*=====================================================================================*/

typedef enum {

    ADD    = 0,
    MULT   = 1,
    SUB    = 2,
    DIV    = 3,
    SIN    = 4,
    COS    = 5,
    TG     = 6,
    CTG    = 7,
    DEG    = 8,
    LN     = 9,
    LOG    = 10,
    ARCSIN = 11,
    ARCCOS = 12,
    ARCTG  = 13,
    ARCCTG = 14,
    SH     = 15,
    CH     = 16,
    TH     = 17,
    CTH    = 18,
    ARCSH  = 19,
    ARCCH  = 20,
    ARCTH  = 21,
    ARCCTH = 22, 

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
    {Oper_t::ADD,    "ADD",    "+",      0, 0},
    {Oper_t::MULT,   "MULT",   "*",      0, 0},
    {Oper_t::SUB,    "SUB",    "-",      0, 0},
    {Oper_t::DIV,    "DIV",    "/",      0, 0},
    {Oper_t::SIN,    "SIN",    "sin",    0, 1},
    {Oper_t::COS,    "COS",    "cos",    0, 1},
    {Oper_t::TG,     "TG",     "tg",     0, 1},
    {Oper_t::CTG,    "CTG",    "ctg",    0, 1},
    {Oper_t::DEG,    "DEG",    "^",      0, 0},
    {Oper_t::LN,     "LN",     "ln",     0, 1},
    {Oper_t::LOG,    "LOG",    "log",    0, 0},
    {Oper_t::ARCSIN, "ARCSIN", "arcsin", 0, 1},
    {Oper_t::ARCCOS, "ARCCOS", "arccos", 0, 1},
    {Oper_t::ARCTG,  "ARCTG",  "arctg",  0, 1},
    {Oper_t::ARCCTG, "ARCCTG", "arcctg", 0, 1},
    {Oper_t::SH,     "SH",     "sh",     0, 1},
    {Oper_t::CH,     "CH",     "ch",     0, 1},
    {Oper_t::TH,     "TH",     "th",     0, 1},
    {Oper_t::CTH,    "CTH",    "cth",    0, 1},
    {Oper_t::ARCSH,  "ARCSH",  "arcsh",  0, 1},
    {Oper_t::ARCCH,  "ARCCH",  "arcch",  0, 1},
    {Oper_t::ARCTH,  "ARCTH",  "arcth",  0, 1},
    {Oper_t::ARCCTH, "ARCCTH", "arccth", 0, 1}
};

/*=====================================================================================*/

typedef struct {

    double val;
    char   name;

} Var_t;

typedef struct {

    int    size;
    int    num;
    Var_t* buff;

} NameTable_t;

typedef struct {

    int make;

    double x_max;
    double x_min;
    double y_max;
    double y_min;

} Graph_t;

typedef struct {

    int make;

    double x0;

} Taylor_t;

typedef struct {

    int make;

    int  cur_degree;
    int  degree;
    char var;

} Derivative_t;

typedef struct {

    int make;

    double x0;

} Tangent_t;

typedef struct {

    int simpl_tree;

    Graph_t      graph;
    Taylor_t     taylor;
    Derivative_t dir;
    Tangent_t    tan;

} Params_t;

typedef struct {

    int op_num;

    int max_eq_num;
    int tree_num;
    Tree_t** forest;

    OperInstr_t* def_op_instr;
    OperInstr_t* sort_op_instr;

    NameTable_t name_table;

    Params_t params;

} Diff_t;

#define DIFF_INIT(name) Diff_t name = { \
    .op_num = 0,                        \
    .max_eq_num = 0,                    \
    .tree_num = 0,                      \
    .forest = nullptr,                  \
    .def_op_instr = nullptr,            \
    .sort_op_instr = nullptr,           \
    .name_table = {                     \
        .size = 0,                      \
        .num = 0,                       \
        .buff = nullptr,                \
    },                                  \
    .params = {                         \
        .simpl_tree = 0,                \
        .graph = {                      \
            .make = 0,                  \
            .x_max = 0,                 \
            .x_min = 0,                 \
            .y_max = 0,                 \
            .y_min = 0,                 \
        },                              \
        .taylor = {                     \
            .make = 0,                  \
            .x0 = 0,                    \
        },                              \
        .dir = {                        \
            .make = 0,                  \
            .cur_degree = 0,            \
            .degree = 0,                \
            .var = 0,                   \
        },                              \
        .tan = {                        \
            .make = 0,                  \
            .x0 = 0,                    \
        },                              \
    },                                  \
};                                      

/*=====================================================================================*/

int       HashBinSearch    ( OperInstr_t* Instr, int instr_size, int hash );
void      SortInstrByHash  ( Diff_t* diff );
void      HashAndCopyInstr ( OperInstr_t* Instr_Def, Diff_t* diff );
int       compare          ( const void* cmd_1, const void* cmd_2 );
DiffErr_t DiffCtor         ( Diff_t* diff );
void      DiffDtor         ( Diff_t* diff );
DiffErr_t AddToNameTable   ( Diff_t* diff, char name );
int       GetOperPriority  ( Oper_t oper);