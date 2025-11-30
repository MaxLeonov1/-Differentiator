#include "tree.h"
#include "differentiator.h"
#include "../log_utils/logger.h"
#include "diff_func.h"
#include "eq_simpl.h"
#include "menu.h"
#include "../input_func/input.h"

int main(int argc, char* argv[]) {

    const char* str = "5 - sin ( 5 + x ) + ( 2 + y )^4$";

    _OK_STAT_
    DIFF_INIT(diff_1)

    CmdLineArgs args = ParseCmdLineArgs(argc, argv);
    
    DiffCtor(&diff_1);
    
    PrintTexLogHeader();
    MenuOption choice = MenuOption::DIFFERENTIATE;
    do {
        ShowMainMenu();
        choice = GetMenuChoice();
        HandleMenuChoice(&diff_1, choice, &status, &args);
            
        if (status != DiffErr_t::TREE_OK) {
            printf("[ERROR: %d][%s]\n", status, StatusCodeToStr(status));
        }
            
    } while (choice != MenuOption::EXIT);
    CloseTexLog();
    
    DiffDtor(&diff_1);
    return 0;
}