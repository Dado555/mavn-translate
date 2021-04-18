#include "ResourceAllocation.h"

Variables save;

int getColor(Variable * notColoredVariable, InterferenceGraph * ig) {
	Variables::iterator iter;
	Variables temp;

	// get variable from stack which are interference with notColoredVariable
	for (iter = save.begin(); iter != save.end(); iter++) {
		Variable * variable = (*iter);

		if (ig->matrix[notColoredVariable->getPosition()][variable->getPosition()] == __INTERFERENCE__) {
			temp.push_back(variable);
		}
		else {
			// nothing
		}
	}

	// find diffrent color
	int color = 0;
	bool find;

	for (int color = 0; color < __REG_NUMBER__; color++) {

		find = true;

		for (iter = temp.begin(); iter != temp.end(); iter++) {
			if (color == (*iter)->getAssignment()) {
				find = false;
			}
		}

		if (find == true) {
			return color;
			break;
		}
	}

	return __UNDEFINE__;
}

bool doResourceAllocation(SimplificationStack* simplificationStack, InterferenceGraph * ig) {

	Variable * currentVariable, *previusVariable;

	previusVariable = NULL;

	int counter = 0;

	while (simplificationStack->size() > 0) {

		currentVariable = simplificationStack->top();
		simplificationStack->pop();

		save.push_back(currentVariable);

		if (previusVariable == NULL) {
			// first variable on stact
			currentVariable->setAssignment((Regs)counter);
		}
		else {
			// other varables
			int color = getColor(currentVariable, ig);
			if (color == __UNDEFINE__) {
				// actual spill                
				return false;
			}
			else {
				currentVariable->setAssignment((Regs)color);
			}
		}

		previusVariable = currentVariable;
	}

	ig->variables = &save;

	return true;
}
