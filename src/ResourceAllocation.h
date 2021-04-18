#pragma once
#include "IR.h"

/**
* Method for allocating register for variable.
*/
int getColor(Variable * notColoredVariable, InterferenceGraph * ig);

/**
* Method for allocating registers for all registry variables.
*/
bool doResourceAllocation(SimplificationStack* ss, InterferenceGraph* ig);