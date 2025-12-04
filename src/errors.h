#pragma once

typedef enum{

    TREE_OK = 0,
    MEM_ALLOC_ERR = 1,
    INSERT_EX_POS_ERR = 2,
    FILE_OPEN_ERR = 3,
    READ_SYNTAX_ERR = 4,
    READ_DATA_ERR = 5,
    EMPTY_TREE_ACT_ERR = 6,
    MATH_ERR = 7,
    INC_MATH_OP = 8,
    INC_NODE_TYPE = 9,
    TREE_STRUCT_ERR = 10,
    VAR_PLOT_ERR = 11,
    INVALID_TREE_IDX = 12,

} DiffErr_t;