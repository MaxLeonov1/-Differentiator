#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "differentiator.h"
#include "errors.h"

#define _DEBUG

/*=====================================================================================*/

typedef char*  TreeElem_t;
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
    TreeNode_t* parent;

};

struct TreeDebugInfo {

    const char* name;
    const char* func;
    const char* file;
    const int line;

};

struct Tree {

    #ifdef _DEBUG
    TreeDebugInfo_t info;
    #endif

    TreeNode_t* root;

    size_t cpcty; 
    char* buffer; 

};

/*=====================================================================================*/

void        TreeCtor     ( Tree_t* tree );
TreeErr_t   TreeDtor     ( Tree_t* tree );
TreeErr_t   AllocNode    ( TreeNode_t** node, Node_t node_t );
void        CreateLogDir ( char* dir_name );
TreeErr_t   DeleteNode   ( TreeNode* node );
TreeErr_t   InsertNode   ( TreeNode_t** node, Val_t value, Node_t node_t, TreeNode_t* prev_node );
TreeNode_t* CopyTree ( TreeNode_t* node, TreeNode_t* prev_node );

TreeErr_t   SaveToDisk   ( Diff_t* diff, int tree_idx, const char* disk_name );
void        WriteToDisk  ( TreeNode_t* node, Var_t* name_table, FILE* disk );
TreeErr_t   ReadFromDisk ( Diff_t* diff, const char* filename );
TreeNode_t* ReadNode     ( char* buffer, size_t* pos, TreeErr_t* status, Diff_t* diff );
char*       ReadData     ( char* ptr, size_t* len );

const char* StatusCodeToStr ( TreeErr_t status );
void TreeDump       ( Diff_t* diff, int tree_idx, TreeErr_t status, const char* format, ... );
void PrintLogHeader ( Tree_t* tree, FILE* log_file, TreeErr_t status );
int  PrintGraphNodes( TreeNode_t* node, int rank, FILE* graph_text, Diff_t* diff );
void CreateGraphImg ( Diff_t* diff, int tree_idx, const char* graphname, const char* graph_dir );

TreeErr_t CreateNodeFromStr ( const char* str, Diff_t* diff, TreeNode_t** new_node );

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

#ifdef _WIN32
    #include <direct.h>
    #define _MKDIR(name) _mkdir(name)
    #define PATH_SEP '\\'
#else
    #define _MKDIR(name) mkdir(name, FILE_MODE_)
    #define PATH_SEP '/'
#endif

#ifndef _DEBUG
    #define TREE_INIT(name) Tree_t name = { nullptr, 0 };
#else
    #define TREE_INIT(name) Tree_t name = { {#name, __FUNCTION__, __FILE__, __LINE__}, nullptr, 0 };
#endif

#define _OK_STAT_ TreeErr_t status = TreeErr_t::TREE_OK;
#define _RET_OK_ return TreeErr_t::TREE_OK;
#define _DUMP_IF_ERR_(tree, status, mes) if(status = TreeErr_t::TREE_OK) TreeDump(&tree, status, mes);

#define TREE_STAT_CHECK_DUMP_(tree_p, status)   \
    if ( status != TreeErr_t::TREE_OK ) {  \
        TreeDump(tree_p, status);          \
        return status;                     \
    }

#define TREE_STAT_CHECK_                              \
    if ( status != TreeErr_t::TREE_OK ) return status; \

#define SKIP_SPACE_ while (buffer[*pos] == ' ') (*pos)++;

/*=====================================================================================*/