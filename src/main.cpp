#include "tree.h"
#include "differentiator.h"
#include "dump_tex.h"
#include "diff_func.h"



int main() {

    DIFF_INIT(diff_1)
    TREE_INIT(tree_1)
    TREE_INIT(tree_2)

    TreeCtor(&tree_1);
    TreeCtor(&tree_2);
    DiffCtor(&diff_1);

    diff_1.forest[0] = &tree_1;
    diff_1.forest[1] = &tree_2;

    ReadFromDisk(&diff_1, "disk.bin");

    //diff_1.forest[1]->root = CopyTree(diff_1.forest[0]->root, nullptr);

    FindDerivative(&tree_1, &tree_2, 1);
    diff_1.tree_num = 2;

    TreeDump(&diff_1, 0, TreeErr_t::TREE_OK, nullptr);
    TreeDump(&diff_1, 1, TreeErr_t::TREE_OK, nullptr);
    CreateTexLog(&diff_1, 1, "tex_log.tex");

    SaveToDisk(&diff_1, 0, "test_disk.bin");

    DiffDtor(&diff_1);

}
