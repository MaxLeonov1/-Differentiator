#include <stdio.h>
#include <stdlib.h>

#include "dump_tex.h"



void CreateTexLog (Diff_t* diff, int tree_idx, const char* filename) {

    FILE* file = fopen ( filename, "wb" );
    if ( file == nullptr ) exit(0);

    fprintf(file, "\\documentclass{article}\n");
    fprintf(file, "\\usepackage{amsmath}\n");
    fprintf(file, "\\usepackage[utf8]{inputenc}\n");
    fprintf(file, "\\usepackage[T2A]{fontenc}\n");
    fprintf(file, "\\begin{document}\n\n");
    
    fprintf(file, "Equation:\n");
    fprintf(file, "\\begin{equation*} \n");
    TreeDumpTex(diff->forest[tree_idx]->root, file, diff);
    fprintf(file, " \\end{equation*}\n\n");
    
    fprintf(file, "\\end{document}\n");
    fclose(file);

}



void TreeDumpTex (TreeNode_t* node, FILE* file, Diff_t* diff) {

    if (!node) return;

    if (node->type == Node_t::NUM)
        HandleNum(node, file);

    if (node->type == Node_t::VAR)
        HandleVar(node, file, diff);

    if (node->type == Node_t::OP_BIN)
        HandleOpersBin(node, file, diff);

    if (node->type == Node_t::OP_UN)
        HandleOpersUn(node, file, diff);

}



int get_priority(Oper_t oper) {

    switch(oper) {
        case Oper_t::DEG:
            return 3;
        case Oper_t::MULT: case Oper_t::DIV:
            return 2;
        default:
            return 1;

    }

}



void HandleNum (TreeNode_t* node, FILE* file) {

    fprintf(file, "%.2lf", node->data.num);

}



void HandleVar (TreeNode_t* node, FILE* file, Diff_t* diff) {

    fprintf(file, "%s", diff->name_table.buff[node->data.oper].name);

}



void HandleOpersBin (TreeNode_t* node, FILE* file, Diff_t* diff) {

    if (node->data.oper == Oper_t::DIV) {
        fprintf(file, "\\frac{");
        TreeDumpTex(node->left, file, diff);
        fprintf(file, "}{");
        TreeDumpTex(node->right, file, diff);
        fprintf(file, "}");

    } else if (node->data.oper == Oper_t::DEG) {

        TreeDumpTex(node->left, file, diff);
        fprintf(file, "^{");
        TreeDumpTex(node->right, file, diff);
        fprintf(file, "}");

    } else if (node->data.oper == Oper_t::MULT) {

        fprintf(file, "(");
        TreeDumpTex(node->left, file, diff);
        fprintf(file, "\\cdot");
        TreeDumpTex(node->right, file, diff);
        fprintf(file, ")");

    } else {

        fprintf(file, "(");
        TreeDumpTex(node->left, file, diff);
        fprintf(file, "%s", diff->def_op_instr[node->data.oper].name);
        TreeDumpTex(node->right, file, diff);
        fprintf(file, ")");
    }

}



void HandleOpersUn (TreeNode_t* node, FILE* file, Diff_t* diff) {

    fprintf(file, "\\%s(", diff->def_op_instr[node->data.oper].name);
    TreeDumpTex(node->left, file, diff);
    fprintf(file, ")");

}