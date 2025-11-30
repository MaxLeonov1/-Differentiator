#include <stdio.h>
#include <stdlib.h>

#include "dump_tex.h"
#include "logger.h"



static FILE* GetTexFile() {

    char filename[MAX_STR_LEN_] = "";                         
    const char* log_dir = GetGlobalLogDir();                  
    sprintf(filename, "%s%ctex_log.tex", log_dir, PATH_SEP); 

    FILE* file = fopen ( filename, "ab" );                    
    return file;                       

}



void PrintTexLogHeader() {

    FILE* file = GetTexFile();
    if ( file == nullptr ) return;

    fprintf(file, "\\documentclass{article}\n");
    fprintf(file, "\\usepackage{amsmath}\n");
    fprintf(file, "\\usepackage{breqn}\n");
    fprintf(file, "\\begin{document}\n\n");

    fclose(file);

}



void PrintMesAndEqToTex (Diff_t* diff, TreeNode_t* root, const char* mes ) {

    FILE* file = GetTexFile();
    if ( file == nullptr ) return;

    //fprintf(file, "Let's find derivative for: [i don't want to do this]\n");
    fprintf(file, "%s\n", mes);
    fprintf(file, "\\begin{dmath*} \n");
    TreeDumpTex( root, file, diff);
    fprintf(file, " \n\\end{dmath*}\n\n");
    
    fclose(file);


}



void CreateTexLog (Diff_t* diff, TreeNode_t* root, TreeNode_t* d_root ) {

    FILE* file = GetTexFile();
    if ( file == nullptr ) return;
    
    fprintf(file, "%s\n", RndLinkWords[rand()%rndw_num]);
    fprintf(file, "\\begin{dmath*} \n");
    fprintf(file, "\\frac{d}{dx}(");
    TreeDumpTex( d_root, file, diff);
    fprintf(file, ") = ");
    TreeDumpTex( root, file, diff);
    fprintf(file, " \n\\end{dmath*}\n\n");
    
    fclose(file);

}



void CloseTexLog () {

    char cmd[MAX_STR_LEN_] = "";
    const char* log_dir = GetGlobalLogDir();

    FILE* file = GetTexFile();
    if ( file == nullptr ) return;

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

        fprintf(file, "( ");
        TreeDumpTex(node->left, file, diff);
        fprintf(file, ")^{ ");
        TreeDumpTex(node->right, file, diff);
        fprintf(file, "} ");

    } else if (node->data.oper == Oper_t::MULT) {

        TreeDumpTex(node->left, file, diff);
        fprintf(file, "\\cdot ");
        TreeDumpTex(node->right, file, diff);

    } else {

        TreeDumpTex(node->left, file, diff);
        fprintf(file, "%s ", diff->def_op_instr[node->data.oper].name);
        TreeDumpTex(node->right, file, diff);
    }

}



void HandleOpersUn (TreeNode_t* node, FILE* file, Diff_t* diff) {

    fprintf(file, " \\%s(", diff->def_op_instr[node->data.oper].name);
    TreeDumpTex(node->right, file, diff);
    fprintf(file, " )");

}   