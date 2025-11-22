#pragma once

#include "differentiator.h"



TreeNode_t* SimplConstTree ( TreeNode_t* node);
double      EvalConstTree  ( TreeNode_t* node);
int         ConstTreeCheck ( TreeNode_t* node);
DiffErr_t   SimplTree(Diff_t* diff, int tree_idx);