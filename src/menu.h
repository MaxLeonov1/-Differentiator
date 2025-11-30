// menu.h
#pragma once

#include "differentiator.h"

typedef struct {
    
    char* inp_file;
    char* out_file;

} CmdLineArgs;

#define DEF_IN_FILE "/mnt/c/Users/Max/C_Projects/-Differentiator/disk.bin"
#define DEF_OUT_FILE "/mnt/c/Users/Max/C_Projects/-Differentiator/disk_out.bin"

typedef enum {
    READ_FILE = 1,
    SAVE_FILE = 2,
    DIFFERENTIATE = 3,
    CALCULATE_VALUE = 4,
    TAYLOR_SERIES = 5,
    EXIT = 6
} MenuOption;


void ShowCmdOp();
CmdLineArgs ParseCmdLineArgs (int argc, char* argv[]);
void ShowMainMenu();
MenuOption GetMenuChoice();
void HandleMenuChoice(Diff_t* diff, MenuOption choice, DiffErr_t* status, CmdLineArgs* args);