#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "differentiator.h"
#include "errors.h"

#define _DEBUG

/*=====================================================================================*/

typedef struct TreeNode TreeNode_t;
typedef struct Tree Tree_t;
typedef struct TreeDebugInfo TreeDebugInfo_t;

/*=====================================================================================*/

typedef enum{

    OP_UN = 0,
    OP_BIN = 1,
    VAR = 2,
    NUM = 3,

} Node_t;

typedef union {

    double num;
    size_t var_idx;
    Oper_t oper;

} Val_t;

struct TreeNode {

    Node_t      type;
    Val_t       data;
    TreeNode_t* left;
    TreeNode_t* right;

};

struct TreeDebugInfo {

    const char* func;
    const char* file;
    int line;

};

struct Tree {

    #ifdef _DEBUG
    TreeDebugInfo_t info;
    #endif

    TreeNode_t* root;

    size_t cpcty; 

};

/*=====================================================================================*/

void        TreeCtor    ( Tree_t* tree );
DiffErr_t   TreeDtor    ( Tree_t* tree );
DiffErr_t   AllocNode   ( TreeNode_t** node, Node_t node_t );
DiffErr_t   DeleteNode  ( TreeNode* node );
DiffErr_t   InsertNode  ( TreeNode_t** node, Val_t value, Node_t node_t);
TreeNode_t* CopyTree    ( TreeNode_t* node);
int         AddEquation ( Diff_t* diff, DiffErr_t* status);

TreeNode_t* CreateNumNode ( double val);
TreeNode_t* CreateBinOp   ( Oper_t oper, TreeNode_t* left, TreeNode_t* right);
TreeNode_t* CreateUnOp    ( Oper_t oper, TreeNode_t* left);
TreeNode_t* CreateVarNode ( int var_idx);

/*=====================================================================================*/

#define _left_ 0
#define _right_ 1
#define _nil_ "nil"
#define _nil_len_ 4
#define _buff_byte_padding_ 5

#define FILE_MODE_ 0755
#define MAX_STR_LEN_ 1024

#define EDGE_STD_SET_ "penwidth = 2.0, arrowsize = 0.5, constraint = \"true\""
#define DEF_DISK_NAME_ "treedisk.bin"

#define OLD_NODE_COL_ "fillcolor = \"#fff0b3\", color = \"#ffe680\""
#define NEW_NODE_COL_ "fillcolor = \"#ccccff\", color = \"#b3b3ff\""

/*=====================================================================================*/

#ifndef _DEBUG
    #define TREE_INIT(name) Tree_t name = { nullptr, 0 };
    #define TREE_INIT_LIST(name) { nullptr, 0 }
#else
    #define TREE_INIT(name) Tree_t name = { {#name, __FUNCTION__, __FILE__, __LINE__}, nullptr, 0};
    #define TREE_INIT_LIST(name) { {#name, __FUNCTION__, __FILE__, __LINE__}, nullptr, 0}
#endif

#define _OK_STAT_ DiffErr_t status = DiffErr_t::TREE_OK;
#define _RET_OK_ return DiffErr_t::TREE_OK;
#define _DUMP_IF_ERR_(tree, status, mes) if(status = DiffErr_t::TREE_OK) TreeDump(&tree, status, mes);

#define TREE_STAT_CHECK_DUMP_(tree_p, status)   \
    if ( status != DiffErr_t::TREE_OK ) {  \
        TreeDump(tree_p, status);          \
        return status;                     \
    }

#define TREE_STAT_CHECK_                              \
    if ( status != DiffErr_t::TREE_OK ) return status; \

#define SKIP_SPACE_ while (buffer[*pos] == ' ') (*pos)++;

/*=====================================================================================*/