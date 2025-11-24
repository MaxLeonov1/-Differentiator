#pragma once

#include "differentiator.h"



TreeNode_t* SimplConstTree   ( TreeNode_t* node);
int         TryEvalConstTree ( TreeNode_t* node, double* result);
TreeNode_t* SimplTrivleCases ( TreeNode_t* node);
DiffErr_t   SimplTree        ( Diff_t* diff, int tree_idx);