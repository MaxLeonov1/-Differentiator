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

#define SH_  Oper_t::SH
#define CH_  Oper_t::CH
#define TH_  Oper_t::TH
#define CTH_ Oper_t::CTH

#define dL_ Differentiate(node->left, prev_node, dir_var, diff)
#define dR_ Differentiate(node->right, prev_node, dir_var, diff)
#define cL_ CopyTree(node->left, prev_node)
#define cR_ CopyTree(node->right, prev_node)

#define CB_(oper, left, right ) CreateBinOp(oper, left, right, nullptr)
#define CU_(oper, right ) CreateUnOp(oper, right, nullptr)
#define CN_(val ) CreateNumNode(val, nullptr)

#define DUMP_TEX_ CreateTexLog(diff, node, prev_node);




DiffErr_t FindDerivative ( Diff_t* diff, int inp_eq_idx, int dir_var, int dir_deg, DiffErr_t* status ) {

    assert(diff);
    assert(diff->forest[inp_eq_idx]);

    for(size_t i = 1; i <= dir_deg; i++) {

        //PrintDiffToTex(diff, diff->forest[inp_eq_idx]->root);
        PrintMesAndEqToTex(
            diff, diff->forest[inp_eq_idx]->root,
            "Let's find derivative for: [i don't want to do this]");

        int out_eq_idx = AddEquation(diff, status);
        diff->forest[out_eq_idx]->root = Differentiate ( diff->forest[inp_eq_idx]->root, nullptr, dir_var, diff);
        inp_eq_idx = out_eq_idx;
    }
    _RET_OK_

}



TreeNode_t* Differentiate ( TreeNode_t* node, TreeNode_t* prev_node, int dir_var, Diff_t* diff) {

    assert(node);
    assert(diff);

    TreeNode_t* d_node = nullptr;

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
                    d_node = CB_(node->data.oper, dL_, dR_); 
                    DUMP_TEX_
                    return d_node;
                
                case Oper_t::MULT:
                    d_node = CB_(SUM_, CB_(MUL_, dL_, cR_), CB_(MUL_, cL_, dR_));
                    DUMP_TEX_
                    return d_node;

                case Oper_t::DIV:
                    d_node = CB_(DIV_,
                                CB_(SUB_, CB_(MUL_, dL_, cR_), CB_(MUL_, cL_, dR_)), 
                                CB_(MUL_, cL_, cR_));
                    DUMP_TEX_
                    return d_node;
                
                case Oper_t::DEG:
                    if(node->right->type == Node_t::NUM) {
                        d_node = CB_(MUL_, cR_, CB_(DEG_, cL_, CB_(SUB_, cR_, CN_(1)))); 
                        DUMP_TEX_
                        return d_node;

                    } else if(node->left->type == Node_t::NUM) {
                        d_node = CB_(MUL_, CB_(DEG_, cL_, cR_), CU_(LN_, cL_));
                        DUMP_TEX_
                        return d_node;

                    } else {
                        d_node = CB_(MUL_,
                                     CB_(DEG_, cL_, cR_), 
                                     CB_(SUM_, 
                                         CB_(MUL_, dR_, CU_(LN_, cL_)),
                                         CB_(MUL_, cR_, CB_(DIV_, dL_, cL_))));
                        DUMP_TEX_
                        return d_node;
                    }
                case Oper_t::LOG:
                    if(node->right->type == Node_t::NUM) {
                        d_node = CB_(MUL_,
                                     CB_(DIV_, CN_(1), CB_(MUL_, cR_, CU_(LN_, cL_))),
                                     dR_);
                        DUMP_TEX_
                        return d_node;

                    } else {
                        d_node = CB_(SUB_,
                                     CB_(DIV_, dR_, CB_(MUL_, cR_, CU_(LN_, cL_))),
                                     CB_(DIV_,
                                         CB_(MUL_, CU_(LN_, cR_), dL_),
                                         CB_(MUL_, cL_, CB_(DEG_, CU_(LN_, cL_), CN_(2))))); 
                        DUMP_TEX_
                        return d_node;

                    }
                default:
                    printf("%s[UNKNOWN CMD]%s\n", RED, RES_COL);
                    return nullptr;

            }
        }   

        case Node_t::OP_UN: {

            switch(node->data.oper) {

                case Oper_t::SIN:
                    d_node = CB_(MUL_, CU_(COS_, cR_), dR_);
                    DUMP_TEX_
                    return d_node;
                
                case Oper_t::COS:
                    d_node = CB_(MUL_, 
                                 CB_(MUL_, CN_(-1), CU_(SIN_, cR_)),
                                 dR_ );
                    DUMP_TEX_
                    return d_node;

                case Oper_t::LN:
                    d_node = CB_(MUL_,
                                 CB_(DIV_, CN_(1), cR_),
                                 dR_);
                    DUMP_TEX_
                    return d_node;

                case Oper_t::TG:
                    d_node = CB_(MUL_,
                                 CB_(DIV_,
                                     CN_(1),
                                     CB_(MUL_, CU_(COS_, cR_), CU_(COS_, cR_))),
                                 dR_);
                    DUMP_TEX_
                    return d_node;

                case Oper_t::ARCSIN:
                    d_node =  CB_(MUL_,
                                  CB_(DIV_,
                                      CN_(1),
                                      CB_(DEG_, CB_(SUB_, CN_(1), CB_(DEG_, cR_, CN_(2))), CN_(0.5))),
                                  dR_);
                    DUMP_TEX_
                    return d_node;
                
                case Oper_t::ARCCOS:
                    d_node = CB_(MUL_,
                                 CB_(MUL_, CN_(-1),
                                     CB_(DIV_,
                                         CN_(1),
                                         CB_(DEG_, CB_(SUB_, CN_(1), CB_(DEG_, cR_, CN_(2))), CN_(0.5)))),
                                 dR_);
                    DUMP_TEX_
                    return d_node;
                
                case Oper_t::ARCTG:
                    d_node = CB_(MUL_,
                                 CB_(DIV_,
                                     CN_(1),
                                     CB_(SUM_, CN_(1), CB_(DEG_, cR_, CN_(2)))),
                                 dR_);
                    DUMP_TEX_
                    return d_node;

                case Oper_t::ARCCTG:
                    d_node = CB_(MUL_,
                                 CB_(MUL_, CN_(-1),
                                     CB_(DIV_,
                                         CN_(1),
                                         CB_(SUM_, CN_(1), CB_(DEG_, cR_, CN_(2))))),
                                 dR_);
                    DUMP_TEX_
                    return d_node;

                case Oper_t::SH:
                    d_node = CB_(MUL_, CU_(CH_, cR_), dR_);
                    DUMP_TEX_
                    return d_node;

                case Oper_t::CH:
                    d_node = CB_(MUL_, CU_(SH_, cR_), dR_);
                    DUMP_TEX_
                    return d_node;

                case Oper_t::TH:
                    d_node = CB_(MUL_,
                                 CB_(SUB_, CN_(1), CB_(DEG_, CU_(TH_, cR_), CN_(2))),
                                 dR_);
                    DUMP_TEX_
                    return d_node;
                
                case Oper_t::CTH:
                    d_node = CB_(MUL_,
                                 CB_(SUB_, CN_(1), CB_(DEG_, CU_(CH_, cR_), CN_(2))),
                                 dR_);
                    DUMP_TEX_
                    return d_node;
                
                case Oper_t::ARCSH:
                    d_node = CB_(MUL_,
                                 CB_(DIV_,
                                     CN_(1),
                                     CB_(DEG_, CB_(SUM_, CB_(DEG_, cR_, CN_(2)), CN_(1)), CN_(0.5))),
                                 dR_);
                    DUMP_TEX_
                    return d_node;
                
                case Oper_t::ARCCH:
                    d_node = CB_(MUL_,
                                 CB_(DIV_,
                                     CN_(1),
                                     CB_(DEG_, CB_(SUB_, CB_(DEG_, cR_, CN_(2)), CN_(1)), CN_(0.5))),
                                 dR_);
                    DUMP_TEX_
                    return d_node;
                
                case Oper_t::ARCTH:
                    d_node = CB_(MUL_,
                                 CB_(DIV_,
                                     CN_(1),
                                     CB_(SUB_, CN_(1), CB_(DEG_, cR_, CN_(2)))),
                                 dR_);
                    DUMP_TEX_
                    return d_node;
                
                case Oper_t::ARCCTH:
                    d_node = CB_(MUL_,
                                 CB_(DIV_,
                                     CN_(1),
                                     CB_(SUB_, CN_(1), CB_(DEG_, cR_, CN_(2)))),
                                 dR_);
                    DUMP_TEX_
                    return d_node;

                default:
                    printf("%s[UNKNOWN CMD]%s(or not parsed)\n", RED, RES_COL);
                    return nullptr;
            }
        }
        default:
            printf("[NODE TYPE ERR]\n");
            return nullptr;
    }
    return nullptr;
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

#undef SH_
#undef CH_
#undef TH_
#undef CTH_

#undef dL_ 
#undef dR_ 
#undef cL_ 
#undef cR_ 

#undef CB_
#undef CU_
#undef CN_

#undef DUMP_TEX_


DiffErr_t FindValue (Diff_t* diff, int tree_idx) {

    assert(diff);
    assert(diff->forest[tree_idx]);

    AskVarVal (diff);

    DiffErr_t status = DiffErr_t::TREE_OK;
    double res = CalcTree(diff->forest[tree_idx]->root, diff, &status);
    if (status != DiffErr_t::TREE_OK) return status;

    printf("%sResult: %lf%s\n", BLUE, res, RES_COL);

    _RET_OK_

}



void AskVarVal (Diff_t* diff) {

    assert(diff);

    for (size_t idx = 0; idx < diff->name_table.num; idx++) {

        // for (size_t i = 0; i<diff->name_table.num; i++) {
        //     printf("%d %d\n", diff->name_table.buff[i], diff->name_table.buff[i].name);
        // }
        printf("What is '%c' value?\n", diff->name_table.buff[idx].name);
        scanf("%lf", &diff->name_table.buff[idx].val);
    }
}



double CalcTree (TreeNode_t* node, Diff_t* diff, DiffErr_t* status ) {

    if (*status != DiffErr_t::TREE_OK)
        return 0;

    switch(node->type){

        case Node_t::NUM: {
            return node->data.num;
        }

        case Node_t::VAR: {
            return diff->name_table.buff[node->data.var_idx].val;
        }

        case Node_t::OP_BIN: {
            double l_val = CalcTree(node->left, diff, status);
            double r_val = CalcTree(node->right, diff, status);

            switch(node->data.oper) {

                case Oper_t::ADD:
                    return l_val+r_val;

                case Oper_t::SUB:
                    return l_val-r_val;

                case Oper_t::MULT:
                    return l_val*r_val;

                case Oper_t::DIV:
                    if (double_cmp(0, r_val)) {
                        *status = DiffErr_t::MATH_ERR;
                        return 0;
                    }
                    return l_val/r_val;

                case Oper_t::DEG:
                    return pow(l_val, r_val);

                case Oper_t::LOG:
                    if (l_val <= 0 || double_cmp(1, l_val)) {
                        *status = DiffErr_t::MATH_ERR;
                        return 0;
                    }
                    if (r_val <= 0) {
                        *status = DiffErr_t::MATH_ERR;
                        return 0;
                    }
                    return log(r_val) / log(l_val);

                default:
                    *status = DiffErr_t::INC_MATH_OP;
                    return 0;
            }
        }

        case Node_t::OP_UN: {
            double val = CalcTree(node->right, diff, status);

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
                        *status = DiffErr_t::MATH_ERR;
                        return 0;
                    }
                    return log(val);

                case Oper_t::ARCSIN:
                    if (val < -1 || val > 1) {
                        *status = DiffErr_t::MATH_ERR;
                        return 0;
                    }
                    return asin(val);

                case Oper_t::ARCCOS:
                    if (val < -1 || val > 1) {
                        *status = DiffErr_t::MATH_ERR;
                        return 0;
                    }
                    return acos(val);

                case Oper_t::ARCTG:
                    return atan(val);

                case Oper_t::ARCCTG:
                    return M_PI_2 - atan(val);

                case Oper_t::SH:
                    return sinh(val);

                case Oper_t::CH:
                    return cosh(val);

                case Oper_t::TH:
                    return tanh(val);

                case Oper_t::CTH:
                    if (double_cmp(0, val)) {
                        *status = DiffErr_t::MATH_ERR;
                        return 0;
                    }
                    return 1.0 / tanh(val);

                case Oper_t::ARCSH:
                    return asinh(val);

                case Oper_t::ARCCH:
                    if (val < 1) {
                        *status = DiffErr_t::MATH_ERR;
                        return 0;
                    }
                    return acosh(val);

                case Oper_t::ARCTH:
                    if (val <= -1 || val >= 1) {
                        *status = DiffErr_t::MATH_ERR;
                        return 0;
                    }
                    return atanh(val);

                case Oper_t::ARCCTH:
                    if (fabs(val) <= 1) {
                        *status = DiffErr_t::MATH_ERR;
                        return 0;
                    }
                    return atanh(1.0 / val);

                default:
                    *status = DiffErr_t::INC_MATH_OP;
                    return 0;
                }
            }
            default: {
                *status = DiffErr_t::INC_NODE_TYPE;
                return 0;
            }
    }
    return 0;
}