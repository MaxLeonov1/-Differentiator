#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "tree.h"
#include "differentiator.h"
#include "../utils/sup_func.h"


#pragma GCC diagnostic ignored "-Wformat=2"

/*=====================================================================================*/

void TreeCtor ( Tree_t* tree ) {

    assert(tree);
    
    tree->root = nullptr;
    tree->cpcty = 1;

}

/*=====================================================================================*/

DiffErr_t TreeDtor ( Tree_t* tree ) {

    assert(tree);

    if (!tree->root) return DiffErr_t::EMPTY_TREE_ACT_ERR;
    DeleteNode(tree->root);

    _RET_OK_

}

/*=====================================================================================*/

DiffErr_t AllocNode ( TreeNode_t** node, Node_t node_t ) {

    assert(node);

    TreeNode_t* node_ptr = (TreeNode_t*)calloc(1, sizeof(node_ptr[0]));
    if ( !node_ptr ) return DiffErr_t::MEM_ALLOC_ERR;

    node_ptr->data.num = 0;
    node_ptr->type = node_t;
    node_ptr->left = nullptr;
    node_ptr->right = nullptr;

    *node = node_ptr; 

    _RET_OK_

}

/*=====================================================================================*/

DiffErr_t DeleteNode ( TreeNode* node ) {

    assert(node != nullptr);

    if (node->left != nullptr)
        DeleteNode (node->left);
    if (node->right != nullptr)
        DeleteNode (node->right);

    free(node);

    _RET_OK_

}

/*=====================================================================================*/

DiffErr_t InsertNode ( TreeNode_t** node, Val_t value, Node_t node_t ){

    assert(node);
    _OK_STAT_

    TreeNode_t* node_new = nullptr;
    status = AllocNode(&node_new, node_t);
    TREE_STAT_CHECK_

    switch(node_t) {

        case Node_t::NUM:
            node_new->data.num = value.num;
            break;
        case Node_t::VAR:
            node_new->data.var_idx = value.var_idx;
            break;
        case Node_t::OP_BIN:
            node_new->data.oper = value.oper;
            break;
        case Node_t::OP_UN:
            node_new->data.oper = value.oper;
            break;
    }

    *node = node_new;

    _RET_OK_

}

/*=====================================================================================*/

TreeNode_t* CopyTree ( TreeNode_t* node) {

    if (node == nullptr) return nullptr;

    TreeNode_t* new_node = (TreeNode_t*)calloc(1, sizeof(TreeNode_t));
    if (new_node == nullptr) return new_node;

    new_node->data.num = node->data.num;
    new_node->type = node->type;

    if(node->left != nullptr)
        new_node->left = CopyTree (node->left);
    if(node->right != nullptr)
        new_node->right = CopyTree (node->right);

    return new_node;

}

/*=====================================================================================*/

int AddEquation (Diff_t* diff, DiffErr_t* status) {

    Tree_t* eq = (Tree_t*)calloc(1, sizeof(Tree_t));
    if (!eq) {
        *status = DiffErr_t::MEM_ALLOC_ERR;
        return -1;
    }

    (*eq).cpcty = 0;
    _IF_DEBUG(
    (*eq).info.file = __FILE__;
    (*eq).info.func = __FUNCTION__;
    (*eq).info.line = __LINE__;
    )
    (*eq).root = nullptr;

    TreeCtor(eq);

    if (diff->max_eq_num <= diff->tree_num - 1) {

        diff->max_eq_num = diff->max_eq_num*2;
        diff->forest = (Tree_t**)realloc(diff->forest, diff->max_eq_num*sizeof(diff->forest[0]));
        if (!diff->forest) {
            *status = DiffErr_t::MEM_ALLOC_ERR;
            return -1;
        }
    }

    diff->forest[diff->tree_num] = eq;

    diff->tree_num++;

    return diff->tree_num - 1;
}

/*=====================================================================================*/

TreeNode_t* CreateVarNode (int var_idx) {

    TreeNode_t* node = (TreeNode_t*)calloc(1, sizeof(TreeNode_t));
    if (!node) return node;

    node->data.var_idx = var_idx;
    node->left = nullptr;
    node->right = nullptr;
    node->type = Node_t::VAR;

    return node;
}

/*=====================================================================================*/

TreeNode_t* CreateNumNode (double val) {

    TreeNode_t* node = (TreeNode_t*)calloc(1, sizeof(TreeNode_t));
    if (!node) return node;

    node->data.num = val;
    node->left = nullptr;
    node->right = nullptr;
    node->type = Node_t::NUM;

    return node;
}

/*=====================================================================================*/

TreeNode_t* CreateBinOp (Oper_t oper, TreeNode_t* left, TreeNode_t* right) {

    TreeNode_t* node = (TreeNode_t*)calloc(1, sizeof(TreeNode_t));
    if (!node) return node;

    node->data.oper = oper;
    node->left = left;
    node->right = right;
    node->type = Node_t::OP_BIN;

    return node;
}
 
/*=====================================================================================*/

TreeNode_t* CreateUnOp (Oper_t oper, TreeNode_t* right) {

    TreeNode_t* node = (TreeNode_t*)calloc(1, sizeof(TreeNode_t));
    if (!node) return node;

    node->data.oper = oper;
    node->left = nullptr;
    node->right = right;
    node->type = Node_t::OP_UN;

    return node;
}
