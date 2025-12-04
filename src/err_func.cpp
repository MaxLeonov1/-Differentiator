#include "err_func.h"



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
        case DiffErr_t::INC_MATH_OP:
            return "INCORRECT MATH OPERATION";
        case DiffErr_t::INC_NODE_TYPE:
            return "INCORRECT NODE TYPE";
        default:
            return "UNHANDLED ERR";
    }

}




DiffErr_t VerifyTree(TreeNode_t* node) {
    if (node == nullptr) {
        return DiffErr_t::TREE_OK;
    }

    switch (node->type) {
        case Node_t::NUM:
        case Node_t::VAR:
            
            if (node->left != nullptr || node->right != nullptr)
                return DiffErr_t::TREE_STRUCT_ERR;
            break;

        case Node_t::OP_BIN:
            
            if (node->left == nullptr || node->right == nullptr)
                return DiffErr_t::TREE_STRUCT_ERR;
            break;

        case Node_t::OP_UN:
            if (node->left == nullptr)
                return DiffErr_t::TREE_STRUCT_ERR;
            
            if (node->right != nullptr)
                return DiffErr_t::TREE_STRUCT_ERR;
            break;

        default:
            return DiffErr_t::TREE_STRUCT_ERR;
            break;
    }

    if (node->left != nullptr) {
        DiffErr_t status = VerifyTree(node->left);
        if (status != DiffErr_t::TREE_OK) {
            return status;
        }
    }

    if (node->right != nullptr) {
        DiffErr_t status = VerifyTree(node->right);
        if (status != DiffErr_t::TREE_OK) {
            return status;
        }
    }

    return DiffErr_t::TREE_OK;
}