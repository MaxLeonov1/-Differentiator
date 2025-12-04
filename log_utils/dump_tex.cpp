#include <stdio.h>
#include <stdlib.h>

#include "dump_tex.h"
#include "logger.h"



static void HandleNum          (TreeNode_t* node, FILE* file);
static void HandleVar          (TreeNode_t* node, FILE* file, Diff_t* diff);
static void HandleOpersBin     (TreeNode_t* node, TreeNode_t* prev_node, FILE* file, Diff_t* diff);
static void HandleOpersUn      (TreeNode_t* node, TreeNode_t* prev_node, FILE* file, Diff_t* diff);



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

    fprintf(
        file, 
        "\\documentclass{article}\n"
        "\\usepackage{amsmath}\n"
        "\\usepackage{breqn}\n"
        "\\usepackage{graphicx}"
        "\\usepackage{float}\n"
        "\\DeclareGraphicsExtensions{.pdf,.png,.jpg}"
        "\\begin{document}\n\n"
    );

    fprintf(
        file,
        "\\begin{titlepage}\n"
        "\\centering\n"
        "\\vspace*{2cm}\n"
        "{\\Huge \\bfseries Calculus Stuff \\par}\n"
        "\\vspace{2cm}\n"
        "{\\large \\today \\par}\n"
        "\\vspace{3cm}\n"
        "\\begin{flushright}\n"
        "By: \\\\\n"
        "Who really cares\\par\n"
        "\\end{flushright}\n"
        "\\vfill\n"
        "\\end{titlepage}\n\n"
    );

    fclose(file);

}



void PrintSectionHeaderToTex(const char* title) {
    
    FILE* file = GetTexFile();
    if (file == nullptr) return;

    fprintf(file, "\\section*{%s}\n\n", title);
    
    fclose(file);
}



void PrintMesAndEqToTex (Diff_t* diff, TreeNode_t* root, const char* mes ) {

    FILE* file = GetTexFile();
    if ( file == nullptr ) return;

    //fprintf(file, "Let's find derivative for: [i don't want to do this]\n");
    fprintf(file, "%s\n", mes);
    fprintf(file, "\\begin{dmath*} \n");
    TreeDumpTex(root, nullptr, file, diff);
    fprintf(file, " \n\\end{dmath*}\n\n");
    
    fclose(file);


}



void PrintMesAndTaylorEqToTex (Diff_t* diff, TreeNode_t* root, const char* mes,
                               double x0, int deg) {

    FILE* file = GetTexFile();
    if ( file == nullptr ) return;

    fprintf(file, "%s\n", mes);
    fprintf(file, "\\begin{dmath*} \n");
    TreeDumpTex(root, nullptr, file, diff);
    fprintf(file, "+o((x-%lf)^{%d})", x0, deg);
    fprintf(file, " \n\\end{dmath*}\n\n");
    
    fclose(file);

}



void CreateTexLog (Diff_t* diff, TreeNode_t* root, TreeNode_t* d_root ) {

    FILE* file = GetTexFile();
    if ( file == nullptr ) return;
    
    fprintf(file, "%s\n", RndLinkWords[rand()%rndw_num]);
    fprintf(file, "\\begin{dmath*} \n");
    fprintf(file, "\\frac{d}{dx}(");
    TreeDumpTex( root, nullptr, file, diff);
    fprintf(file, ") = ");
    TreeDumpTex( d_root, nullptr, file, diff);
    fprintf(file, " \n\\end{dmath*}\n\n");
    
    fclose(file);

}



void PutImgToLog (Diff_t* diff, const char* img_filename ) {

    FILE* file = GetTexFile();
    if ( file == nullptr ) return;

    fprintf(file, "And you may have a look at graphic:\n");
    fprintf(
        file,
        "\\begin{figure}[H]\n"
        "\\centering\n"
        "\\includegraphics[scale=0.5]{%s}\n"
        "\\end{figure}\n",
        img_filename
    );

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



void TreeDumpTex (TreeNode_t* node, TreeNode_t* prev_node, FILE* file, Diff_t* diff) {

    if (!node) return;

    int parent_priority = 0, node_priority = 0;
    node_priority = GetOperPriority(node->data.oper);
    if (prev_node)
        parent_priority = GetOperPriority(prev_node->data.oper);

    switch(node->type) {

        case Node_t::NUM:
            HandleNum(node, file);
            break;

        case Node_t::VAR:
            HandleVar(node, file, diff);
            break;

        case Node_t::OP_BIN:
            if (parent_priority > node_priority) fprintf(file, "(");
            HandleOpersBin(node, prev_node, file, diff);
            if (parent_priority > node_priority) fprintf(file, ")");
            break;

        case Node_t::OP_UN:
            HandleOpersUn(node, prev_node, file, diff);
            break;
    }

}



static void HandleNum (TreeNode_t* node, FILE* file) {

    fprintf(file, "%lg ", node->data.num);

}



static void HandleVar (TreeNode_t* node, FILE* file, Diff_t* diff) {

    fprintf(file, "%c ", diff->name_table.buff[node->data.oper].name);

}



static void HandleOpersBin (TreeNode_t* node, TreeNode_t* prev_node, FILE* file, Diff_t* diff) {

    if (node->data.oper == Oper_t::DIV) {
        fprintf(file, "\\frac{ ");
        TreeDumpTex(node->left, node, file, diff);
        fprintf(file, "}{ ");
        TreeDumpTex(node->right, node, file, diff);
        fprintf(file, "} ");

    } else if (node->data.oper == Oper_t::DEG) {

        TreeDumpTex(node->left, node, file, diff);
        fprintf(file, "^{ ");
        TreeDumpTex(node->right, node, file, diff);
        fprintf(file, "} ");

    } else if (node->data.oper == Oper_t::MULT) {

        TreeDumpTex(node->left, node, file, diff);
        fprintf(file, "\\cdot ");
        TreeDumpTex(node->right, node, file, diff);

    } else {

        TreeDumpTex(node->left, node, file, diff);
        fprintf(file, "%s ", diff->def_op_instr[node->data.oper].name);
        TreeDumpTex(node->right, node, file, diff);
    }

}



static void HandleOpersUn (TreeNode_t* node, TreeNode_t* prev_node, FILE* file, Diff_t* diff) {

    fprintf(file, " \\%s(", diff->def_op_instr[node->data.oper].name);
    TreeDumpTex(node->right, node, file, diff);
    fprintf(file, " )");

}   