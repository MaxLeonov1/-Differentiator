#pragma once
#include <stdio.h>
#include <stdlib.h>

#define _DEBUG

/*=====================================================================================*/

typedef char* TreeElem_t;
typedef struct TreeNode TreeNode_t;
typedef struct Tree Tree_t;
typedef struct TreeDebugInfo TreeDebugInfo_t;

/*=====================================================================================*/

struct TreeDebugInfo {

    const char* name;
    const char* func;
    const char* file;
    const int line;

};

struct TreeNode {

    TreeElem_t   data;
    unsigned int data_hash;
    TreeNode_t*  left;
    TreeNode_t*  right;
    TreeNode_t*  parent;
    int          is_alloc;

};

struct Tree {

    #ifdef _DEBUG
    TreeDebugInfo_t info;
    #endif

    TreeNode_t* root;

    size_t cpcty; //TODO: make cpcty changing
    char* buffer; 

};

/*=====================================================================================*/

typedef enum {

    TREE_OK = 0,
    MEM_ALLOC_ERR = 1,
    INSERT_EX_POS_ERR = 2,
    FILE_OPEN_ERR = 3,
    READ_SYNTAX_ERR = 4,
    READ_DATA_ERR = 5,
    EMPTY_TREE_ACT_ERR = 6,

} TreeErr_t;

/*=====================================================================================*/

void      TreeCtor        ( Tree_t* tree );
TreeErr_t TreeDtor        ( Tree_t* tree );
TreeErr_t AllocNode       ( TreeNode_t** node );
void      CreateLogDir    ( char* dir_name );
TreeErr_t DeleteNode      ( TreeNode* node );
TreeErr_t InsertNode      ( TreeNode_t** node, TreeElem_t elem, TreeNode_t* prev_node );
TreeErr_t InsertNodeAfter ( TreeNode_t* node, TreeElem_t elem, int child );

TreeErr_t   SaveToDisk   ( Tree_t* tree, const char* disk_name );
void        WriteToDisk  ( TreeNode_t* node, FILE* disk );
TreeErr_t   ReadFromDisk ( Tree_t* tree, const char* filename );
TreeNode_t* ReadNode     ( char* buffer, size_t* pos, TreeErr_t* status, size_t* cpcty );
char*       ReadData     ( char* ptr, size_t* len );

const char* StatusCodeToStr ( TreeErr_t status );
void TreeDump       ( Tree_t* tree, TreeErr_t status, const char* format, ... );
void PrintLogHeader ( Tree_t* tree, FILE* log_file, TreeErr_t status );
int  PrintGraphNodes( TreeNode_t* node, int rank, FILE* graph_text );
void CreateGraphImg ( Tree_t* tree, const char* graphname, const char* graph_dir );

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

#define NODE_INIT_(node_ptr)    \
    node_ptr->left = nullptr;   \
    node_ptr->right = nullptr;  \
    node_ptr->data = nullptr;   \
    node_ptr->parent = nullptr; \
    node_ptr->data_hash = 0;    \
    node_ptr->is_alloc = 1;     \

#define SKIP_SPACE_ while (buffer[*pos] == ' ') (*pos)++;

/*=====================================================================================*/