#pragma once

#include "tree.h"
#include "differentiator.h"
#include "sup_func.h"



void TreeDump       ( Diff_t* diff, int tree_idx, DiffErr_t status, const char* format, ... );
void PrintLogHeader ( Tree_t* tree, FILE* log_file, DiffErr_t status );
int  PrintGraphNodes( TreeNode_t* node, int rank, FILE* graph_text, Diff_t* diff );
void CreateGraphImg ( Diff_t* diff, int tree_idx, const char* graphname, const char* graph_dir );