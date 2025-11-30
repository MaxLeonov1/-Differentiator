#pragma once

#include "tree.h"



TreeNode_t* Differentiate  ( TreeNode_t* node, TreeNode_t* prev_node, int dir_var, Diff_t* diff);
DiffErr_t FindDerivative   ( Diff_t* diff, int inp_eq_idx, int dir_var, int dir_deg, DiffErr_t* status );
DiffErr_t   FindValue      ( Diff_t* diff, int tree_idx);
void        AskVarVal      ( Diff_t* diff);
double      CalcTree       (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);