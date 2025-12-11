#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "input_output_func.h"


#define MAX_OP_LEN_ 10

#define DEF_X_MAX_ 10
#define DEF_X_MIN_ -10
#define DEF_Y_MAX_ 10
#define DEF_Y_MIN_ -10

#define DEF_X0_ 0
#define DEF_VAR_ 'x'
#define DEF_DEG_ 2

/*=====================================================================================*/

static void GetGraphParams      (Diff_t* diff, char** str);
static void GetTangentParams    (Diff_t* diff, char** str);
static void GetDerivativeParams (Diff_t* diff, char** str);
static void GetTaylorParams     (Diff_t* diff, char** str);
static void ParseParams         (Diff_t* diff, char** str);

static TreeNode_t* GetE    (Diff_t* diff, char** str);
static TreeNode_t* GetT    (Diff_t* diff, char** str);
static TreeNode_t* GetFunc (Diff_t* diff, char** str);
static TreeNode_t* GetDeg  (Diff_t* diff, char** str);
static TreeNode_t* GetP    (Diff_t* diff, char** str);
static TreeNode_t* GetV    (char** str, Diff_t* diff);
static TreeNode_t* GetN    (char** str);

/*=====================================================================================*/

TreeNode_t* GetExpr (Diff_t* diff, char** str, DiffErr_t* status) {

    assert(diff);
    assert(str);
    assert(*str);

    skip_space(str);
    TreeNode_t* val = GetE(diff, str);
    
    if (**str != '$') {
        *status = DiffErr_t::READ_SYNTAX_ERR;
        return val;
    }
    (*str)++;

    /*
    --TAN
    --DIF
    --GRH
    --TLR
    */

    skip_space(str);
    ParseParams(diff, str);

    // printf(
    //     "TAN:\n"
    //     "*make:%d\n"
    //     "*%lf\n"
    //     "DIF:\n"
    //     "*make:%d\n"
    //     "*%d\n"
    //     "*%c\n"
    //     "TAYLOR:\n"
    //     "*make:%d\n"
    //     "*%lf\n"
    //     "GRAPH:\n"
    //     "*make:%d\n"
    //     "*%lf\n"
    //     "*%lf\n"
    //     "*%lf\n"
    //     "*%lf\n",
    //     diff->params.tan.make,
    //     diff->params.tan.x0,
    //     diff->params.dir.make,
    //     diff->params.dir.degree,
    //     diff->params.dir.var,
    //     diff->params.taylor.make,
    //     diff->params.taylor.x0,
    //     diff->params.graph.make,
    //     diff->params.graph.x_max,
    //     diff->params.graph.x_min,
    //     diff->params.graph.y_max,
    //     diff->params.graph.y_min);

    return val;

}

/*=====================================================================================*/

static void ParseParams (Diff_t* diff, char** str) {
    
    assert(diff);
    assert(str);
    assert(*str);
    
    skip_space(str);
    
    while (**str != '\0' && **str != '\n' && **str != '\r') {
        
        skip_space(str);
        
        if (strncmp(*str, "--", 2) != 0) {
            break;
        }
        
        (*str) += 2;
        
        if (strncmp(*str, "GRH", 3) == 0) {
            (*str) += 3;
            diff->params.graph.make = 1;
            GetGraphParams(diff, str);
        } 
        else if (strncmp(*str, "TAN", 3) == 0) {
            (*str) += 3;
            diff->params.tan.make = 1;
            GetTangentParams(diff, str);
        } 
        else if (strncmp(*str, "DIF", 3) == 0) {
            (*str) += 3;
            diff->params.dir.make = 1;
            GetDerivativeParams(diff, str);
        } 
        else if (strncmp(*str, "TLR", 3) == 0) {
            (*str) += 3;
            diff->params.taylor.make = 1;
            GetTaylorParams(diff, str);
        } 
        else {
            
            while (**str != '\0' && strncmp(*str, "--", 2) != 0) {
                (*str)++;
            }
            continue;
        }
        
        skip_space(str);
    }
}

/*=====================================================================================*/

static void GetGraphParams (Diff_t* diff, char** str) {

    double min = 0, max = 0;
    int len = 0, scanned = 0;

    scanned = sscanf(*str, " x[%lf:%lf]%n", &min, &max, &len );
    *(*str+=len);
    
    if (scanned == 2 && max > min) {
        diff->params.graph.x_max = max;
        diff->params.graph.x_min = min;

    } else {
        diff->params.graph.x_max = DEF_X_MAX_;
        diff->params.graph.x_min = DEF_X_MIN_;

    }
    len = 0;
    scanned = sscanf(*str, " y[%lf:%lf]%n", &min, &max, &len );
    *(*str+=len);

    if (scanned == 2 && max > min) {
        diff->params.graph.y_max = max;
        diff->params.graph.y_min = min;

    } else {
        diff->params.graph.y_max = DEF_Y_MAX_;
        diff->params.graph.y_min = DEF_Y_MIN_;

    }

    // printf(
    //     "%lf:%lf  %lf:%lf\n",
    //     diff->log_params.graph.x_min,
    //     diff->log_params.graph.x_max,
    //     diff->log_params.graph.y_min,
    //     diff->log_params.graph.y_max);

}

/*=====================================================================================*/

static void GetTangentParams (Diff_t* diff, char** str) {

    double x0 = 0;
    int len = 0, scanned = 0;

    scanned = sscanf(*str, " x0[%lf]%n", &x0, &len );
    *(*str+=len);

    if (scanned == 1)
        diff->params.tan.x0 = x0;
    else
        diff->params.tan.x0 = DEF_X0_;

}

/*=====================================================================================*/

static void GetDerivativeParams (Diff_t* diff, char** str) {

    int var = 0, deg = 0;
    int len = 0, scanned = 0;

    scanned = sscanf(*str, " var[%c]%n", &var, &len );
    *(*str+=len);

    if (scanned == 1)
        diff->params.dir.var = var;
    else
        diff->params.dir.var = DEF_VAR_;

    len = 0;
    scanned = sscanf(*str, " deg[%d]%n", &deg, &len );
    *(*str+=len);

    if (scanned == 1)
        diff->params.dir.degree = deg;
    else
        diff->params.dir.degree = DEF_DEG_;

}

/*=====================================================================================*/

static void GetTaylorParams (Diff_t* diff, char** str) {

    double x0 = 0;
    int len = 0, scanned = 0;

    scanned = sscanf(*str, " x0[%lf]%n", &x0, &len );
    *(*str+=len);

    if (scanned == 1)
        diff->params.taylor.x0 = x0;
    else
        diff->params.taylor.x0 = DEF_X0_;

}

/*=====================================================================================*/

static TreeNode_t* GetE (Diff_t* diff, char** str) {

    skip_space(str);

    TreeNode_t* val = GetT(diff, str);
    skip_space(str);

    while(**str == '+' || **str == '-') {
        int op = **str;
        (*str)++;

        TreeNode_t* val2 = GetT(diff, str);
        skip_space(str);
        
        if (op == '+')
            val = CreateBinOp(Oper_t::ADD, val, val2);
        else
            val = CreateBinOp(Oper_t::SUB, val, val2);
    }

    skip_space(str);
    return val;


}

/*=====================================================================================*/

static TreeNode_t* GetT (Diff_t* diff,  char** str) {

    skip_space(str);

    TreeNode_t* val = GetDeg(diff, str);
    skip_space(str);
    
    while(**str == '/' || **str == '*') {
        int op = **str;
        (*str)++;

        TreeNode_t* val2 = GetDeg(diff, str);
        skip_space(str);

        if (op == '*')
            val = CreateBinOp(Oper_t::MULT, val, val2);
        else
            val = CreateBinOp(Oper_t::DIV, val, val2);
    }

    skip_space(str);
    return val;

}

/*=====================================================================================*/

static TreeNode_t* GetDeg (Diff_t* diff,  char** str) {

    skip_space(str);

    TreeNode_t* val = GetFunc(diff, str);

    while(**str == '^') {

        (*str)++;
        TreeNode_t* val2 = GetFunc(diff, str);

        val = CreateBinOp(Oper_t::DEG, val, val2);
    }
    return val;


}

/*=====================================================================================*/

static TreeNode_t* GetFunc (Diff_t* diff,  char** str) {

    skip_space(str);

    char buff[MAX_OP_LEN_] = "";
    int len = 0;
    
    sscanf(*str, "%9[^ (]%n", buff, &len);
    u_int hash = djb2hash(buff);

    int idx = HashBinSearch(diff->sort_op_instr, diff->op_num, hash);

    if (idx == -1) {
        return GetP(diff, str);

    } else if (diff->sort_op_instr[idx].is_sing) {
        (*str)+=len;

        TreeNode_t* op_val = GetP(diff, str);
        TreeNode_t* val = CreateUnOp(diff->sort_op_instr[idx].code, op_val);
        return val;
    }

}

/*=====================================================================================*/

static TreeNode_t* GetP (Diff_t* diff,  char** str) {

    skip_space(str);
    TreeNode_t* val = nullptr;

    if (**str == '(') {
        (*str)++;
        val = GetE(diff, str);
        (*str)++;
        return val;

    } else if (isdigit(**str)) {
        val = GetN(str);
        return val;
    
    } else {
        val = GetV(str, diff);
        return val;

    }
}

/*=====================================================================================*/

static TreeNode_t* GetN ( char** str) {

    skip_space(str);

    int val = 0;

    while('0'<=**str && **str<='9') {
        val = val*10 + (**str-'0');
        (*str)++;
    }

    return CreateNumNode(double(val));
}

/*=====================================================================================*/
 
static TreeNode_t* GetV ( char** str, Diff_t* diff) {;

    skip_space(str);
    
    AddToNameTable(diff, **str);
    
    (*str)++;

    return CreateVarNode(diff->name_table.num-1);

}

/*=====================================================================================*/

#undef MAX_OP_LEN_

#undef DEF_X_MAX_
#undef DEF_X_MIN_
#undef DEF_Y_MAX_
#undef DEF_Y_MIN_

#undef DEF_X0_
#undef DEF_VAR_
#undef DEF_DEG_
