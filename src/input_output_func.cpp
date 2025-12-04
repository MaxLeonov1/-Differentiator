#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "input_output_func.h"

/*=====================================================================================*/

DiffErr_t SaveToDisk ( Diff_t* diff, int tree_idx, const char* disk_name ) {

    assert(diff);
    assert(diff->forest[tree_idx]);

    if (disk_name == nullptr)
        disk_name = DEF_DISK_NAME_;

    FILE* disk = fopen( disk_name, "wb" );
    if (disk == nullptr) return DiffErr_t::FILE_OPEN_ERR;

    if (!diff->forest[tree_idx]->root) return DiffErr_t::EMPTY_TREE_ACT_ERR;

    WriteEqToFile(diff->forest[tree_idx]->root, nullptr, diff->name_table.buff, disk);

    fclose(disk);

    _RET_OK_

}

/*=====================================================================================*/

#define _Wl if(node->left)  WriteEqToFile(node->left, node, name_table, disk);
#define _Wr if(node->right) WriteEqToFile(node->right, node, name_table, disk);
#define _IF_PR_LOW(source) if (node->data.oper == Oper_t::ADD || node->data.oper == Oper_t::SUB) {source}

void WriteEqToFile ( TreeNode_t* node, TreeNode_t* prev_node, Var_t* name_table, FILE* disk ) {

    assert(node);
    assert(name_table);
    assert(disk);

    int parent_priority = 0, node_priority = 0;
    node_priority = GetOperPriority(node->data.oper);
    if (prev_node)
        parent_priority = GetOperPriority(prev_node->data.oper);

    switch(node->type) {
        case Node_t::NUM:
            fprintf(disk, "%lg ", node->data.num);
            break;
        case Node_t::OP_BIN:
            if (parent_priority > node_priority) fprintf(disk, "(");
            _Wl
            fprintf(disk, "%s ", OperInstructions[node->data.oper].name);
            _Wr
            if (parent_priority > node_priority) fprintf(disk, ")");
            break;
        case Node_t::OP_UN:
            fprintf(disk, "%s(", OperInstructions[node->data.oper].name);
            _Wr
            fprintf(disk, ")");
            break;
        case Node_t::VAR:
            fprintf(disk, "%c ", name_table[node->data.var_idx].name);
            break;
    }
}

#undef _Wl
#undef _Wr
#undef _IF_PR_LOW

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

    diff->forest[0]->root = GetExpr(diff, &buffer, &status);
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
