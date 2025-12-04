#include "../src/differentiator.h"
#include "../src/main_func.h"
#include "logger.h"
#include "../src/input_output_func.h"



static const char* GraphCol[] = {
    "blue",
    "green",
    "red",
};



DiffErr_t AddGraphToPlotPDF(Diff_t* diff, TreeNode_t* root,
                            const char* output_filename, int points, int var_idx);
DiffErr_t QuickPlotPDF(Diff_t* diff, TreeNode_t* tree, const char* output_filename, int var_idx);
void      CreatePlotPDF();