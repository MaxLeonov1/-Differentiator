#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "visuals.h"



static DiffErr_t CreatePlotData (Diff_t* diff, FILE* data_file, TreeNode_t* root,
                                 int points, int var_idx );



DiffErr_t AddGraphToPlotPDF(Diff_t* diff, TreeNode_t* root, const char* output_filename,
                            int points, int var_idx) {
    
    _OK_STAT_

    const char* log_dir = GetGlobalLogDir();
    static int graph_num = 1;
    
    char data_file[MAX_STR_LEN_] = "";
    char script_file[MAX_STR_LEN_] = "";
    char output_filedir[MAX_STR_LEN_] = "";
    sprintf(data_file, "%s/plot_data_%d.txt", log_dir, graph_num);
    sprintf(script_file, "%s/plot_script.txt", log_dir);
    sprintf(output_filedir, "%s/%s", log_dir, output_filename);
    
    FILE* data_fp = fopen(data_file, "w");
    if (!data_fp)
        return DiffErr_t::FILE_OPEN_ERR;

    if (diff->name_table.num>=1 && diff->name_table.num<2) {
        status = CreatePlotData(diff, data_fp, root, points, var_idx);
        STAT_CHECK_
    } else
        return DiffErr_t::VAR_PLOT_ERR;
    
    fclose(data_fp);
    
    FILE* script_fp = fopen(script_file, "a");
    if (!script_fp) 
        return DiffErr_t::FILE_OPEN_ERR;
    
    if (graph_num == 1) {

        fprintf(
            script_fp, 
            "set terminal pdfcairo size 10,6 enhanced font 'Arial,12'\n"
            "set output '%s'\n"
            "set title 'Function Graph'\n"
            "set xlabel 'x'\n"
            "set ylabel 'y'\n"
            "set xrange[%g:%g]\n"
            "set yrange[%g:%g]\n"
            "set grid\n"
            "set key box\n"
            "plot",
            output_filedir,
            diff->params.graph.x_min,
            diff->params.graph.x_max,
            diff->params.graph.y_min,
            diff->params.graph.y_max);
    }
    
    fprintf(script_fp, " '%s' with lines linewidth 2 linecolor rgb '%s' title '", 
            data_file, GraphCol[graph_num-1]);
    WriteEqToFile(root, nullptr, diff->name_table.buff, script_fp);
    fprintf(script_fp, "', \\\n");
    graph_num++;
    
    fclose(script_fp);
    
    char command[MAX_STR_LEN_];
    snprintf(command, sizeof(command), "gnuplot \"%s\"", script_file);
    system(command);
    
    return DiffErr_t::TREE_OK;
}



void CreatePlotPDF() {

    const char* log_dir = GetGlobalLogDir();
    
    char script_file[MAX_STR_LEN_] = "";
    sprintf(script_file, "%s/plot_script.txt", log_dir);

    char command[MAX_STR_LEN_];
    snprintf(command, sizeof(command), "gnuplot \"%s\"", script_file);
    system(command);

}



DiffErr_t QuickPlotPDF(Diff_t* diff, TreeNode_t* tree, const char* output_filename, int var_idx) {
    return AddGraphToPlotPDF(diff, tree, output_filename, 1000, var_idx);
}



static DiffErr_t CreatePlotData (Diff_t* diff, FILE* data_file, TreeNode_t* root,
                                 int points, int var_idx ) {

    _OK_STAT_

    double original_x = diff->name_table.buff[var_idx].val;
    double x_max = diff->params.graph.x_max;
    double x_min = diff->params.graph.x_min;
    
    double step = (x_max - x_min) / (points - 1);
    for (int i = 0; i < points; i++) {
        double x = x_min + i * step;
        
        diff->name_table.buff[var_idx].val = x;
        double y = CalcTree(root, diff, &status);
        if (status != DiffErr_t::TREE_OK) return status;
        
        if (!isnan(y) && !isinf(y)) {
            fprintf(data_file, "%f %f\n", x, y);
        }
    }
    
    diff->name_table.buff[var_idx].val = original_x;
    return status;

}