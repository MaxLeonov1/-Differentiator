#include <stdio.h>
#include <assert.h>
#include <ctype.h>

#include "input.h"
#include "../utils/sup_func.h"


#define MAX_OP_LEN_ 10


TreeNode_t* GetG (Diff_t* diff, char** str, DiffErr_t* status) {

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

    return val;

}



TreeNode_t* GetE (Diff_t* diff, char** str) {

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



TreeNode_t* GetT (Diff_t* diff,  char** str) {

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



TreeNode_t* GetDeg (Diff_t* diff,  char** str) {

    skip_space(str);

    TreeNode_t* val = GetFunc(diff, str);

    while(**str == '^') {

        (*str)++;
        TreeNode_t* val2 = GetFunc(diff, str);

        val = CreateBinOp(Oper_t::DEG, val, val2);
    }
    return val;


}



TreeNode_t* GetFunc (Diff_t* diff,  char** str) {

    skip_space(str);

    char buff[MAX_OP_LEN_] = "";
    int len = 0;

    sscanf(*str, "%[^ (]%n", buff, &len);
    u_int hash = djb2hash(buff);

    int idx = HashBinSearch(diff->sort_op_instr, diff->op_num, hash);

    if (idx == -1) {
        return GetP(diff, str);

    } else if (diff->sort_op_instr[idx].is_sing) {
        (*str)+=len;
        
        return CreateUnOp(diff->sort_op_instr[idx].code, GetP(diff, str));
    }

}



TreeNode_t* GetP (Diff_t* diff,  char** str) {

    skip_space(str);

    if (**str == '(') {
        (*str)++;
        TreeNode_t* val = GetE(diff, str);
        (*str)++;
        return val;

    } else if (isdigit(**str)) 
        return GetN(str);
    
    else return GetV(str, diff);

}



TreeNode_t* GetN ( char** str) {

    skip_space(str);

    int val = 0;

    while('0'<=**str && **str<='9') {
        val = val*10 + (**str-'0');
        (*str)++;
    }
    
    return CreateNumNode(double(val));
}



TreeNode_t* GetV ( char** str, Diff_t* diff) {;

    skip_space(str);

    AddToNameTable(diff, **str);

    (*str)++;

    return CreateVarNode(diff->name_table.num-1);

}