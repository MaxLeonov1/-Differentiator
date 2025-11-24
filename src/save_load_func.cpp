#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "tree.h"
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

    fprintf(disk, "(");

    if (node->left)
        WriteToDisk(node->left, name_table, disk);

    switch(node->type) {
        case Node_t::NUM:
            fprintf(disk, " %.2lf ", node->data.num);
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
        
    fprintf(disk, ")");

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
    if (!buffer) return DiffErr_t::MEM_ALLOC_ERR;
    fread(buffer, sizeof(buffer[0]), (size_t)byte_num + _buff_byte_padding_, file);

    size_t pos = 0;

    diff->forest[0]->root = ReadNode(buffer, &pos, &status, diff);
    if (!diff->forest[0]->root) return status;

    if (status == DiffErr_t::READ_SYNTAX_ERR ) {
        //printf("%d %s\n", pos, tree->buffer);
        TreeDump(diff, 0, status, &buffer[pos]);
        free(buffer);
        return status; 

    } else if (status != DiffErr_t::TREE_OK ) {
        free(buffer);
        return status;
    }

    free(buffer);
    _RET_OK_

}

/*=====================================================================================*/

TreeNode_t* ReadNode(char* buffer, size_t* pos, DiffErr_t* status, Diff_t* diff) {
    if (*status != DiffErr_t::TREE_OK) return nullptr;

    SKIP_SPACE_

    if (buffer[*pos] != '(') {
        *status = DiffErr_t::READ_SYNTAX_ERR;
        return nullptr;
    }
    (*pos)++;
    SKIP_SPACE_

    TreeNode_t* left = nullptr;
    if (buffer[*pos] == '(') {
        left = ReadNode(buffer, pos, status, diff);
        if (*status != DiffErr_t::TREE_OK) return nullptr;
    }
    SKIP_SPACE_

    char temp_data[MAX_STR_LEN_] = "";
    size_t len = 0;
    
    while (buffer[*pos] != ' ' && buffer[*pos] != ')' && buffer[*pos] != '\0' ) {
        temp_data[len++] = buffer[*pos];
        (*pos)++;
    }
    temp_data[len] = '\0';
    
    if (len == 0) {
        *status = DiffErr_t::READ_DATA_ERR;
        return nullptr;
    }

    SKIP_SPACE_

    TreeNode_t* right = nullptr;
    if (buffer[*pos] == '(') {
        right = ReadNode(buffer, pos, status, diff);
        if (*status != DiffErr_t::TREE_OK) return nullptr;
    }
    SKIP_SPACE_

    (*pos)++; //for ")"
    SKIP_SPACE_

    TreeNode_t* node = nullptr;
    *status = CreateNodeFromStr(temp_data, diff, &node);
    if (*status != DiffErr_t::TREE_OK) return nullptr;

    diff->forest[0]->cpcty++;

    if (node->type == Node_t::OP_BIN) {
        node->left = left;
        node->right = right;
        
        if (left) left->parent = node;
        if (right) right->parent = node;
    }
    else if (node->type == Node_t::OP_UN) {
        node->left = nullptr;
        node->right = right;
        
        if (right) right->parent = node;
        
        if (left != nullptr) {
            *status = DiffErr_t::READ_SYNTAX_ERR;
            DeleteNode(node);
            return nullptr;
        }
    }
    else if (node->type == Node_t::NUM || node->type == Node_t::VAR) {
        if (left != nullptr || right != nullptr) {
            *status = DiffErr_t::READ_SYNTAX_ERR;
            DeleteNode(node);
            return nullptr;
        }
    }

    return node;
}

/*=====================================================================================*/

DiffErr_t CreateNodeFromStr ( const char* str, Diff_t* diff, TreeNode_t** new_node ) {

    _OK_STAT_

    int str_hash = djb2hash(str);
    int idx = HashBinSearch(diff->sort_op_instr, diff->op_num, str_hash);

    if (idx != -1) {

        Node_t type = Node_t::OP_BIN;
        if (diff->sort_op_instr[idx].is_sing)
            type = Node_t::OP_UN;

        TreeNode_t* node = nullptr;
        status = AllocNode(&node, type);
        TREE_STAT_CHECK_
                
        node->data.oper = diff->sort_op_instr[idx].code;
        *new_node = node;
        _RET_OK_
    }

    char* endptr = nullptr;
    double value = strtod(str, &endptr);
    if (*endptr == '\0') {

        TreeNode_t* node = nullptr;
        status = AllocNode(&node, Node_t::NUM);
        TREE_STAT_CHECK_
        
        node->data.num = value;
        *new_node = node;
        _RET_OK_
    }

    if (strlen(str) == 1 && isalpha(str[0])) {

        int var_idx = -1;
        for (size_t i = 0; i < diff->name_table.num; i++) {
            if (diff->name_table.buff[i].name == str[0]) {
                var_idx = (int)i;
                break;
            }
        }
        
        if (var_idx == -1)
            status = AddToNameTable(diff, str[0]);
        TREE_STAT_CHECK_
        
        TreeNode_t* node = nullptr;
        status = AllocNode(&node, Node_t::VAR);
        TREE_STAT_CHECK_
        
        node->data.var_idx = diff->name_table.num - 1;
        *new_node = node;
        _RET_OK_
    }

    return DiffErr_t::READ_DATA_ERR;

}

/*=====================================================================================*/
