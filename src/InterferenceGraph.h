#pragma once

#include "IR.h"

/**
* Builds the interference graph based on instruction list of the analyzed program.
* @param - instructions
*	List of instructions
* @return
*	Reference to the interference graph
*/
InterferenceGraph* buildInterferenceGraph(Instructions& instructions, Variables& variables);

/**
* Get only registry variables from list of all variables
*/
Variables* onlyRegistryVariables(Variables& variableList);

/**
* Prints the interference graph
*/
void printInterferenceMatrix(std::ostream& out, InterferenceGraph& ig);
