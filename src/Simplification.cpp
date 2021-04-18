#include "Simplification.h"

SimplificationStack* doSimplification(InterferenceGraph* ig, int degree) {
	SimplificationStack* stack = new SimplificationStack();
	Variables variables;
	std::map<Variable*, int> neighbours;
	Variables::iterator iterator;
	for (iterator = (*(ig->variables)).begin(); iterator != (*(ig->variables)).end(); iterator++) {
		std::vector<int> row = ig->matrix[(*iterator)->getPosition()];
		int countInterferences = 0;
		for (std::vector<int>::iterator it = row.begin(); it != row.end(); it++) {
			if ((*it) == __INTERFERENCE__)
				countInterferences++;
		}
		neighbours[*iterator] = countInterferences;
	}
	for (iterator = (*(ig->variables)).begin(); iterator != (*(ig->variables)).end(); iterator++) {
		if (neighbours[*iterator] < degree) {
			stack->push(*iterator);
			for (Variables::iterator iter = (*(ig->variables)).begin(); iter != (*(ig->variables)).end(); iter++) {
				if (ig->matrix[(*iterator)->getPosition()][(*iter)->getPosition()] == __INTERFERENCE__) {
					neighbours[*iter]--;
				}
			}
		}
		else
			variables.push_back(*iterator);
	}
	for (Variable* var : variables) {
		stack->push(var);
	}
	return stack;
}
