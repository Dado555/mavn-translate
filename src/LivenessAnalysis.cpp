#include "LivenessAnalysis.h"

bool variableExists(Variable* variable, Variables variables) {
	Variables::iterator varIt = variables.begin();
	for (; varIt != variables.end(); varIt++) {
		if (variable->getName() == (*varIt)->getName() && variable->getVarType() == (*varIt)->getVarType()) {
			return true;
		}
	}
	return false;
}

void livenessAnalysis(Instructions& instructions)
{
	bool doItAgain = true;

	while (doItAgain) {
		doItAgain = false;

		Instructions::reverse_iterator rit = instructions.rbegin();
		for (; rit != instructions.rend(); rit++) {
			// 0. in' = in, out' = out
			Variables& out = (*rit)->getOut();
			Variables& in = (*rit)->getIn();
			Variables newOut;
			Variables newIn;

			// 1. newOut = U succIn
			Instructions& successors = (*rit)->getSucc();
			Instructions::iterator succIt = successors.begin();
			for (; succIt != successors.end(); succIt++) {
				Variables& succIn = (*succIt)->getIn();
				newOut.insert(newOut.end(), succIn.begin(), succIn.end());
			}
			newOut.sort();
			newOut.unique();

			// 2. newIn = use U (out/def)
			Variables& use = (*rit)->getUse();
			Variables& def = (*rit)->getDef();

			Variables outMinusDef;
			Variables::iterator varIt = out.begin();
			for (; varIt != out.end(); varIt++) {
				if (!variableExists(*varIt, def)) {
					outMinusDef.push_back(*varIt);
				}
			}

			newIn = use;
			newIn.insert(newIn.end(), outMinusDef.begin(), outMinusDef.end());
			newIn.sort();
			newIn.unique();

			if (newIn != in || newOut != out) {
				doItAgain = true;
			}

			in = newIn;
			out = newOut;
		}
	}
}