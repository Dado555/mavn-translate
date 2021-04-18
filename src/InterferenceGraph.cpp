#include "InterferenceGraph.h"

#include <iostream>
#include <vector>

InterferenceGraph* buildInterferenceGraph(Instructions& instructions, Variables& variables) {
	InterferenceGraph* ig = new InterferenceGraph();
	ig->variables = onlyRegistryVariables(variables);
	ig->matrix.resize(ig->variables->size());
	for (int i = 0; i < ig->matrix.size(); i++)
		ig->matrix[i].resize(ig->matrix.size());
	//every instruction
	for (Instructions::iterator instructionIter = instructions.begin(); instructionIter != instructions.end(); instructionIter++) {
		// every def variable
		for (Variables::iterator defVarIter = (*instructionIter)->getDef().begin(); defVarIter != (*instructionIter)->getDef().end(); defVarIter++) {
			// every out variable
			for (Variables::iterator outVarIter = (*instructionIter)->getOut().begin(); outVarIter != (*instructionIter)->getOut().end(); outVarIter++) {
				// interferences?
				if ((*defVarIter)->getPosition() != (*outVarIter)->getPosition() && (*defVarIter)->getVarType()==REG_VAR && (*outVarIter)->getVarType()==REG_VAR) {
					ig->matrix[(*defVarIter)->getPosition()][(*outVarIter)->getPosition()] = __INTERFERENCE__;
					ig->matrix[(*outVarIter)->getPosition()][(*defVarIter)->getPosition()] = __INTERFERENCE__;
				}
			}
		}
	}
	return ig;
}

Variables* onlyRegistryVariables(Variables& variableList) {
	Variables* registry = new Variables();
	int position = 0;
	for (Variable* var : variableList) {
		if (var->getVarType() == REG_VAR) {
			var->setPosition(position++);
			registry->push_back(var);
		}
	}
	return registry;
}

void printInterferenceMatrix(std::ostream& out, InterferenceGraph& ig) {
	if (ig.matrix.size() == 0)
	{
		out << "Interference matrix empty!" << std::endl;
		return;
	}

	for (auto varIt = ig.variables->begin(); varIt != ig.variables->end(); varIt++)
	{
		out << "\t" << (*varIt)->getName();
	}
	out << std::endl;

	auto varIt = ig.variables->begin();
	for (auto it1 = ig.matrix.begin(); it1 != ig.matrix.end(); it1++)
	{
		out << (*varIt++)->getName();
		for (auto it2 = (*it1).begin(); it2 != (*it1).end(); it2++)
		{
			out << "\t" << *it2;
		}
		out << std::endl;
	}
}
