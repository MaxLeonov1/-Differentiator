#include "tree.h"
#include "differentiator.h"
#include "dump_tex.h"
#include "diff_func.h"



int main() {

    DIFF_INIT(diff_1)
    TREE_INIT(tree_1)

    TreeCtor(&tree_1);
    DiffCtor(&diff_1);

    diff_1.tree = &tree_1;

    // Val_t value = {.oper = Oper_t::MULT};
    // Val_t value2 = {.num = 1};
    // Val_t value3 = {.var_idx = 0};

    // AddToNameTable(&diff_1, "x");

    // InsertNode(&tree_1.root, value, Node_t::OP, tree_1.root);
    // InsertNode(&tree_1.root->left, value2, Node_t::NUM, tree_1.root);
    // CreateNodeFromStr("*", &diff_1, &tree_1.root->right);
    //InsertNode(&tree_1.root->right, value3, Node_t::VAR, tree_1.root);

    ReadFromDisk(&diff_1, "disk.bin");

    TreeDump(&diff_1, TreeErr_t::TREE_OK, nullptr);
    CreateTexLog(&diff_1, "tex_log.tex");

    SaveToDisk(&diff_1, "test_disk.bin");

    TreeDtor(&tree_1);
    DiffDtor(&diff_1);

}
