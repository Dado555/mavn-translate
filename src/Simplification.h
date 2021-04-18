#ifndef __SIMPLIFICATION_H__
#define __SIMPLIFICATION_H__

#include "IR.h"

#include <stack>
#include <map>

/**
* Use this function to do simplification algorithm.
* @param ig pointer to interference graph
* @param degree numer of registers on hardware platform
* @return simplification stack
*/
SimplificationStack* doSimplification(InterferenceGraph* ig, int degree);

#endif
