#include "errors.h"
#include "differentiator.h"

const char* StatusCodeToStr ( DiffErr_t status ) {

    switch(status) {
        case DiffErr_t::INSERT_EX_POS_ERR:
            return "TRY TO INSERT IN EXISTING TREE NODE";
        case DiffErr_t::FILE_OPEN_ERR:
            return "UNABLE TO OPEN FILE";
        case DiffErr_t::MEM_ALLOC_ERR:
            return "ERROR IN ALLOCATION";
        case DiffErr_t::READ_SYNTAX_ERR:
            return "INCORRECT FILE SYNTAX";
        case DiffErr_t::READ_DATA_ERR:
            return "ERROR WHILE READING NODE DATA";
        case DiffErr_t::EMPTY_TREE_ACT_ERR:
            return "OPERATION WITH EMPTY TREE";
        case DiffErr_t::TREE_OK:
            return "OK";
    }

}