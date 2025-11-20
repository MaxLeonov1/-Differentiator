#include <assert.h>

#include "diff_func.h"



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

}



TreeNode_t* Differentiate ( TreeNode_t* node, TreeNode_t* prev_node, size_t dir_var) {

    if (node->type == Node_t::NUM) {

        return CreateNumNode(0, prev_node);
    }

    if (node->type == Node_t::VAR) {

        if (node->data.var_idx == dir_var)
            return CreateNumNode(1, prev_node);
        else 
            return CreateNumNode(0, prev_node);
    }

    if (node->type == Node_t::OP_BIN) {

        switch(node->type) {

            case Oper_t::ADD:
            case Oper_t::SUB:
                return CB_(node->data.oper, dL_, dR_, prev_node);

            case Oper_t::MULT:
                return CB_(SUM_, CB_(MUL_, dL_, cR_, prev_node), CB_(MUL_, cL_, dR_, prev_node), prev_node);

            case Oper_t::DIV:
                return CB_(DIV_,
                            CB_(SUB_, CB_(MUL_, dL_, cR_, prev_node), CB_(MUL_, cL_, dR_, prev_node), prev_node), 
                            CB_(MUL_, cL_, cR_, prev_node), prev_node);

        }
    }

    if (node->type == Node_t::OP_UN) {

        switch(node->type) {

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
        }
    }

}