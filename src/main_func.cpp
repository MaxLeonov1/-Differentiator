#include <assert.h>
#include <math.h>

#include "main_func.h"

/*=====================================================================================*/

static TreeNode_t* CreateTaylorElem(Diff_t* diff, int var_idx, double x0, int k, double fk_x0);

/*=====================================================================================*/

DiffErr_t FindDerivative ( Diff_t* diff, int inp_eq_idx, int dir_var, int dir_deg, DiffErr_t* status ) {

    assert(diff);
    assert(diff->forest[inp_eq_idx]);

    for(size_t i = 1; i <= dir_deg; i++) {

        PrintSectionHeaderToTex("Finding another derivative");
        PrintMesAndEqToTex(
            diff, diff->forest[inp_eq_idx]->root,
            "Let's find derivative for: [i don't want to do this]");

        int out_eq_idx = AddEquation(diff, status);
        diff->forest[out_eq_idx]->root = Differentiate ( diff->forest[inp_eq_idx]->root, dir_var, diff, status);
        inp_eq_idx = out_eq_idx;
        *status = SimplTree(diff, diff->tree_num - 1);
    }
    _RET_OK_

}

/*=====================================================================================*/

TreeNode_t* Differentiate(TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status) {

    assert(node);
    assert(diff);
    
    TreeNode_t* d_node = nullptr;
    
    switch(node->type) {
        case Node_t::NUM:
            return DiffNum(node, dir_var, diff);
            
        case Node_t::VAR:
            return DiffVar(node, dir_var, diff);
            
        case Node_t::OP_BIN:
        case Node_t::OP_UN: {
            int oper_idx = (int)node->data.oper;
            if (oper_idx >= oper_instr_num || DiffFuncs[oper_idx] == nullptr) {
                *status = DiffErr_t::INC_MATH_OP;
                return nullptr;
            }
            d_node = DiffFuncs[oper_idx](node, dir_var, diff, status);
            return d_node;
        }
            
        default:
            *status = DiffErr_t::INC_NODE_TYPE;
            return nullptr;
    }
}

/*=====================================================================================*/

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

/*=====================================================================================*/

void AskVarVal (Diff_t* diff) {

    assert(diff);

    for (size_t idx = 0; idx < diff->name_table.num; idx++) {

        printf("What is '%c' value?\n", diff->name_table.buff[idx].name);
        scanf("%lf", &diff->name_table.buff[idx].val);
    }
}

/*=====================================================================================*/

double CalcTreeAtPoint(Diff_t* diff, TreeNode_t* tree, int var_idx, double x0, DiffErr_t* status) {
    
    double old_val = diff->name_table.buff[var_idx].val;
    diff->name_table.buff[var_idx].val = x0;
    double result = CalcTree(tree, diff, status);
    diff->name_table.buff[var_idx].val = old_val;
    return result;
}

/*=====================================================================================*/

double CalcTree(TreeNode_t* node, Diff_t* diff, DiffErr_t* status) {
    if (*status != DiffErr_t::TREE_OK)
        return 0;
    
    switch(node->type) {
        case Node_t::NUM:
            return CalcNum(node, diff, status);
            
        case Node_t::VAR:
            return CalcVar(node, diff, status);
            
        case Node_t::OP_BIN:
        case Node_t::OP_UN: {
            int oper_idx = (int)node->data.oper;
            if (oper_idx >= oper_instr_num || CalcFuncs[oper_idx] == nullptr) {
                *status = DiffErr_t::INC_MATH_OP;
                return 0;
            }
            return CalcFuncs[oper_idx](node, diff, status);
        }
            
        default: {
            *status = DiffErr_t::INC_NODE_TYPE;
            return 0;
        }
    }
}

/*=====================================================================================*/

static TreeNode_t* CreateTaylorElem(Diff_t* diff, int var_idx, double x0, int k, double fk_x0) {

    if (k == 0) {
        return CreateNumNode(fk_x0);
    }
    
    // (x - x0)
    TreeNode_t* x_node = CreateVarNode(var_idx);
    if (!x_node) return nullptr;
    
    TreeNode_t* x0_node = CreateNumNode(x0);
    if (!x0_node) {

        DeleteNode(x_node);
        return nullptr;
    }
    
    TreeNode_t* x_minus_x0 = CreateBinOp(Oper_t::SUB, x_node, x0_node);
    if (!x_minus_x0) {

        DeleteNode(x_node);
        DeleteNode(x0_node);
        return nullptr;
    }
    
    // (x - x0)^k
    TreeNode_t* degree = nullptr;
    if (k == 1) {
        degree = x_minus_x0;
    } else {

        TreeNode_t* exponent = CreateNumNode(k);
        if (!exponent) {

            DeleteNode(x_minus_x0);
            return nullptr;
        }
        degree = CreateBinOp(Oper_t::DEG, x_minus_x0, exponent);
        if (!degree) {

            DeleteNode(x_minus_x0);
            DeleteNode(exponent);
            return nullptr;
        }
    }
    
    // k!
    long long factorial_k = 1;
    for (int i = 2; i <= k; i++) {
        factorial_k *= i;
    }
    
    // f^(k)(x0) / k!
    TreeNode_t* coeff = nullptr;
    if (factorial_k == 1) {
        coeff = CreateNumNode(fk_x0);
    } else {

        TreeNode_t* fk_node = CreateNumNode(fk_x0);
        TreeNode_t* fact_node = CreateNumNode((double)factorial_k);
        if (!fk_node || !fact_node) {
            if (fk_node) DeleteNode(fk_node);
            if (fact_node) DeleteNode(fact_node);
            if (degree) DeleteNode(degree);
            return nullptr;
        }
        coeff = CreateBinOp(Oper_t::DIV, fk_node, fact_node);
        if (!coeff) {
            DeleteNode(fk_node);
            DeleteNode(fact_node);
            if (degree) DeleteNode(degree);
            return nullptr;
        }
    }
    
    // (f^(k)(x0) / k!)*(x - x0)
    if (k == 1) {

        TreeNode_t* result = CreateBinOp(Oper_t::MULT, coeff, x_minus_x0);
        if (!result) {
            DeleteNode(coeff);
            DeleteNode(x_minus_x0);
        }
        return result;
    } else {

        TreeNode_t* result = CreateBinOp(Oper_t::MULT, coeff, degree);
        if (!result) {
            DeleteNode(coeff);
            DeleteNode(degree);
        }
        return result;
    }

}

/*=====================================================================================*/

DiffErr_t CreateTaylorSeriesEquation( Diff_t* diff, int func_tree_idx, int n, 
                                      int var_idx, double x0) {
    assert(diff);
    
    if (func_tree_idx < 0 || func_tree_idx >= diff->tree_num)
        return DiffErr_t::INVALID_TREE_IDX;
    
    if (func_tree_idx + n > diff->tree_num)
        return DiffErr_t::INVALID_TREE_IDX;
        
    DiffErr_t status = DiffErr_t::TREE_OK;
    int result_tree_idx = AddEquation(diff, &status);
    if (status != DiffErr_t::TREE_OK) return status;
 
    TreeNode_t* taylor_tree = nullptr;
    
    for (int k = 0; k < n; k++) {
    
        int deriv_idx = func_tree_idx + k;
        TreeNode_t* deriv_tree = diff->forest[deriv_idx]->root;
        
        double fk_x0 = CalcTreeAtPoint(diff, deriv_tree, var_idx, x0, &status);
        
        if (double_cmp(fk_x0, 0) && k > 0) continue;
        
        TreeNode_t* elem = CreateTaylorElem(diff, var_idx, x0, k, fk_x0);
        //printf("%lf %p\n", elem->data.num, elem);
        if (!elem) {
            if (taylor_tree) DeleteNode(taylor_tree);
            return DiffErr_t::MEM_ALLOC_ERR;
        }
        
        if (!taylor_tree) {
            taylor_tree = elem;
        } else {

            TreeNode_t* new_sum = CreateBinOp(Oper_t::ADD, taylor_tree, elem);
            if (!new_sum) {
                DeleteNode(taylor_tree);
                DeleteNode(elem);
                return DiffErr_t::MEM_ALLOC_ERR;
            }
            taylor_tree = new_sum;
        }
    }
    
    if (!taylor_tree) taylor_tree = CreateNumNode(0);

    diff->forest[result_tree_idx]->root = taylor_tree;
    //TreeDump(diff, result_tree_idx, status, nullptr);
    return DiffErr_t::TREE_OK;
}

/*=====================================================================================*/