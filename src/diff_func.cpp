#include <assert.h>
#include <math.h>

#include "diff_func.h"
#include "../log_utils/logger.h"
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
#define DEG_ Oper_t::DEG
#define LN_  Oper_t::LN

#define SIN_ Oper_t::SIN
#define COS_ Oper_t::COS
#define TG_  Oper_t::TG
#define CTG_ Oper_t::CTG

#define dL_ Differentiate(node->left, prev_node, dir_var, diff)
#define dR_ Differentiate(node->right, prev_node, dir_var, diff)
#define cL_ CopyTree(node->left, prev_node)
#define cR_ CopyTree(node->right, prev_node)

#define CB_(oper, left, right ) CreateBinOp(oper, left, right, nullptr)
#define CU_(oper, right ) CreateUnOp(oper, right, nullptr)
#define CN_(val ) CreateNumNode(val, nullptr)



DiffErr_t FindDerivative ( Diff_t* diff, int inp_tree_idx, int out_tree_idx, int dir_var ) {

    assert(diff);
    assert(diff->forest[inp_tree_idx]);
    assert(diff->forest[out_tree_idx]);

    diff->forest[out_tree_idx]->root = Differentiate ( diff->forest[inp_tree_idx]->root, nullptr, dir_var, diff);

    _RET_OK_

}



TreeNode_t* Differentiate ( TreeNode_t* node, TreeNode_t* prev_node, int dir_var, Diff_t* diff) {

    assert(node);
    assert(diff);

    CreateTexLog(diff, node);

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
                case Oper_t::SUB:
                    return CB_(node->data.oper, dL_, dR_);
                
                case Oper_t::MULT:
                    return CB_(SUM_, CB_(MUL_, dL_, cR_), CB_(MUL_, cL_, dR_));

                case Oper_t::DIV:
                    return CB_(DIV_,
                                CB_(SUB_, CB_(MUL_, dL_, cR_), CB_(MUL_, cL_, dR_)), 
                                CB_(MUL_, cL_, cR_));
                
                case Oper_t::DEG:
                    if(node->right->type == Node_t::NUM)
                        return CB_(MUL_, cR_, CB_(DEG_, cL_, CB_(SUB_, cR_, CN_(1)))); 

                    else if(node->left->type == Node_t::NUM) 
                        return CB_(MUL_, CB_(DEG_, cL_, cR_), CU_(LN_, cL_));

                    else
                        return CB_(MUL_,
                                   CB_(DEG_, cL_, cR_), 
                                   CB_(SUM_, 
                                       CB_(MUL_, dR_, CU_(LN_, cL_)),
                                       CB_(MUL_, cR_, CB_(DIV_, dL_, cL_))));

                default:
                    printf("%s[UNKNOWN CMD]%s(or not parsed)\n", RED, RES_COL);
                    return nullptr;

            }
        }   

        case Node_t::OP_UN: {

            switch(node->data.oper) {

                case Oper_t::SIN:
                    return CB_(MUL_, CU_(COS_, cR_), dR_);
                
                case Oper_t::COS:
                    return CB_(MUL_, 
                            CB_(MUL_, CN_(-1), CU_(SIN_, cR_)),
                            dR_ );
                case Oper_t::LN:
                    return CB_(MUL_,
                            CB_(DIV_, CN_(1), cR_),
                            dR_);

                case Oper_t::TG:
                    return CB_(MUL_,
                            CB_(DIV_,
                                CN_(1),
                                CB_(MUL_, CU_(COS_, cR_), CU_(COS_, cR_))),
                            dR_);
                default:
                    printf("%s[UNKNOWN CMD]%s(or not parsed)\n", RED, RES_COL);
                    return nullptr;
            }
        }
    }

}



#undef nR_
#undef nL_ 
#undef nP_ 
#undef nT_ 

#undef SUM_ 
#undef MUL_ 
#undef SUB_ 
#undef DIV_ 
#undef DEG_
#undef LN_

#undef SIN_ 
#undef COS_ 
#undef TG_  
#undef CTG_ 

#undef dL_ 
#undef dR_ 
#undef cL_ 
#undef cR_ 

#undef CB_
#undef CU_
#undef CN_



DiffErr_t FindValue (Diff_t* diff, int tree_idx) {

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

        printf("%p\n", diff->name_table.buff[idx]);
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
            double val = CalcTree(node->right, diff);

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