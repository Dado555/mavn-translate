#pragma once
#include "Types.h"
#include "Constants.h"
#include "IR.h"

/**
* Method to check if variable exists in list.
*/
bool variableExists(Variable* variable, Variables variables);

/**
* Method for liveness analysis.
*/
void livenessAnalysis(Instructions& instructions);