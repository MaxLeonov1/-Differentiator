#include "../src/tree.h"



TreeNode_t* GetG    (Diff_t* diff, char** str, DiffErr_t* status);
TreeNode_t* GetE    (Diff_t* diff, char** str);
TreeNode_t* GetT    (Diff_t* diff, char** str);
TreeNode_t* GetFunc (Diff_t* diff, char** str);
TreeNode_t* GetDeg  (Diff_t* diff, char** str);
TreeNode_t* GetP    (Diff_t* diff, char** str);
TreeNode_t* GetV    (char** str, Diff_t* diff);
TreeNode_t* GetN    (char** str);