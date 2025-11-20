#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <stdarg.h>

#include "tree.h"
#include "differentiator.h"
#include "sup_func.h"

#pragma GCC diagnostic ignored "-Wformat=2"
#pragma GCC diagnostic ignored "-Wformat-overflow"
#pragma GCC diagnostic ignored "-Wformat-truncation"

/*=====================================================================================*/

#define NUM_COL_ "fillcolor=\"#4CAF50\", color=\"#2E7D32\""
#define VAR_COL_ "fillcolor=\"#FF9800\", color=\"#EF6C00\""
#define OP_COL_  "fillcolor=\"#2196F3\", color=\"#1565C0\""

/*=====================================================================================*/

const char* StatusCodeToStr ( TreeErr_t status ) {

    switch(status) {
        case TreeErr_t::INSERT_EX_POS_ERR:
            return "TRY TO INSERT IN EXISTING TREE NODE";
        case TreeErr_t::FILE_OPEN_ERR:
            return "UNABLE TO OPEN FILE";
        case TreeErr_t::MEM_ALLOC_ERR:
            return "ERROR IN ALLOCATION";
        case TreeErr_t::READ_SYNTAX_ERR:
            return "INCORRECT FILE SYNTAX";
        case TreeErr_t::READ_DATA_ERR:
            return "ERROR WHILE READING NODE DATA";
        case TreeErr_t::EMPTY_TREE_ACT_ERR:
            return "OPERATION WITH EMPTY TREE";
        case TreeErr_t::TREE_OK:
            return "OK";
    }

}

/*=====================================================================================*/

void CreateLogDir ( char* dir_name ) {

    assert(dir_name);
 
    char proj_path[MAX_STR_LEN_] = "";
    time_t log_time_sec = time(NULL);
    struct tm* log_time = localtime( &log_time_sec );
    
    getcwd(proj_path, sizeof(proj_path));
    sprintf ( proj_path, "%s%clogs", proj_path, PATH_SEP );
    _MKDIR(proj_path);

    sprintf ( dir_name,
              "%s%clog_%d.%d.%d_%d-%d-%d",
              proj_path,
              PATH_SEP,
              log_time->tm_mday,
              log_time->tm_mon,
              1900 + log_time->tm_year,
              log_time->tm_hour,
              log_time->tm_min,
              log_time->tm_sec );
    _MKDIR(dir_name);

}

/*=====================================================================================*/

void TreeDump ( Diff_t* diff, int tree_idx, TreeErr_t status, const char* format, ... ) {

    assert(diff);

    static int call_num = 1;
    char filename[MAX_STR_LEN_] = "";
    char graphname[MAX_STR_LEN_] = "";
    static char log_dir[MAX_STR_LEN_] = "";

    if (call_num == 1) CreateLogDir ( log_dir );

    snprintf ( filename, sizeof(filename), "%s%clist_log.htm", log_dir, PATH_SEP );
    snprintf ( graphname, sizeof(graphname), "graph_%d.svg", call_num );

    call_num++;

    FILE* log_file = fopen ( filename, "a" );
    if ( log_file == nullptr ) exit(0);

    fprintf ( log_file,
              "\n<div style=\"height:4px;background:#000\"/>\n" 
              "<pre>\n"
              "<body style=\"background-color: white;\">\n" );

     if (format != NULL) {
        va_list args;
        va_start(args, format);

        fprintf(log_file, "<h3>");
        vfprintf(log_file, format, args);
        fprintf(log_file, "</h3>");
        
        va_end(args);
    }

    PrintLogHeader ( diff->forest[tree_idx], log_file, status );
    
    fprintf ( log_file, "<h3>[IMG]:</h3>\n" );

    if (diff->forest[tree_idx]->root) {

        CreateGraphImg ( diff, tree_idx, graphname, log_dir );
        fprintf ( 
            log_file, 
            "<img "
            "src = \"%s\" "
            "style=\"width: 80vw; height: auto; max-width: 100%%;\" >",
            graphname 
        );
    }

    fclose ( log_file );

}

/*=====================================================================================*/

void PrintLogHeader ( Tree_t* tree, FILE* log_file, TreeErr_t status ) {

    assert(tree);
    assert(log_file);

    if ( status != TreeErr_t::TREE_OK )
        fprintf(
            log_file,
            "<h3 style=\"color:red;\">[ERROR][%d]</h3>\n"
            "<h3 style=\"color:red;\">[%s]</h3>\n",
            status, StatusCodeToStr(status)
        );
    else
        fprintf(
            log_file,
            "<h3 style=\"color:green;\">[NO ERROR DUMP]</h3>\n"
        );

    fprintf( 
        log_file,
        "<h3>[TREE INFO]:</h3>\n"
        "name: %s\n"
        "location: %s::%d, %s()\n"
        "<h3>[TREE DATA]:</h3>\n"
        "capacity: %lu\n"
        "root[%lu]\n",
        tree->info.name,
        tree->info.file,
        tree->info.line,
        tree->info.func,
        tree->cpcty,
        tree->root
    );

    
}

/*=====================================================================================*/

void CreateGraphImg ( Diff_t* diff, int tree_idx, const char* graphname, const char* graph_dir ) {

    assert(diff);
    assert(graphname);
    assert(graph_dir);

    char graph_txt_path[MAX_STR_LEN_] = {0};
    char graph_svg_path[MAX_STR_LEN_] = {0};
    snprintf ( graph_svg_path, sizeof(graph_svg_path), "%s/%s", graph_dir, graphname );
    snprintf ( graph_txt_path, sizeof(graph_txt_path), "%s/graph.txt", graph_dir );

    FILE* graph_text = fopen ( graph_txt_path, "w" );

    fprintf ( graph_text, 
              "digraph structs {\n"
              "   rankdir = HR;\n"
              "   bgcolor = \"white\""
              /*"   splines = \"ortho\";\n"*/
              "   node [fontname=\"Helvetica-BoldOblique\", fontsize=\"11\","
              " style=\"filled,bold\", fillcolor = \"#fff0b3\", color = \"#ffe680\"];\n"
              );
    
    PrintGraphNodes(diff->forest[tree_idx]->root, 1, graph_text, diff);

    fprintf ( graph_text, "}" );
    fclose ( graph_text );

    char cmd_line[MAX_STR_LEN_] = {0};
    snprintf ( cmd_line, sizeof(cmd_line), "dot -Tsvg %s -o %s", graph_txt_path, graph_svg_path );
    system ( cmd_line );

}



int PrintGraphNodes(TreeNode_t* node, int rank, FILE* graph_text, Diff_t* diff) {

    assert(node);
    assert(graph_text);

    static int idx = 0;
    int idx_left = 0;
    int idx_right = 0;

    char data[MAX_STR_LEN_] = "";
    const char* type  = "";
    const char* color = "";

    if(rank == 1) idx = 0;

    if (node->left)  idx_left  = PrintGraphNodes(node->left,  rank+1, graph_text, diff);
    if (node->right) idx_right = PrintGraphNodes(node->right, rank+1, graph_text, diff);

    switch(node->type){
        case Node_t::NUM:
            type = "NUM";
            color = NUM_COL_;
            sprintf(
                data, 
                "[%lf]", 
                node->data.num
            );
            break;

        case Node_t::OP_BIN:
            type = "OPER_BIN";
            color = OP_COL_;
            sprintf(
                data, 
                "[%s(%s)]", 
                diff->def_op_instr[node->data.oper].f_name, 
                diff->def_op_instr[node->data.oper].name
            );
            break;

        case Node_t::OP_UN:
            type = "OPER_UN";
            color = OP_COL_;
            sprintf(
                data, 
                "[%s(%s)]", 
                diff->def_op_instr[node->data.oper].f_name, 
                diff->def_op_instr[node->data.oper].name
            );
            break;

        case Node_t::VAR:
            type = "VAR";
            color = VAR_COL_;
            sprintf(
                data,
                "[ %d '%s')]",
                node->data.var_idx,
                diff->name_table.buff[node->data.var_idx].name
            );
            break;
    }

    fprintf(
        graph_text, 
        "node_%d[shape=Mrecord, rank=%d, %s"
        "label=\" { %p | type: %s | data: %s |  parent: %p | { Left: %p | Right: %p } } \",];\n",
        idx,
        rank,
        color,
        node,
        type,
        data,
        node->parent,
        node->left,
        node->right
    );

    if (node->left) {
        fprintf(
            graph_text,
            "node_%d -> node_%d["EDGE_STD_SET_"];\n",
            idx, idx_left
        );
            
    }

    if (node->right) {
        fprintf(
            graph_text,
            "node_%d -> node_%d["EDGE_STD_SET_"];\n",
            idx, idx_right
        );
        
    }

    return idx++;

}