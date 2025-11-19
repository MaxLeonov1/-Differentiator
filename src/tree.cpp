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

TreeErr_t TreeDtor ( Tree_t* tree ) {

    assert(tree);

    if (!tree->root) return TreeErr_t::EMPTY_TREE_ACT_ERR;
    DeleteNode(tree->root);
    free(tree->buffer);

    _RET_OK_

}

/*=====================================================================================*/

TreeErr_t AllocNode ( TreeNode_t** node, Node_t node_t, Val_t value_t ) {

    assert(node);

    TreeNode_t* node_ptr = (TreeNode_t*)calloc(1, sizeof(node_ptr[0]));
    if ( !node_ptr ) return TreeErr_t::MEM_ALLOC_ERR;

    node_ptr->data.num = 0;
    node_ptr->type = node_t;
    node_ptr->left = nullptr;
    node_ptr->right = nullptr;
    node_ptr->parent = nullptr;

    *node = node_ptr; 

    _RET_OK_

}

/*=====================================================================================*/

TreeErr_t DeleteNode ( TreeNode* node ) {

    assert(node != nullptr);

    if (node->left != nullptr)
        DeleteNode (node->left);
    if (node->right != nullptr)
        DeleteNode (node->right);

    free(node);

    _RET_OK_

}

/*=====================================================================================*/

TreeErr_t InsertNode ( TreeNode_t** node, Val_t* value, Node_t node_t, TreeNode_t* prev_node ) {

    assert(node);
    _OK_STAT_

    TreeNode_t* node_new = nullptr;
    status = AllocNode(&node_new, node_t, *value);
    TREE_STAT_CHECK_

    switch(node_t) {

        case Node_t::NUM:
            node_new->data.num = value->num;
        case Node_t::VAR:
            node_new->data.var_idx = value->var_idx;
        case Node_t::OP:
            node_new->data.oper = value->oper;
    }

    node_new->parent = prev_node;
    *node = node_new;

    _RET_OK_

}

/*=====================================================================================*/

// TreeErr_t InsertNodeAfter ( TreeNode_t* node, TreeElem_t elem, int child ) {

//     assert(node);
//     _OK_STAT_

//     if ( 
//         node->left && child == _left_ &&
//         node->right && child == _right_
//     )
//         return TreeErr_t::INSERT_EX_POS_ERR;

//     if ( child == _left_ ) {
//         status = InsertNode( &node->left, elem, node );
//         TREE_STAT_CHECK_
//     } else {
//         status = InsertNode( &node->right, elem, node );
//         TREE_STAT_CHECK_
//     }     
    
//     _RET_OK_

// }

