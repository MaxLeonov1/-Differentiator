#include "math.h"

#include "tree.h"
#include "eq_simpl.h"
#include "../utils/sup_func.h"
#include "../utils/colors.h"



int ConstTreeCheck(TreeNode_t* node) {
    if (node == nullptr) return 0;
    
    switch (node->type) {
        case NUM:
            return 1;
            
        case VAR:
            return 0;
            
        case OP_BIN:
            return ConstTreeCheck(node->left) && 
                   ConstTreeCheck(node->right);
            
        case OP_UN:
            return ConstTreeCheck(node->left);
            
        default:
            return 0;
    }
}



double EvalConstTree(TreeNode_t* node) {
    if (node == NULL) return 0;
    
    switch (node->type) {
        case NUM:
            return node->data.num;
            
        case OP_BIN: {
            double left_val = EvalConstTree(node->left);
            double right_val = EvalConstTree(node->right);
            
            switch (node->data.oper) {
                case ADD:
                    return left_val + right_val;
                case SUB:
                    return left_val - right_val;
                case MULT:
                    return left_val * right_val;
                case DIV:
                    if (double_cmp(0, right_val)) {
                        printf("%s[zero_div in const eval]%s\n", RED, RES_COL);
                        return 0;
                    }
                    return left_val / right_val;
                case DEG:
                    return pow(left_val, right_val);
                default:
                    return 0;
            }
        }
            
        case OP_UN: {
            double val = EvalConstTree(node->left);
            
            switch (node->data.oper) {
                case SIN:
                    return sin(val);
                case COS:
                    return cos(val);
                case TG:
                    return tan(val);
                case CTG:
                    return 1.0 / tan(val);
                case LN:
                    if (val <= 0) {
                        printf("%s[neg_log in const eval]%s\n", RED, RES_COL);
                        return 0;
                    }
                    return log(val);
                default:
                    return 0;
            }
        }
            
        default:
            return 0;
    }
}



TreeNode_t* SimplConstTree(TreeNode_t* node) {
    if (node == NULL) return NULL;
    
    if (node->left) node->left = SimplConstTree(node->left);
    if (node->right) node->right = SimplConstTree(node->right);
    
    if (ConstTreeCheck(node)) {
        double val = EvalConstTree(node);
        
        TreeNode_t* parent = node->parent;
        
        DeleteNode(node); //TODO: err checka
        
        TreeNode_t* new_node = CreateNumNode(val, parent);
        
        return new_node;
    }

    return node;
}



DiffErr_t SimplTree(Diff_t* diff, int tree_idx) {

    _OK_STAT_

    diff->forest[tree_idx]->root = SimplConstTree(diff->forest[tree_idx]->root);

    _RET_OK_
}