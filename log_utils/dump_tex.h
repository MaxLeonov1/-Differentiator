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



void PrintTexLogHeader  ();
void TreeDumpTex        (TreeNode_t* node, TreeNode_t* prev_node, FILE* file, Diff_t* diff);
void PrintSectionHeaderToTex(const char* title);
void PrintMesAndEqToTex (Diff_t* diff, TreeNode_t* root, const char* mes );
void PrintMesAndTaylorEqToTex (Diff_t* diff, TreeNode_t* root, const char* mes,
                               double x0, int deg);
void CreateTexLog       (Diff_t* diff, TreeNode_t* root, TreeNode_t* d_root);
void PutImgToLog        (Diff_t* diff, const char* img_filename );
void CloseTexLog        ();