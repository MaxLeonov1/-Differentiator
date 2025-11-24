// menu.h
#pragma once

#include "differentiator.h"

typedef enum {
    READ_FILE = 1,
    SAVE_FILE = 2,
    DIFFERENTIATE = 3,
    CALCULATE_VALUE = 4,
    TAYLOR_SERIES = 5,
    EXIT = 6
} MenuOption;

void ShowMainMenu();
MenuOption GetMenuChoice();
void HandleMenuChoice(Diff_t* diff, MenuOption choice, DiffErr_t* status);