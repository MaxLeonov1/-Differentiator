#pragma once

#include <math.h>
#include <assert.h>

#include "differentiator.h"
#include "main_func.h"
#include "../log_utils/logger.h"
#include "../utils/colors.h"


/*=====================================================================================*/

typedef TreeNode_t* (*DiffFunc_t)(TreeNode_t*, int, Diff_t*, DiffErr_t*);
typedef double (*CalcFunc_t)(TreeNode_t*, Diff_t*, DiffErr_t*);

/*=====================================================================================*/

extern DiffFunc_t DiffFuncs[];
extern CalcFunc_t CalcFuncs[];

extern const int oper_instr_num;

/*=====================================================================================*/

void InitDiffFuncs();
void InitCalcFuncs();

/*=====================================================================================*/

TreeNode_t* DiffNum(TreeNode_t* node, int dir_var, Diff_t* diff);
TreeNode_t* DiffVar(TreeNode_t* node, int dir_var, Diff_t* diff);

double CalcNum(TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
double CalcVar(TreeNode_t* node, Diff_t* diff, DiffErr_t* status);

/*=====================================================================================*/

TreeNode_t* DiffAdd (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);
TreeNode_t* DiffSub (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);
TreeNode_t* DiffMul (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);
TreeNode_t* DiffDiv (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);
TreeNode_t* DiffDeg (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);
TreeNode_t* DiffLog (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);

double CalcAdd (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
double CalcSub (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
double CalcMul (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
double CalcDiv (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
double CalcDeg (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
double CalcLog (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);

/*=====================================================================================*/

TreeNode_t* DiffSin    (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);
TreeNode_t* DiffCos    (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);
TreeNode_t* DiffLn     (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);
TreeNode_t* DiffTg     (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);
TreeNode_t* DiffCtg    (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);
TreeNode_t* DiffArcsin (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);
TreeNode_t* DiffArccos (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);
TreeNode_t* DiffArctg  (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);
TreeNode_t* DiffArcctg (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);
TreeNode_t* DiffSh     (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);
TreeNode_t* DiffCh     (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);
TreeNode_t* DiffTh     (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);
TreeNode_t* DiffCth    (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);
TreeNode_t* DiffArcsh  (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);
TreeNode_t* DiffArcch  (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);
TreeNode_t* DiffArcth  (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);
TreeNode_t* DiffArccth (TreeNode_t* node, int dir_var, Diff_t* diff, DiffErr_t* status);

double CalcSin    (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
double CalcCos    (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
double CalcTg     (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
double CalcCtg    (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
double CalcLn     (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
double CalcArcsin (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
double CalcArccos (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
double CalcArctg  (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
double CalcArcctg (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
double CalcSh     (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
double CalcCh     (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
double CalcTh     (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
double CalcCth    (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
double CalcArcsh  (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
double CalcArcch  (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
double CalcArcth  (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
double CalcArccth (TreeNode_t* node, Diff_t* diff, DiffErr_t* status);
