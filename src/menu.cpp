// menu.cpp
#include <stdio.h>
#include <string.h>

#include "menu.h"



static void AskAboutDiffDeg (Diff_t* diff, int* dir_deg, int* dir_var);

static void ProcessDifferentiation (Diff_t* diff, DiffErr_t* status);
static void ProcessTaylor          (Diff_t* diff, DiffErr_t* status);
static void ProcessTangent         (Diff_t* diff, DiffErr_t* status);



void ShowCmdOp() {
    printf(
        "%s==========================================================\n"
        "| -i <input_file_path>  | defines input file             |\n"
        "| -o <output_file_path> | defines output file [optional] |\n" 
        "==========================================================\n%s",
        BLUE, RES_COL
    );
}



CmdLineArgs ParseCmdLineArgs (int argc, char* argv[], Diff_t* diff) {

    CmdLineArgs args = {0};

    for ( size_t i = 0; i < argc; i++ ) {

        if (strcmp(argv[i], "-i") == 0 && i+1 < argc) {
            args.inp_file = strdup(argv[i+1]);
            i++;
        } else if (strcmp(argv[i], "-o") == 0 && i+1 < argc) {
            args.out_file = strdup(argv[i+1]);
            i++;
        } else if (strcmp(argv[i], "-h") == 0) {
            ShowCmdOp();
        } else if (strcmp(argv[i], "-st") == 0) {
            diff->params.simpl_tree = 1;
        }

    }

    return args;

}



void ShowMainMenu() {
    printf(
        "%s===================================\n"
        "| Choose operation mode:          |\n"
        "| 1. Read expression from file    |\n"
        "| 2. Save expression to file      |\n"
        "| 3. Differentiate expression     |\n"
        "| 4. Find expression value        |\n"
        "| 5. Default Mode                 |\n"
        "| 6. Exit program                 |\n"
        "===================================\n%s",
        BLUE, RES_COL
    );
}



MenuOption GetMenuChoice() {
    int choice = 0;
    scanf("%d", &choice);
    getchar(); 
    return (MenuOption)choice;
}



#define NO_LOAD_TERM_                                     \
    if (diff->tree_num == 0) {                            \
        printf("You have not loaded any expression\n");   \
        break;                                            \
    }

#define SAVED_EXPR_IDX_ 0
#define DIR_VAR_ diff->params.dir.var
#define DIR_DEG_ diff->params.dir.degree
#define TLR_X0_  diff->params.taylor.x0
#define TAN_X0_  diff->params.tan.x0

void HandleMenuChoice(Diff_t* diff, MenuOption choice, DiffErr_t* status, CmdLineArgs* args) {
    int    expr_idx = 0;
    int    dir_var  = diff->params.dir.var;
    int    dir_deg  = diff->params.dir.degree;
    double tlr_x0   = diff->params.taylor.x0;
    double tan_x0   = diff->params.tan.x0;

    
    switch (choice) {
        case MenuOption::READ_FILE: {
            AddEquation(diff, status);

            if (!args->inp_file) {
                const char* filename = DEF_IN_FILE;
                *status = ReadFromDisk(diff, filename);
            } else 
                *status = ReadFromDisk(diff, args->inp_file);

            if (*status == DiffErr_t::TREE_OK)
                printf("Expression loaded successfully\n");
            
            PrintMesAndEqToTex(diff, diff->forest[0]->root,
                               "So we have this equation, and we are going to do "
                               "something with it [at this moment i dont know what"
                               " **** we are going to involve this equation to]");
            if (diff->params.graph.make) {                   
                QuickPlotPDF(diff, diff->forest[0]->root, GRAPH_NAME_, dir_var);
            }
            TreeDump(diff, 0, *status, nullptr);
            break;
        }    
        case MenuOption::SAVE_FILE: {
            printf("Enter expression index: ");
            scanf("%d", &expr_idx);
        
            if (!args->out_file) {
                const char* filename = DEF_OUT_FILE;
                *status = SaveToDisk(diff, expr_idx, filename);
            } else
                *status = SaveToDisk(diff, expr_idx, args->out_file);

            if (*status == DiffErr_t::TREE_OK) {
                printf("Expression saved successfully\n");
            }

            break;
        }   
        case MenuOption::DIFFERENTIATE: {
            NO_LOAD_TERM_

            int dir_deg = 0;

            printf("Enter differentiated expression idx: \n");
            scanf("%d", &expr_idx);

            AskAboutDiffDeg(diff, &dir_deg, &dir_var);

            *status = FindDerivative(diff, expr_idx, dir_var, dir_deg);

            if (*status == DiffErr_t::TREE_OK)
                printf("Differentiation completed\n");
        
            _IF_DEBUG(TreeDump(diff, 0, *status, nullptr);)
            QuickPlotPDF(diff, diff->forest[0]->root, "output_graph.pdf", dir_var);
            QuickPlotPDF(diff, diff->forest[1]->root, "output_graph.pdf", dir_var);
            CreatePlotPDF();
            PutImgToLog(diff, "output_graph.pdf");
            break;
        }    
        case MenuOption::CALCULATE_VALUE: {
            NO_LOAD_TERM_

            printf("Enter calc expression idx: \n");
            scanf("%d", &expr_idx);
            if (expr_idx < diff->tree_num) {
                printf("There is no expression with such idx.\n");
                break;
            }
            *status = FindValue(diff, expr_idx);
            break;
        }    
        case MenuOption::STD_WORKING_MODE: {
            NO_LOAD_TERM_

            for (size_t i = 0; i<diff->name_table.num; i++) {
                if (DIR_VAR_ == diff->name_table.buff[i].name) {
                    DIR_VAR_ = i;
                    break;
                }
            }

            if (diff->params.dir.make) {
                ProcessDifferentiation (diff, status);

                if (diff->params.taylor.make)
                    ProcessTaylor (diff, status);
                if (diff->params.tan.make)
                    ProcessTangent (diff, status);
            }

            CreatePlotPDF();
            PutImgToLog(diff, GRAPH_NAME_);
            break;
        }    
        case MenuOption::EXIT: {
            printf("Goodbye!\n");
            break;
        }    
        default:
            printf("%s[INVALID OPTION]%s\n", RED, RES_COL);
            break;
    }
}

#undef NO_LOAD_TERM_



static void AskAboutDiffDeg (Diff_t* diff, int* dir_deg, int* dir_var) {

    int is_found = 0;
            
    do {
        printf("Enter differentiation degree: \n");
        scanf("%d", dir_deg);

        printf("Enter variable name for differentiation: \n");
        reset_input_buff();
        scanf("%c", dir_var);

        for (size_t i = 0; i<diff->name_table.num; i++) {
            if (*dir_var == diff->name_table.buff[i].name) {
                is_found = 1;
                *dir_var = i;
                break;
            }
        }

        if (!is_found) 
            printf("There is no such variable in expression.\n");

    } while (!is_found);

}



static void ProcessDifferentiation (Diff_t* diff, DiffErr_t* status ){

    assert(diff);

    *status = FindDerivative(diff, SAVED_EXPR_IDX_, DIR_VAR_, DIR_DEG_);

    if (*status == DiffErr_t::TREE_OK)
        printf("Differentiation completed\n");
    else
        return;
            
    _IF_DEBUG(TreeDump(diff, SAVED_EXPR_IDX_, *status, nullptr);)

} 



static void ProcessTaylor (Diff_t* diff, DiffErr_t* status ) {

    assert(diff);

    *status = CreateTaylorSeriesEquation(diff, SAVED_EXPR_IDX_, DIR_DEG_, DIR_VAR_, TLR_X0_);
            
    if (*status == DiffErr_t::TREE_OK)
        printf("Taylor series created\n");
    else
        return;
            
    PrintSectionHeaderToTex("Getting Taylor Series!");
    PrintMesAndEqToTex(diff, diff->forest[SAVED_EXPR_IDX_]->root,
                       "No one asked, but we will find Taylor series for this equation:");
    PrintMesAndTaylorEqToTex(diff, diff->forest[DIR_DEG_+1]->root,
                             TLR_X0_, DIR_DEG_,
                             "Using our derivatives, we obtain:");
    SimplTree(diff, DIR_DEG_+1);
    PrintMesAndTaylorEqToTex(diff, diff->forest[DIR_DEG_+1]->root,
                             TLR_X0_, DIR_DEG_,
                             "So, we have simplified Taylor series at $x_0$:%lg,"
                             " with accuracy: $o(x-%lg)^{%d}$",
                             TLR_X0_, TLR_X0_, DIR_DEG_);

    _IF_DEBUG(TreeDump(diff, diff->tree_num-1, *status, nullptr);)
    if(diff->params.graph.make)
        QuickPlotPDF(diff, diff->forest[diff->tree_num-1]->root, GRAPH_NAME_, DIR_VAR_);

}



static void ProcessTangent (Diff_t* diff, DiffErr_t* status ) {

    assert(diff);

    *status = ConstructTangentToTex(diff, SAVED_EXPR_IDX_, SAVED_EXPR_IDX_+1, TAN_X0_, DIR_VAR_);

    if (*status == DiffErr_t::TREE_OK)
        printf("Tangent created\n");
    else
        return;

    PrintSectionHeaderToTex("Finding tangent");
    _IF_DEBUG( TreeDump(diff, diff->tree_num-1, *status, NULL));
    PrintMesAndEqToTex(diff, diff->forest[diff->tree_num-1]->root,
                       "Using some mysterious dark magic we summoned the TANGENT equation "
                       "at point of x=%lg", TAN_X0_);

    if(diff->params.graph.make)
        QuickPlotPDF(diff, diff->forest[diff->tree_num-1]->root, GRAPH_NAME_, DIR_VAR_);

}

#undef SAVED_EXPR_IDX_
#undef DIR_VAR_ 
#undef DIR_DEG_ 
#undef TLR_X0_  
#undef TAN_X0_  
