#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "tree.h"
#include "../input_func/input.h"
#include "../utils/sup_func.h"
#include "../log_utils/logger.h"

/*=====================================================================================*/

DiffErr_t SaveToDisk ( Diff_t* diff, int tree_idx, const char* disk_name ) {

    assert(diff);
    assert(diff->forest[tree_idx]);

    if (disk_name == nullptr)
        disk_name = DEF_DISK_NAME_;

    FILE* disk = fopen( disk_name, "wb" );
    if (disk == nullptr) return DiffErr_t::FILE_OPEN_ERR;

    if (!diff->forest[tree_idx]->root) return DiffErr_t::EMPTY_TREE_ACT_ERR;

    WriteToDisk(diff->forest[tree_idx]->root, diff->name_table.buff, disk);

    fclose(disk);

    _RET_OK_

}

/*=====================================================================================*/

void WriteToDisk ( TreeNode_t* node, Var_t* name_table, FILE* disk ) {

    assert(node);
    assert(name_table);
    assert(disk);

    if (node->left)
        WriteToDisk(node->left, name_table, disk);

    switch(node->type) {
        case Node_t::NUM:
            fprintf(disk, " %lg ", node->data.num);
            break;
        case Node_t::OP_BIN:
            fprintf(disk, " %s ", OperInstructions[node->data.oper].name);
            break;
        case Node_t::OP_UN:
            fprintf(disk, " %s ", OperInstructions[node->data.oper].name);
            break;
        case Node_t::VAR:
            fprintf(disk, " %c ", name_table[node->data.var_idx].name);
            break;
    }
    
    if (node->right)
        WriteToDisk(node->right, name_table, disk);

}

/*=====================================================================================*/

DiffErr_t ReadFromDisk (Diff_t* diff, const char* filename ) {

    assert(diff);
    assert(filename);

    _OK_STAT_
    FILE* file = fopen(filename, "rb");
    if (!file) return DiffErr_t::FILE_OPEN_ERR;
    long long byte_num = FileByteCount(filename);

    char* buffer = (char*)calloc((size_t)byte_num + _buff_byte_padding_, sizeof(char));
    char* buff_ptr = buffer; 
    if (!buffer) return DiffErr_t::MEM_ALLOC_ERR;
    fread(buffer, sizeof(buffer[0]), (size_t)byte_num + _buff_byte_padding_, file);

    diff->forest[0]->root = GetG(diff, &buffer, &status);
    if (!diff->forest[0]->root) return status;

    if (status == DiffErr_t::READ_SYNTAX_ERR ) {
        TreeDump(diff, 0, status, buffer);
        free(buff_ptr);
        return status; 

    } else if (status != DiffErr_t::TREE_OK ) {
        free(buff_ptr);
        return status;
    }

    free(buff_ptr);
    _RET_OK_

}

/*=====================================================================================*/
