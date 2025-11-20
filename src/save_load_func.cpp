#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "tree.h"
#include "../utils/sup_func.h"

/*=====================================================================================*/

TreeErr_t SaveToDisk ( Diff_t* diff, int tree_idx, const char* disk_name ) {

    assert(diff);
    assert(diff->forest[tree_idx]);

    if (disk_name == nullptr)
        disk_name = DEF_DISK_NAME_;

    FILE* disk = fopen( disk_name, "wb" );
    if (disk == nullptr) return TreeErr_t::FILE_OPEN_ERR;

    if (!diff->forest[tree_idx]->root) return TreeErr_t::EMPTY_TREE_ACT_ERR;

    WriteToDisk(diff->forest[tree_idx]->root, diff->name_table.buff, disk);

    fclose(disk);

    _RET_OK_

}

/*=====================================================================================*/

void WriteToDisk ( TreeNode_t* node, Var_t* name_table, FILE* disk ) {

    assert(node);
    assert(disk);

    char data[MAX_STR_LEN_] = "";

    switch(node->type) {
        case Node_t::NUM:
            fprintf(disk, "( \"%lf\" ", node->data.num);
            break;
        case Node_t::OP_BIN:
            fprintf(disk, "( \"%s\" ", OperInstructions[node->data.oper].name);
            break;
        case Node_t::OP_UN:
            fprintf(disk, "( \"%s\" ", OperInstructions[node->data.oper].name);
            break;
        case Node_t::VAR:
            fprintf(disk, "( \"%s\" ", name_table[node->data.var_idx].name);
            break;
    }

    if (node->left)
        WriteToDisk(node->left, name_table, disk);
    else 
        fprintf(disk, " nil"); 
    
    if (node->right)
        WriteToDisk(node->right, name_table, disk);
    else 
        fprintf(disk, " nil");
        
    fprintf(disk, ")");

}

/*=====================================================================================*/

TreeErr_t ReadFromDisk (Diff_t* diff, const char* filename ) {

    assert(diff);
    assert(filename);

    _OK_STAT_

    FILE* file = fopen(filename, "rb");
    if (!file) return TreeErr_t::FILE_OPEN_ERR;
    long long byte_num = FileByteCount(filename);

    diff->forest[0]->buffer = (char*)calloc((size_t)byte_num + _buff_byte_padding_, sizeof(diff->forest[0]->buffer[0]));
    if (!diff->forest[0]->buffer) return TreeErr_t::MEM_ALLOC_ERR;
    fread(diff->forest[0]->buffer, sizeof(diff->forest[0]->buffer[0]), (size_t)byte_num + _buff_byte_padding_, file);

    size_t pos = 0;

    diff->forest[0]->root = ReadNode(diff->forest[0]->buffer, &pos, &status, diff);

    if (status == TreeErr_t::READ_SYNTAX_ERR ) {
        //printf("%d %s\n", pos, tree->buffer);
        TreeDump(diff, 0, status, &diff->forest[0]->buffer[pos]);
        return status; 

    } else if (status != TreeErr_t::TREE_OK )
        return status;

    _RET_OK_

}

/*=====================================================================================*/

TreeNode_t* ReadNode ( char* buffer, size_t* pos, TreeErr_t* status, Diff_t* diff ) {

    if (*status != TreeErr_t::TREE_OK) return nullptr;

    if ( buffer[*pos] == '(' ) {
        (*pos)++;
        SKIP_SPACE_

        // (*cpcty)++;
        // (*pos)++;

        size_t len = 0;

        SKIP_SPACE_
        char* temp_data = ReadData(&buffer[*pos], &len);
        if (!temp_data) {
            *status = TreeErr_t::READ_DATA_ERR;
            return nullptr;
        }
        (*pos) += len;

        TreeNode_t* node = nullptr;
        *status = CreateNodeFromStr(temp_data, diff, &node);
        if (*status != TreeErr_t::TREE_OK)
            return nullptr;

        diff->forest[0]->cpcty++;

        //printf("%s\n", &buffer[*pos]);
        SKIP_SPACE_
        node->left = ReadNode( buffer, pos, status, diff);
        SKIP_SPACE_
        node->right = ReadNode( buffer, pos, status, diff);
        SKIP_SPACE_
        (*pos)++;
        //printf(" %s\n", &buffer[*pos]);

        if (node->left && node->right) {
            node->left->parent = node;
            node->right->parent = node;
        }
        SKIP_SPACE_
        return node; 

    } else if (strncmp(&buffer[*pos], "nil", 3) == 0) {

        SKIP_SPACE_
        (*pos)+=3;
        SKIP_SPACE_
        *status = TreeErr_t::TREE_OK;
        SKIP_SPACE_
        return nullptr;

    } else {

        //printf("%c\n", buffer[*pos]);
        *status = TreeErr_t::READ_SYNTAX_ERR;
        return nullptr;

    }

}

/*=====================================================================================*/

TreeErr_t CreateNodeFromStr ( const char* str, Diff_t* diff, TreeNode_t** new_node ) {

    _OK_STAT_

    for (size_t i = 0; i < diff->op_num; i++) {

        if (strcmp(str, diff->def_op_instr[i].name) == 0) {

            Node_t type = Node_t::OP_BIN;
            if (diff->def_op_instr[i].is_sing)
                type = Node_t::OP_UN;

            TreeNode_t* node = nullptr;
            status = AllocNode(&node, type);
            TREE_STAT_CHECK_
            
            node->data.oper = diff->def_op_instr[i].code;
            *new_node = node;
            _RET_OK_
        }
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
        
        status = AddToNameTable(diff, str);
        TREE_STAT_CHECK_
        
        TreeNode_t* node = nullptr;
        status = AllocNode(&node, Node_t::NUM);
        TREE_STAT_CHECK_
        
        node->type = Node_t::VAR;
        node->data.var_idx = diff->name_table.num - 1;
        *new_node = node;
        _RET_OK_
    }

    return TreeErr_t::READ_DATA_ERR;

}

/*=====================================================================================*/

char* ReadData ( char* ptr, size_t* len ) {

    //printf("%s\n", ptr);
    sscanf( ptr, "\"%*[^\"]\"%n", len );
    *(ptr + *len - 1) = '\0';
    return ptr+1;

}