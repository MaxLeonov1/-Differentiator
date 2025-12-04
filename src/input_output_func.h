#pragma once

#include <stdio.h>

#include "tree.h"
#include "../utils/sup_func.h"
#include "../log_utils/logger.h"
#include "differentiator.h"



DiffErr_t   SaveToDisk   ( Diff_t* diff, int tree_idx, const char* disk_name );
void        WriteEqToFile  ( TreeNode_t* node, TreeNode_t* prev_node, Var_t* name_table, FILE* disk );
DiffErr_t   ReadFromDisk ( Diff_t* diff, const char* filename );
TreeNode_t* GetExpr    (Diff_t* diff, char** str, DiffErr_t* status);