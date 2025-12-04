#include "tree.h"
#include "errors.h"

const char* StatusCodeToStr (DiffErr_t status);
DiffErr_t   VerifyTree      (TreeNode_t* node);