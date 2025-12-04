#include "diff_calc_func.h"



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

#define dL_ Differentiate(node->left, dir_var, diff, status)
#define dR_ Differentiate(node->right, dir_var, diff, status)
#define cL_ CopyTree(node->left)
#define cR_ CopyTree(node->right)

#define CB_(oper, left, right ) CreateBinOp(oper, left, right)
#define CU_(oper, right ) CreateUnOp(oper, right)
#define CN_(cR_ ) CreateNumNode(cR_)

#define DUMP_TEX_ CreateTexLog(diff, node, d_node);

/*=====================================================================================*/

TreeNode_t* DiffNum(TreeNode_t* node, int dir_var, Diff_t* diff) {
    return CN_(0);
}

TreeNode_t* DiffVar(TreeNode_t* node, int dir_var, Diff_t* diff) {
    if (node->data.var_idx == dir_var)
        return CN_(1);
    else 
        return CN_(0);
}

TreeNode_t* DiffAdd(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = CB_(SUM_, dL_, dR_); 
    DUMP_TEX_
    return d_node;
}

TreeNode_t* DiffSub(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = CB_(SUB_, dL_, dR_); 
    DUMP_TEX_
    return d_node;
}

TreeNode_t* DiffMul(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = CB_(SUM_, CB_(MUL_, dL_, cR_), CB_(MUL_, cL_, dR_));
    DUMP_TEX_
    return d_node;
}

TreeNode_t* DiffDiv(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = CB_(DIV_,
                             CB_(SUB_, CB_(MUL_, dL_, cR_), CB_(MUL_, cL_, dR_)), 
                             CB_(MUL_, cL_, cR_));
    DUMP_TEX_
    return d_node;
}

TreeNode_t* DiffDeg(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = nullptr;
    
    if(nR_->type == Node_t::NUM) {
        d_node = CB_(MUL_, cR_, CB_(DEG_, cL_, CB_(SUB_, cR_, CN_(1)))); 
        DUMP_TEX_
        return d_node;

    } else if(nL_->type == Node_t::NUM) {
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
}

TreeNode_t* DiffLog(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = nullptr;
    
    if(nR_->type == Node_t::NUM) {
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
}

TreeNode_t* DiffSin(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = CB_(MUL_, CU_(COS_, cR_), dR_);
    DUMP_TEX_
    return d_node;
}

TreeNode_t* DiffCos(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = CB_(MUL_, 
                             CB_(MUL_, CN_(-1), CU_(SIN_, cR_)),
                             dR_ );
    DUMP_TEX_
    return d_node;
}

TreeNode_t* DiffLn(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = CB_(MUL_,
                             CB_(DIV_, CN_(1), cR_),
                             dR_);
    DUMP_TEX_
    return d_node;
}

TreeNode_t* DiffTg(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = CB_(MUL_,
                             CB_(DIV_,
                                 CN_(1),
                                 CB_(MUL_, CU_(COS_, cR_), CU_(COS_, cR_))),
                             dR_);
    DUMP_TEX_
    return d_node;
}

TreeNode_t* DiffCtg(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = CB_(MUL_,
                             CB_(MUL_, CN_(-1),
                                 CB_(DIV_,
                                     CN_(1),
                                     CB_(MUL_, CU_(SIN_, cR_), CU_(SIN_, cR_)))),
                             dR_);
    DUMP_TEX_
    return d_node;
}

TreeNode_t* DiffArcsin(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = CB_(MUL_,
                              CB_(DIV_,
                                  CN_(1),
                                  CB_(DEG_, CB_(SUB_, CN_(1), CB_(DEG_, cR_, CN_(2))), CN_(0.5))),
                              dR_);
    DUMP_TEX_
    return d_node;
}

TreeNode_t* DiffArccos(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = CB_(MUL_,
                             CB_(MUL_, CN_(-1),
                                 CB_(DIV_,
                                     CN_(1),
                                     CB_(DEG_, CB_(SUB_, CN_(1), CB_(DEG_, cR_, CN_(2))), CN_(0.5)))),
                             dR_);
    DUMP_TEX_
    return d_node;
}

TreeNode_t* DiffArctg(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = CB_(MUL_,
                             CB_(DIV_,
                                 CN_(1),
                                 CB_(SUM_, CN_(1), CB_(DEG_, cR_, CN_(2)))),
                             dR_);
    DUMP_TEX_
    return d_node;
}

TreeNode_t* DiffArcctg(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = CB_(MUL_,
                             CB_(MUL_, CN_(-1),
                                 CB_(DIV_,
                                     CN_(1),
                                     CB_(SUM_, CN_(1), CB_(DEG_, cR_, CN_(2))))),
                             dR_);
    DUMP_TEX_
    return d_node;
}

TreeNode_t* DiffSh(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = CB_(MUL_, CU_(CH_, cR_), dR_);
    DUMP_TEX_
    return d_node;
}

TreeNode_t* DiffCh(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = CB_(MUL_, CU_(SH_, cR_), dR_);
    DUMP_TEX_
    return d_node;
}

TreeNode_t* DiffTh(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = CB_(MUL_,
                             CB_(SUB_, CN_(1), CB_(DEG_, CU_(TH_, cR_), CN_(2))),
                             dR_);
    DUMP_TEX_
    return d_node;
}

TreeNode_t* DiffCth(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = CB_(MUL_,
                             CB_(SUB_, CN_(1), CB_(DEG_, CU_(CTH_, cR_), CN_(2))),
                             dR_);
    DUMP_TEX_
    return d_node;
}

TreeNode_t* DiffArcsh(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = CB_(MUL_,
                             CB_(DIV_,
                                 CN_(1),
                                 CB_(DEG_, CB_(SUM_, CB_(DEG_, cR_, CN_(2)), CN_(1)), CN_(0.5))),
                             dR_);
    DUMP_TEX_
    return d_node;
}

TreeNode_t* DiffArcch(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = CB_(MUL_,
                             CB_(DIV_,
                                 CN_(1),
                                 CB_(DEG_, CB_(SUB_, CB_(DEG_, cR_, CN_(2)), CN_(1)), CN_(0.5))),
                             dR_);
    DUMP_TEX_
    return d_node;
}

TreeNode_t* DiffArcth(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = CB_(MUL_,
                             CB_(DIV_,
                                 CN_(1),
                                 CB_(SUB_, CN_(1), CB_(DEG_, cR_, CN_(2)))),
                             dR_);
    DUMP_TEX_
    return d_node;
}

TreeNode_t* DiffArccth(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {
    TreeNode_t* d_node = CB_(MUL_,
                             CB_(DIV_,
                                 CN_(1),
                                 CB_(SUB_, CN_(1), CB_(DEG_, cR_, CN_(2)))),
                             dR_);
    DUMP_TEX_
    return d_node;
}

/*=====================================================================================*/

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


#define cL_ CalcTree(nL_, diff, status)
#define cR_ CalcTree(nR_, diff, status)

/*=====================================================================================*/

double CalcNum(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    return node->data.num;
}

double CalcVar(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    return diff->name_table.buff[node->data.var_idx].val;
}

double CalcAdd(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    return cL_ + cR_;
}

double CalcSub(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    return cL_ - cR_;
}

double CalcMul(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    return cL_ * cR_;
}

double CalcDiv(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    
    if (double_cmp(0, cR_)) {
        *status = DiffErr_t::MATH_ERR;
        return 0;
    }
    return cL_ / cR_;
}

double CalcDeg(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    return pow(cL_, cR_);
}

double CalcLog(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    
    if (cL_ <= 0 || double_cmp(1, cL_)) {
        *status = DiffErr_t::MATH_ERR;
        return 0;
    }
    if (cR_ <= 0) {
        *status = DiffErr_t::MATH_ERR;
        return 0;
    }
    return log(cR_) / log(cL_);
}

double CalcSin(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    return sin(cR_);
}

double CalcCos(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    return cos(cR_);
}

double CalcTg(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    return tan(cR_);
}

double CalcCtg(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    return 1/tan(cR_);
}

double CalcLn(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    
    if (cR_ < 0) {
        *status = DiffErr_t::MATH_ERR;
        return 0;
    }
    return log(cR_);
}

double CalcArcsin(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    
    if (cR_ < -1 || cR_ > 1) {
        *status = DiffErr_t::MATH_ERR;
        return 0;
    }
    return asin(cR_);
}

double CalcArccos(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    
    if (cR_ < -1 || cR_ > 1) {
        *status = DiffErr_t::MATH_ERR;
        return 0;
    }
    return acos(cR_);
}

double CalcArctg(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    return atan(cR_);
}

double CalcArcctg(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    return M_PI_2 - atan(cR_);
}

double CalcSh(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    return sinh(cR_);
}

double CalcCh(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    return cosh(cR_);
}

double CalcTh(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    return tanh(cR_);
}

double CalcCth(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    
    if (double_cmp(0, cR_)) {
        *status = DiffErr_t::MATH_ERR;
        return 0;
    }
    return 1.0 / tanh(cR_);
}

double CalcArcsh(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    return asinh(cR_);
}

double CalcArcch(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    
    if (cR_ < 1) {
        *status = DiffErr_t::MATH_ERR;
        return 0;
    }
    return acosh(cR_);
}

double CalcArcth(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    
    if (cR_ <= -1 || cR_ >= 1) {
        *status = DiffErr_t::MATH_ERR;
        return 0;
    }
    return atanh(cR_);
}

double CalcArccth(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    
    if (fabs(cR_) <= 1) {
        *status = DiffErr_t::MATH_ERR;
        return 0;
    }
    return atanh(1.0 / cR_);
}

/*=====================================================================================*/

const int oper_instr_num = sizeof(OperInstructions)/sizeof(OperInstructions[0]);

DiffFunc_t DiffFuncs[oper_instr_num] = {};
CalcFunc_t CalcFuncs[oper_instr_num] = {};

/*=====================================================================================*/

void InitDiffFuncs() {
    int is_init = 0;
    if (is_init) return;
    
    DiffFuncs[(int)(Oper_t::ADD)] =    DiffAdd;
    DiffFuncs[(int)(Oper_t::SUB)] =    DiffSub;
    DiffFuncs[(int)(Oper_t::MULT)] =   DiffMul;
    DiffFuncs[(int)(Oper_t::DIV)] =    DiffDiv;
    DiffFuncs[(int)(Oper_t::DEG)] =    DiffDeg;
    DiffFuncs[(int)(Oper_t::LOG)] =    DiffLog;
    
    DiffFuncs[(int)(Oper_t::SIN)] =    DiffSin;
    DiffFuncs[(int)(Oper_t::COS)] =    DiffCos;
    DiffFuncs[(int)(Oper_t::LN)] =     DiffLn;
    DiffFuncs[(int)(Oper_t::TG)] =     DiffTg;
    DiffFuncs[(int)(Oper_t::CTG)] =    DiffCtg;
    DiffFuncs[(int)(Oper_t::ARCSIN)] = DiffArcsin;
    DiffFuncs[(int)(Oper_t::ARCCOS)] = DiffArccos;
    DiffFuncs[(int)(Oper_t::ARCTG)] =  DiffArctg;
    DiffFuncs[(int)(Oper_t::ARCCTG)] = DiffArcctg;
    DiffFuncs[(int)(Oper_t::SH)] =     DiffSh;
    DiffFuncs[(int)(Oper_t::CH)] =     DiffCh;
    DiffFuncs[(int)(Oper_t::TH)] =     DiffTh;
    DiffFuncs[(int)(Oper_t::CTH)] =    DiffCth;
    DiffFuncs[(int)(Oper_t::ARCSH)] =  DiffArcsh;
    DiffFuncs[(int)(Oper_t::ARCCH)] =  DiffArcch;
    DiffFuncs[(int)(Oper_t::ARCTH)] =  DiffArcth;
    DiffFuncs[(int)(Oper_t::ARCCTH)] = DiffArccth;
    
    is_init = 1;
}

void InitCalcFuncs() {
    int is_init = 0;
    if (is_init) return;
    
    CalcFuncs[(int)(Oper_t::ADD)] =    CalcAdd;
    CalcFuncs[(int)(Oper_t::SUB)] =    CalcSub;
    CalcFuncs[(int)(Oper_t::MULT)] =   CalcMul;
    CalcFuncs[(int)(Oper_t::DIV)] =    CalcDiv;
    CalcFuncs[(int)(Oper_t::DEG)] =    CalcDeg;
    CalcFuncs[(int)(Oper_t::LOG)] =    CalcLog;
    
    CalcFuncs[(int)(Oper_t::SIN)] =    CalcSin;
    CalcFuncs[(int)(Oper_t::COS)] =    CalcCos;
    CalcFuncs[(int)(Oper_t::LN)] =     CalcLn;
    CalcFuncs[(int)(Oper_t::TG)] =     CalcTg;
    CalcFuncs[(int)(Oper_t::CTG)] =    CalcCtg;
    CalcFuncs[(int)(Oper_t::ARCSIN)] = CalcArcsin;
    CalcFuncs[(int)(Oper_t::ARCCOS)] = CalcArccos;
    CalcFuncs[(int)(Oper_t::ARCTG)] =  CalcArctg;
    CalcFuncs[(int)(Oper_t::ARCCTG)] = CalcArcctg;
    CalcFuncs[(int)(Oper_t::SH)] =     CalcSh;
    CalcFuncs[(int)(Oper_t::CH)] =     CalcCh;
    CalcFuncs[(int)(Oper_t::TH)] =     CalcTh;
    CalcFuncs[(int)(Oper_t::CTH)] =    CalcCth;
    CalcFuncs[(int)(Oper_t::ARCSH)] =  CalcArcsh;
    CalcFuncs[(int)(Oper_t::ARCCH)] =  CalcArcch;
    CalcFuncs[(int)(Oper_t::ARCTH)] =  CalcArcth;
    CalcFuncs[(int)(Oper_t::ARCCTH)] = CalcArccth;
    
    is_init = 1;
}