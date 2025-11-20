#pragma once

#include <stdio.h>

#include "tree.h"



void TreeDumpTex    (TreeNode_t* node, FILE* file, Diff_t* diff);
void HandleNum      (TreeNode_t* node, FILE* file);
void HandleVar      (TreeNode_t* node, FILE* file, Diff_t* diff);
void HandleOpersBin (TreeNode_t* node, FILE* file, Diff_t* diff);
void HandleOpersUn  (TreeNode_t* node, FILE* file, Diff_t* diff);
void CreateTexLog   (Diff_t* diff, const char* filename);