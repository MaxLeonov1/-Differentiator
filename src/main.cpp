#include "tree.h"
#include "differentiator.h"



int main() {
    
    TREE_INIT(tree_1)

    TreeCtor(&tree_1);

    Val_t value = {.oper = Oper_t::MULT};
    Val_t value2 = {.num = 1};
    Val_t value3 = {.num = 1};

    InsertNode(&tree_1.root, &value, Node_t::OP, tree_1.root);
    InsertNode(&tree_1.root->left, &value2, Node_t::NUM, tree_1.root);
    InsertNode(&tree_1.root->right, &value3, Node_t::NUM, tree_1.root);
    TreeDump(&tree_1, TreeErr_t::TREE_OK, nullptr);

    TreeDtor(&tree_1);

}
