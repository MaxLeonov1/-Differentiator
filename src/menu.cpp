// menu.cpp
#include <stdio.h>
#include <string.h>

#include "colors.h"
#include "../utils/sup_func.h"
#include "../log_utils/logger.h"
#include "menu.h"
#include "diff_func.h"
#include "eq_simpl.h"



void ShowCmdOp() {
    printf(
        "%s==========================================================\n"
        "| -i <input_file_path>  | defines input file             |\n"
        "| -o <output_file_path> | defines output file [optional] |\n" 
        "==========================================================\n%s",
        BLUE, RES_COL
    );
}



CmdLineArgs ParseCmdLineArgs (int argc, char* argv[]) {

    CmdLineArgs args = {0};

    for ( size_t i = 0; i < argc; i++ ) {

        if (strcmp(argv[i], "-i") == 0 && i+1 < argc) {
            args.inp_file = strdup(argv[i+1]);
            i++;
        } else if (strcmp(argv[i], "-o") == 0 && i+1 < argc) {
            args.out_file = strdup(argv[i+1]);
            i++;
        } else if (strcmp(argv[i], "-h") == 0) 
            ShowCmdOp();

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
        "| 5. Calc Taylor series           |\n"
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

void HandleMenuChoice(Diff_t* diff, MenuOption choice, DiffErr_t* status, CmdLineArgs* args) {
    int expr_idx = 0, dir_var = 0;
    
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
            int is_found = 0;

            printf("Enter index of differentiated expression idx: \n");
            scanf("%d", &expr_idx);
            
            do {
                printf("Enter differentiation degree: \n");
                scanf("%d", &dir_deg);

                printf("Enter variable name for differentiation: \n");
                reset_input_buff();
                scanf("%c", &dir_var);

                for (size_t i = 0; i<diff->name_table.num; i++) {
                    if (dir_var == diff->name_table.buff[i].name) {
                        is_found = 1;
                        dir_var = i;
                        break;
                    }
                }

                if (!is_found) 
                    printf("There is no such variable in %s%d%s expression.\n", GREEN, expr_idx, RES_COL);

            } while (!is_found);

            *status = FindDerivative(diff, expr_idx, dir_var, dir_deg, status);

            if (*status == DiffErr_t::TREE_OK) {
                printf("aboba\n");
                *status = SimplTree(diff, diff->tree_num-1);

                if (*status == DiffErr_t::TREE_OK)
                    printf("Differentiation completed\n");
            }
            _IF_DEBUG(TreeDump(diff, 0, *status, nullptr))
            break;
        }    
        case MenuOption::CALCULATE_VALUE: {
            NO_LOAD_TERM_

            printf("Enter calc expression idx: ");
            scanf("%d", &expr_idx);
            if (expr_idx < diff->tree_num) {
                printf("There is no expression with such idx.\n");
                break;
            }
            *status = FindValue(diff, expr_idx);
            break;
        }    
        case MenuOption::TAYLOR_SERIES: {
            NO_LOAD_TERM_
            printf("[WORK IN PROGRESS]\n");
            // TODO: Реализовать разложение в ряд Тейлора
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