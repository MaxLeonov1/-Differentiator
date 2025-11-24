#include <stdio.h>
#include <stdlib.h>

#include "dump_tex.h"
#include "logger.h"


void CreateTexLog (Diff_t* diff, TreeNode_t* root ) {

    static int call_num = 0;
    char filename[MAX_STR_LEN_] = "";
    const char* log_dir = GetGlobalLogDir();

    sprintf(filename, "%s%ctex_log.tex", log_dir, PATH_SEP);

    FILE* file = fopen ( filename, "ab" );
    if ( file == nullptr ) exit(0);

    if (call_num == 0) {
        fprintf(file, "\\documentclass{article}\n");
        fprintf(file, "\\usepackage{amsmath}\n");
        fprintf(file, "\\begin{document}\n\n");
    }
    call_num++;
    
    fprintf(file, "%s\n", RndLinkWords[rand()%rndw_num]);
    fprintf(file, "\\begin{equation*} \n");
    TreeDumpTex( root, file, diff);
    fprintf(file, " \n\\end{equation*}\n\n");
    
    fclose(file);

}



void CloseTexLog () {

    char cmd[MAX_STR_LEN_] = "";
    char filename[MAX_STR_LEN_] = "";
    const char* log_dir = GetGlobalLogDir();

    sprintf(filename, "%s%ctex_log.tex", log_dir, PATH_SEP);

    FILE* file = fopen ( filename, "ab" );
    if ( file == nullptr ) exit(0);

    fprintf(file, "\\end{document}\n");
    fclose(file);

    _IF_DEBUG(
    sprintf(cmd, "cd \"%s\" && latexmk -pdf -quiet -silent -f tex_log.tex > /dev/null 2>&1", log_dir);
    system(cmd);
    
    sprintf(cmd, "cd \"%s\" && latexmk -c tex_log.tex > /dev/null 2>&1", log_dir);
    system(cmd);
    )

};



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

    fprintf(file, "%.2lf ", node->data.num);

}



void HandleVar (TreeNode_t* node, FILE* file, Diff_t* diff) {

    fprintf(file, "%c ", diff->name_table.buff[node->data.oper].name);

}



void HandleOpersBin (TreeNode_t* node, FILE* file, Diff_t* diff) {

    if (node->data.oper == Oper_t::DIV) {
        fprintf(file, "\\frac{ ");
        TreeDumpTex(node->left, file, diff);
        fprintf(file, "}{ ");
        TreeDumpTex(node->right, file, diff);
        fprintf(file, "} ");

    } else if (node->data.oper == Oper_t::DEG) {

        TreeDumpTex(node->left, file, diff);
        fprintf(file, "^{ ");
        TreeDumpTex(node->right, file, diff);
        fprintf(file, "} ");

    } else if (node->data.oper == Oper_t::MULT) {

        fprintf(file, "( ");
        TreeDumpTex(node->left, file, diff);
        fprintf(file, "\\cdot ");
        TreeDumpTex(node->right, file, diff);
        fprintf(file, ") ");

    } else {

        fprintf(file, "( ");
        TreeDumpTex(node->left, file, diff);
        fprintf(file, "%s ", diff->def_op_instr[node->data.oper].name);
        TreeDumpTex(node->right, file, diff);
        fprintf(file, ") ");
    }

}



void HandleOpersUn (TreeNode_t* node, FILE* file, Diff_t* diff) {

    fprintf(file, " \\%s(", diff->def_op_instr[node->data.oper].name);
    TreeDumpTex(node->right, file, diff);
    fprintf(file, " )");

}