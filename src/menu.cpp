// menu.cpp
#include <stdio.h>

#include "colors.h"
#include "../utils/sup_func.h"
#include "../log_utils/logger.h"
#include "menu.h"
#include "diff_func.h"
#include "eq_simpl.h"



void ShowMainMenu() {
    printf(
        "\n__________________________________\n"
        "| 1. Read expression from file    |\n"
        "| 2. Save expression to file      |\n"
        "| 3. Differentiate expression     |\n"
        "| 4. Calculate expression value   |\n"
        "| 5. Taylor series expansion      |\n"
        "| 6. Exit                         |\n"
        "___________________________________\n"
        "Choose an option: ");
}

MenuOption GetMenuChoice() {
    int choice;
    scanf("%d", &choice);
    getchar(); 
    return (MenuOption)choice;
}

void HandleMenuChoice(Diff_t* diff, MenuOption choice, DiffErr_t* status) {
    char filename[MAX_STR_LEN_] = "";
    int tree_idx, dir_var;
    
    switch (choice) {
        case MenuOption::READ_FILE:
            printf("Enter filename: ");
            scanf("%s", filename);
            *status = ReadFromDisk(diff, filename);
            if (*status == DiffErr_t::TREE_OK)
                printf("Expression loaded successfully\n");
            TreeDump(diff, 0, *status, nullptr);
            break;
            
        case MenuOption::SAVE_FILE:
            printf("Enter tree index: ");
            scanf("%d", &tree_idx);
            printf("Enter filename: ");
            scanf("%s", filename);
            *status = SaveToDisk(diff, tree_idx, filename);
            if (*status == DiffErr_t::TREE_OK) {
                printf("Expression saved successfully\n");
            }
            break;
            
        case MenuOption::DIFFERENTIATE:
            printf("Enter source tree index: ");
            scanf("%d", &tree_idx);
            printf("Enter target tree index: ");
            scanf("%d", &dir_var); 
            printf("Enter variable index for differentiation: ");
            scanf("%d", &dir_var);
            *status = FindDerivative(diff, tree_idx, dir_var, 0);
            if (*status == DiffErr_t::TREE_OK)
                printf("Differentiation completed\n");
            _IF_DEBUG(TreeDump(diff, 0, *status, nullptr))
            break;
            
        case MenuOption::CALCULATE_VALUE:
            printf("Enter tree index: ");
            scanf("%d", &tree_idx);
            *status = FindValue(diff, tree_idx);
            break;
            
        case MenuOption::TAYLOR_SERIES:
            printf("Taylor series expansion - Not implemented yet\n");
            // TODO: Реализовать разложение в ряд Тейлора
            break;
            
        case MenuOption::EXIT:
            CloseTexLog();
            printf("Goodbye!\n");
            break;
            
        default:
            printf("%s[INVALID OPTION]%s\n", RED, RES_COL);
            break;
    }
}