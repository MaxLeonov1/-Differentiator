#include <assert.h>

#include "tree.h"
#include "../utils/sup_func.h"

/*=====================================================================================*/

TreeErr_t SaveToDisk ( Tree_t* tree, const char* disk_name ) {

    assert(tree);

    if (disk_name == nullptr)
        disk_name = DEF_DISK_NAME_;

    FILE* disk = fopen( disk_name, "wb" );
    if (disk == nullptr) return TreeErr_t::FILE_OPEN_ERR;

    if (!tree->root) return TreeErr_t::EMPTY_TREE_ACT_ERR;

    WriteToDisk(tree->root, disk);

    fclose(disk);

    _RET_OK_

}

/*=====================================================================================*/

void WriteToDisk ( TreeNode_t* node, FILE* disk ) {

    assert(node);
    assert(disk);

    fprintf(disk, "(\"%s\"", node->data);

    if (node->left)
        WriteToDisk(node->left, disk);
    else 
        fprintf(disk, " nil"); 
    
    if (node->right)
        WriteToDisk(node->right, disk);
    else 
        fprintf(disk, " nil");
        
    fprintf(disk, ")");

}

/*=====================================================================================*/

TreeErr_t ReadFromDisk (Tree_t* tree, const char* filename ) {

    assert(tree);
    assert(filename);

    _OK_STAT_

    FILE* file = fopen(filename, "rb");
    if (!file) return TreeErr_t::FILE_OPEN_ERR;
    long long byte_num = FileByteCount(filename);

    tree->buffer = (char*)calloc((size_t)byte_num + _buff_byte_padding_, sizeof(tree->buffer[0]));
    if (!tree->buffer) return TreeErr_t::MEM_ALLOC_ERR;
    fread(tree->buffer, sizeof(tree->buffer[0]), (size_t)byte_num + _buff_byte_padding_, file);

    size_t pos = 0;

    tree->root = ReadNode(tree->buffer, &pos, &status, &tree->cpcty);

    if (status == TreeErr_t::READ_SYNTAX_ERR ) {
        //printf("%d %s\n", pos, tree->buffer);
        TreeDump(tree, status, &tree->buffer[pos]);
        return status; 

    } else if (status != TreeErr_t::TREE_OK )
        return status;

    _RET_OK_

}

/*=====================================================================================*/

TreeNode_t* ReadNode ( char* buffer, size_t* pos, TreeErr_t* status, size_t* cpcty ) {

    if (*status != TreeErr_t::TREE_OK) return nullptr;

    if ( buffer[*pos] == '(' ) {
        SKIP_SPACE_

        TreeNode_t* node = nullptr;
        *status = AllocNode(&node);
        if (*status != TreeErr_t::TREE_OK) return nullptr;
        (*cpcty)++;
        (*pos)++;

        size_t len = 0;

        SKIP_SPACE_
        node->data = ReadData(&buffer[*pos], &len);
        if (!node->data) {
            *status = TreeErr_t::READ_DATA_ERR;
            return nullptr;
        }
        
        (*pos) += len;

        //printf("%s\n", &buffer[*pos]);
        SKIP_SPACE_
        node->left = ReadNode( buffer, pos, status, cpcty);
        SKIP_SPACE_
        node->right = ReadNode( buffer, pos, status, cpcty);
        SKIP_SPACE_
        (*pos)++;
        //printf(" %s\n", &buffer[*pos]);

        if (node->left && node->right) {
            node->left->parent = node;
            node->right->parent = node;
        }
        SKIP_SPACE_
        return node; 

    // } else if (strncmp(&buffer[*pos], "nil", 3) == 0) {

    //     SKIP_SPACE_
    //     (*pos)+=3;
    //     SKIP_SPACE_
    //     *status = TreeErr_t::TREE_OK;
    //     SKIP_SPACE_
    //     return nullptr;

    // } else {

    //     //printf("%c\n", buffer[*pos]);
    //     *status = TreeErr_t::READ_SYNTAX_ERR;
    //     return nullptr;

    // }

}

/*=====================================================================================*/

char* ReadData ( char* ptr, size_t* len ) {

    //printf("%s\n", ptr);
    sscanf( ptr, "\"%*[^\"]\"%n", len );
    *(ptr + *len - 1) = '\0';
    return ptr+1;

}