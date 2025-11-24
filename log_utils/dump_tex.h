#pragma once

#include <stdio.h>

#include "tree.h"


static const int rndw_num = 9;
static const char* RndLinkWords[] = {
    "Watch out - derivatives coming through!",
    "Unleashing the power of calculus!",
    "And after some diffirentiation:",
    "This function is about to get derived!",
    "Better watch TikTok then do this:",
    "Why are you reading this?",
    "Look what we have here!",
    "Transforming this **** into another ****:",
    "New derivatives incomming!!!"
};



void TreeDumpTex    (TreeNode_t* node, FILE* file, Diff_t* diff);
void HandleNum      (TreeNode_t* node, FILE* file);
void HandleVar      (TreeNode_t* node, FILE* file, Diff_t* diff);
void HandleOpersBin (TreeNode_t* node, FILE* file, Diff_t* diff);
void HandleOpersUn  (TreeNode_t* node, FILE* file, Diff_t* diff);
void CreateTexLog   (Diff_t* diff, TreeNode_t* root);
void CloseTexLog ();