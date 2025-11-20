#include <assert.h>
#include <math.h>

#include "diff_func.h"
#include "../utils/sup_func.h"
#include "../utils/colors.h"



#define nR_ node->right
#define nL_ node->left   
#define nP_ node->parent
#define nT_ node->type

#define SUM_ Oper_t::ADD
#define MUL_ Oper_t::MULT
#define SUB_ Oper_t::SUB
#define DIV_ Oper_t::DIV

#define SIN_ Oper_t::SIN
#define COS_ Oper_t::COS
#define TG_  Oper_t::TG
#define CTG_ Oper_t::CTG

#define dL_ Differentiate(node->left, prev_node, dir_var)
#define dR_ Differentiate(node->right, prev_node, dir_var)
#define cL_ CopyTree(node->left, prev_node)
#define cR_ CopyTree(node->right, prev_node)

#define CB_(oper, left, right, prev) CreateBinOp(oper, left, right, prev)
#define CU_(oper, left, prev) CreateUnOp(oper, left, prev)
#define CN_(val, prev) CreateNumNode(val, prev)



TreeNode_t* CreateNumNode (double val, TreeNode_t* prev_node) {

    TreeNode_t* node = (TreeNode_t*)calloc(1, sizeof(TreeNode_t));
    if (!node) return node;

    node->data.num = val;
    nP_ = prev_node;
    nL_ = nullptr;
    nR_ = nullptr;
    nT_ = Node_t::NUM;

    return node;
}



TreeNode_t* CreateBinOp (Oper_t oper, TreeNode_t* left, TreeNode_t* right, TreeNode_t* prev_node) {

    TreeNode_t* node = (TreeNode_t*)calloc(1, sizeof(TreeNode_t));
    if (!node) return node;

    node->data.oper = oper;
    nP_ = prev_node;
    nL_ = left;
    nR_ = right;
    nT_ = Node_t::OP_BIN;

    return node;
}
 


TreeNode_t* CreateUnOp (Oper_t oper, TreeNode_t* left, TreeNode_t* prev_node) {

    TreeNode_t* node = (TreeNode_t*)calloc(1, sizeof(TreeNode_t));
    if (!node) return node;

    node->data.oper = oper;
    nP_ = prev_node;
    nL_ = left;
    nR_ = nullptr;
    nT_ = Node_t::OP_UN;

    return node;
}



TreeErr_t FindDerivative ( Tree_t* tree, Tree_t* new_tree, size_t dir_var ) {

    assert(tree);
    assert(new_tree);

    new_tree->root = Differentiate ( tree->root, nullptr, dir_var);

    _RET_OK_

}



TreeNode_t* Differentiate ( TreeNode_t* node, TreeNode_t* prev_node, size_t dir_var) {

    switch(node->type) {

        case Node_t::NUM: {

            return CreateNumNode(0, prev_node);
        }

        case Node_t::VAR: {

            if (node->data.var_idx == dir_var)
                return CreateNumNode(1, prev_node);
            else 
                return CreateNumNode(0, prev_node);
        }

        case Node_t::OP_BIN: {

            switch(node->data.oper) {

                case Oper_t::ADD:
                case Oper_t::SUB: {
                    printf("%d\n", node->data.oper);
                    return CB_(node->data.oper, dL_, dR_, prev_node);
                }
                case Oper_t::MULT:
                    return CB_(SUM_, CB_(MUL_, dL_, cR_, prev_node), CB_(MUL_, cL_, dR_, prev_node), prev_node);

                case Oper_t::DIV:
                    return CB_(DIV_,
                                CB_(SUB_, CB_(MUL_, dL_, cR_, prev_node), CB_(MUL_, cL_, dR_, prev_node), prev_node), 
                                CB_(MUL_, cL_, cR_, prev_node), prev_node);
                default:
                    printf("%s[UNKNOWN CMD]%s(or not parsed)\n", RED, RES_COL);
                    return nullptr;

            }
        }   

        case Node_t::OP_UN: {

            switch(node->data.oper) {

                case Oper_t::SIN:
                    return CB_(MUL_, CU_(COS_, cL_, prev_node), dL_, prev_node);
                
                case Oper_t::COS:
                    return CB_(MUL_, 
                            CB_(MUL_, CN_(-1, prev_node), CU_(SIN_, cL_, prev_node), prev_node),
                            dL_, prev_node );
                case Oper_t::LN:
                    return CB_(MUL_,
                            CB_(DIV_, CN_(1, prev_node), cL_, prev_node),
                            dL_, prev_node);

                case Oper_t::TG:
                    return CB_(MUL_,
                            CB_(DIV_,
                                CN_(1, prev_node),
                                CB_(MUL_, CU_(COS_, cL_, prev_node), CU_(COS_, cL_, prev_node), prev_node), prev_node),
                            dL_, prev_node);
                default:
                    printf("%s[UNKNOWN CMD]%s(or not parsed)\n", RED, RES_COL);
                    return nullptr;
            }
        }
    }

}



TreeErr_t FindValue (Diff_t* diff, int tree_idx) {

    assert(diff);
    assert(diff->forest[tree_idx]);

    AskVarVal (diff);

    double res = CalcTree(diff->forest[tree_idx]->root, diff);

    printf("%sResult: %lf%s\n", BLUE, res, RES_COL);

    _RET_OK_

}



void AskVarVal (Diff_t* diff) {

    assert(diff);

    for (size_t idx = 0; idx < diff->name_table.num; idx++) {

        printf("What is '%s' value?\n", diff->name_table.buff[idx].name);
        scanf("%lf", &diff->name_table.buff[idx].val);
    }
}



double CalcTree (TreeNode_t* node, Diff_t* diff ) {

    switch(node->type){

        case Node_t::NUM: {
            return node->data.num;
        }

        case Node_t::VAR: {
            printf(
                "%d %lf\n",
                node->data.var_idx,
                diff->name_table.buff[node->data.var_idx].val
            );
            return diff->name_table.buff[node->data.var_idx].val;
        }

        case Node_t::OP_BIN: {
            double l_val = CalcTree(node->left, diff);
            double r_val = CalcTree(node->right, diff);

            switch(node->data.oper) {

                case Oper_t::ADD:
                    return l_val+r_val;

                case Oper_t::SUB:
                    return l_val-r_val;

                case Oper_t::MULT:
                    return l_val*r_val;

                case Oper_t::DIV:
                    if (double_cmp(0, r_val)) {
                        printf("%s[zero_div]%s\n", RED, RES_COL);
                        return 0;
                    }
                    return l_val+r_val;

                case Oper_t::DEG:
                    return pow(l_val, r_val);

                default:
                    printf("%s[UNKNOWN CMD]%s(or not parsed)\n", RED, RES_COL);
                    return 0;
            }
        }

        case Node_t::OP_UN:
            double val = CalcTree(node->left, diff);

            switch(node->data.oper) {

                case Oper_t::SIN:
                    return sin(val);
                    
                case Oper_t::COS:
                    return cos(val);

                case Oper_t::TG:
                    return tan(val);

                case Oper_t::CTG:
                    return 1/tan(val);

                case Oper_t::LN:
                    if (val<0) {
                        printf("%s[neg_log]%s\n", RED, RES_COL);
                        return 0;
                    }
                    return log(val);

                default:
                    printf("%s[UNKNOWN CMD]%s(or not parsed)\n", RED, RES_COL);
                    return 0;
            }
    }

}