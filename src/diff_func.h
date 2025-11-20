#pragma once

#include "tree.h"



TreeNode_t* Differentiate  ( TreeNode_t* node, TreeNode_t* prev_node, size_t dir_var);
TreeErr_t   FindDerivative ( Tree_t* tree, Tree_t* new_tree, size_t dir_var );
TreeErr_t   FindValue      ( Diff_t* diff, int tree_idx);
void        AskVarVal      ( Diff_t* diff);
double      CalcTree       (TreeNode_t* node, Diff_t* diff );