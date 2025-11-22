#include <stdio.h>
#include <assert.h>

#include "differentiator.h"
#include "tree.h"
#include "../utils/sup_func.h"

/*=====================================================================================*/

int compare ( const void* cmd_1, const void* cmd_2 ) {
    
    int hash_1 = ((OperInstr_t*)cmd_1)->hash;
    int hash_2 = ((OperInstr_t*)cmd_2)->hash;

    return hash_1 - hash_2;

}

/*=====================================================================================*/

void SortInstrByHash ( Diff_t* diff ) {

    for ( size_t i = 0; i < diff->op_num; i++ )
        diff->sort_op_instr[i] = diff->def_op_instr[i];

    qsort ( diff->sort_op_instr, diff->op_num, sizeof(diff->sort_op_instr[0]), compare );

}

/*=====================================================================================*/

void HashAndCopyInstr ( OperInstr_t* Instr_Def, Diff_t* diff ) {

    for ( size_t i = 0; i < diff->op_num; i++ ) {
        
        diff->def_op_instr[i] = Instr_Def[i];
        const char* name = diff->def_op_instr[i].name;
        diff->def_op_instr[i].hash = djb2hash(name);

    }

}

/*=====================================================================================*/

int HashBinSearch ( OperInstr_t* Instr, int instr_size, int hash ) {

    int left = 0;
    int right = instr_size - 1;

    while ( left <= right ) {

        int mid = ( left + right ) / 2;

        if ( Instr[mid].hash == hash )
            return mid;

        else if ( Instr[mid].hash < hash )
            left = mid + 1;
        
        else
            right = mid - 1;

    }

    return -1;

}

/*=====================================================================================*/

DiffErr_t DiffCtor ( Diff_t* diff ) {

    diff->op_num = sizeof(OperInstructions)/sizeof(OperInstructions[0]);


    diff->forest = (Tree_t**)calloc(10, sizeof(Tree_t*));

    diff->def_op_instr = (OperInstr_t*)calloc(diff->op_num, sizeof(OperInstr_t));
    diff->sort_op_instr = (OperInstr_t*)calloc(diff->op_num, sizeof(OperInstr_t));

    HashAndCopyInstr(OperInstructions, diff);
    SortInstrByHash(diff);

    if (!diff->def_op_instr ||
        !diff->sort_op_instr)
        return DiffErr_t::MEM_ALLOC_ERR;

    _RET_OK_
    
}

/*=====================================================================================*/

void DiffDtor ( Diff_t* diff ) {

    free(diff->def_op_instr);
    free(diff->sort_op_instr);

    for (size_t idx = 0; idx<diff->tree_num; idx++) {
        TreeDtor(diff->forest[idx]);
    }
    free(diff->forest);

    if (diff->name_table.buff) {
        for (size_t idx = 0; idx<diff->name_table.num; idx++) {
            free(diff->name_table.buff[idx].name);
        }
        free(diff->name_table.buff);
    }

}

/*=====================================================================================*/

DiffErr_t AddToNameTable ( Diff_t* diff, const char* name ) {

    assert(diff);

    if (diff->name_table.num+1 >= diff->name_table.size) {

        diff->name_table.size = (diff->name_table.size + 1)*2;
        diff->name_table.buff = (Var_t*)realloc(diff->name_table.buff, sizeof(diff->name_table.buff[0])*diff->name_table.size);
        if (!diff->name_table.buff)
            return DiffErr_t::MEM_ALLOC_ERR;
    }

    diff->name_table.buff[diff->name_table.num].name = my_strdup(name);
    diff->name_table.num++;

    _RET_OK_
}

/*=====================================================================================*/

TreeNode_t* CreateNumNode (double val, TreeNode_t* prev_node) {

    TreeNode_t* node = (TreeNode_t*)calloc(1, sizeof(TreeNode_t));
    if (!node) return node;

    node->data.num = val;
    node->parent = prev_node;
    node->left = nullptr;
    node->right = nullptr;
    node->type = Node_t::NUM;

    return node;
}

/*=====================================================================================*/

TreeNode_t* CreateBinOp (Oper_t oper, TreeNode_t* left, TreeNode_t* right, TreeNode_t* prev_node) {

    TreeNode_t* node = (TreeNode_t*)calloc(1, sizeof(TreeNode_t));
    if (!node) return node;

    node->data.oper = oper;
    node->parent = prev_node;
    node->left = left;
    node->right = right;
    node->type = Node_t::OP_BIN;

    return node;
}
 
/*=====================================================================================*/

TreeNode_t* CreateUnOp (Oper_t oper, TreeNode_t* left, TreeNode_t* prev_node) {

    TreeNode_t* node = (TreeNode_t*)calloc(1, sizeof(TreeNode_t));
    if (!node) return node;

    node->data.oper = oper;
    node->parent = prev_node;
    node->left = left;
    node->right = nullptr;
    node->type = Node_t::OP_UN;

    return node;
}
