#include "math.h"

#include "tree.h"
#include "eq_simpl.h"
#include "../utils/sup_func.h"
#include "../utils/colors.h"

/*=====================================================================================*/

TreeNode_t* SimplConstTree(TreeNode_t* node) {
    if (!node) return nullptr;
    
    if (node->left) node->left = SimplConstTree(node->left);
    if (node->right) node->right = SimplConstTree(node->right);
    
    double const_value;
    if (TryEvalConstTree(node, &const_value)) {
        TreeNode_t* parent = node->parent;
        DeleteNode(node);
        TreeNode_t* new_node = CreateNumNode(const_value, parent);
        return new_node;
    }

    return node;
}

/*=====================================================================================*/

#define EvL_ TryEvalConstTree(node->left, &left_val)
#define EvR_ TryEvalConstTree(node->right, &right_val)

int TryEvalConstTree(TreeNode_t* node, double* result) {
    if (!node) {
        *result = 0;
        return 0;
    }
    
    switch (node->type) {
        case NUM:
            *result = node->data.num;
            return 1;
            
        case VAR:
            return 0;
            
        case OP_BIN: {
            double left_val = 0, right_val = 0;
            
            if (EvL_ && EvR_) {
                switch (node->data.oper) {
                    case ADD:
                        *result = left_val + right_val;
                        return 1;
                    case SUB:
                        *result = left_val - right_val;
                        return 1;
                    case MULT:
                        *result = left_val * right_val;
                        return 1;
                    case DIV:
                        if (double_cmp(0, right_val)) {
                            printf("%s[zero_div in const eval]%s\n", RED, RES_COL);
                            *result = 0;
                            return 1;
                        }
                        *result = left_val / right_val;
                        return 1;
                    case DEG:
                        *result = pow(left_val, right_val);
                        return 1;
                    default:
                        return 0;
                }
            }
            return 0;
        }
            
        case OP_UN: {
            double right_val = 0;
            
            if (EvR_) {
                switch (node->data.oper) {
                    case SIN:
                        *result = sin(right_val);
                        return 1;
                    case COS:
                        *result = cos(right_val);
                        return 1;
                    case TG:
                        *result = tan(right_val);
                        return 1;
                    case CTG:
                        *result = 1.0 / tan(right_val);
                        return 1;
                    case LN:
                        if (right_val <= 0) {
                            printf("%s[neg_log in const eval]%s\n", RED, RES_COL);
                            *result = 0;
                            return 1;
                        }
                        *result = log(right_val);
                        return 1;
                    default:
                        return 0;
                }
            }
            return 0;
        }
            
        default:
            return 0;
    }
}

#undef EvL_
#undef EvR_

/*=====================================================================================*/

#define SIMPL_NODE_(new_node)     \
    TreeNode_t* res = new_node;   \
    new_node = nullptr;           \
    DeleteNode(node);             \
    return res;                   

TreeNode_t* SimplTrivleCases(TreeNode_t* node){

    if(node->left) node->left  = SimplTrivleCases(node->left);
    if(node->left) node->right = SimplTrivleCases(node->right);

    if (node->type == Node_t::OP_BIN) {

        switch(node->data.oper) {

            case Oper_t::ADD:

                if (node->left && node->left->type == Node_t::NUM && 
                    double_cmp(0, node->left->data.num)) {
                    SIMPL_NODE_(node->right)
                }
                if (node->right && node->right->type == Node_t::NUM && 
                    double_cmp(0, node->right->data.num)) {
                    SIMPL_NODE_(node->left)
                }

            case Oper_t::MULT:

                if (node->left && node->left->type == Node_t::NUM) {
                    if (double_cmp(1, node->left->data.num)) {
                        SIMPL_NODE_(node->right)

                    } else if (double_cmp(0, node->left->data.num)) {
                        SIMPL_NODE_(node->left)
                    }
                }

                if (node->right && node->right->type == Node_t::NUM) {
                    if (double_cmp(1, node->right->data.num)) {
                        SIMPL_NODE_(node->left)

                    } else if (double_cmp(0, node->right->data.num)) {
                        SIMPL_NODE_(node->right)
                    }
                }

            case Oper_t::DIV:
                
                if (node->right && node->right->type == Node_t::NUM && 
                    double_cmp(1, node->right->data.num)) {
                    SIMPL_NODE_(node->left)
                }
            
            case Oper_t::DEG:
                
                if (node->right && node->right->type == Node_t::NUM) {
                    if (double_cmp(0, node->right->data.num)) {
                        TreeNode_t* new_node = node->right;
                        new_node->data.num = 1;
                        node->right = nullptr;
                        DeleteNode(node);
                        return new_node;

                    } else if (double_cmp(1, node->right->data.num)) {
                        SIMPL_NODE_(node->left)
                    }
                }
        }
    }

    return node;
}

#undef SIMPL_NODE_

/*=====================================================================================*/

DiffErr_t SimplTree(Diff_t* diff, int tree_idx) {

    _OK_STAT_

    diff->forest[tree_idx]->root = SimplConstTree(diff->forest[tree_idx]->root);
    diff->forest[tree_idx]->root = SimplTrivleCases(diff->forest[tree_idx]->root);

    _RET_OK_
}