#include "tree.h"
#include "differentiator.h"
#include "../log_utils/logger.h"
#include "diff_func.h"
#include "eq_simpl.h"
#include "menu.h"

int main() {

    _OK_STAT_
    DIFF_INIT(diff_1)
    
    DiffCtor(&diff_1);
    
    AddEquation(&diff_1, &status);
    AddEquation(&diff_1, &status);
    

    MenuOption choice = MenuOption::DIFFERENTIATE;
    do {
        ShowMainMenu();
        choice = GetMenuChoice();
        HandleMenuChoice(&diff_1, choice, &status);
            
        if (status != DiffErr_t::TREE_OK) {
            printf("[ERROR: %d][%s]\n", status, StatusCodeToStr(status));
        }
            
    } while (choice != MenuOption::EXIT);
    
    
    DiffDtor(&diff_1);
    return 0;
}