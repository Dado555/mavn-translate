#include <iostream>
#include <iomanip>

#include "SyntaxAnalysis.h"

using namespace std;


SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex)
	: lexicalAnalysis(lex), errorFound(false), tokenIterator(lexicalAnalysis.getTokenList().begin()), registryNum(0), instructionNum(0) {}


bool SyntaxAnalysis::Do()
{
	currentToken = getNextToken();
	Q();
	return !errorFound;
}


void SyntaxAnalysis::printSyntaxError(Token token)
{
	cout << "Syntax error! Token: " << token.getValue() << " unexpected" << endl;
}


void SyntaxAnalysis::printTokenInfo(Token token)
{
	cout << setw(20) << left << token.tokenTypeToString(token.getType());
	cout << setw(25) << right << token.getValue() << endl;
}


void SyntaxAnalysis::eat(TokenType t)
{
	if (errorFound == false)
	{
		if (currentToken.getType() == t)
		{
			//cout << currentToken.getValue() << endl;
			if (t == T_END_OF_FILE)
				return;
			currentToken = getNextToken();
		}
		else
		{
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
}


Token SyntaxAnalysis::getNextToken()
{
	if (tokenIterator == lexicalAnalysis.getTokenList().end())
		throw runtime_error("End of input file reached");
	return *tokenIterator++;
}


void SyntaxAnalysis::Q()
{
	if (errorFound == false)
	{
		S();
		eat(T_SEMI_COL);
		L();
	}
}


void SyntaxAnalysis::S()
{
	if (errorFound == false)
	{
		if (currentToken.getType() == T_MEM) {
			eat(T_MEM);
			Variable* memory = new Variable(currentToken.getValue(), MEM_VAR);
			eat(T_M_ID);
			memory->setNum(atoi(&currentToken.getValue()[0]));
			if (variableInList(memory->getName(), MEM_VAR)) {
				errorFound = true;
				std::string message = "\nSyntax error! Token: " + memory->getName() + " unexpected\n" + memory->getName() + " - variable already exists\n";
				throw std::runtime_error(message);
			}
			if (memory->getName()[0] != 'm') {
				errorFound = true;
				std::string message = "\nSyntax error! Token: " + memory->getName() + " unexpected\n" + memory->getName() + " - variable name incorect. Must start with \'m\'\n";
				throw std::runtime_error(message);
			}
			variableList.push_back(memory);
			eat(T_NUM);
		}
		else if (currentToken.getType() == T_REG) {
			eat(T_REG);
			Variable* registry = new Variable(currentToken.getValue(), REG_VAR, registryNum++);
			if (variableInList(registry->getName(), REG_VAR)) {
				errorFound = true;
				std::string message = "\nSyntax error! Token: " + registry->getName() + " unexpected\n" + registry->getName() + " - variable already exists\n";
				throw std::runtime_error(message);
			}
			if (registry->getName()[0] != 'r') {
				errorFound = true;
				std::string message = "\nSyntax error! Token: " + registry->getName() + " unexpected\n" + registry->getName() + " - variable name incorect. Must start with \'r\'\n";
				throw std::runtime_error(message);
			}
			variableList.push_back(registry);
			eat(T_R_ID);
		}
		else if (currentToken.getType() == T_FUNC) {
			eat(T_FUNC);
			if (!funcNameCorrect(currentToken.getValue())) {
				errorFound = true;
				std::string message = "\nSyntax error! Token: " + currentToken.getValue() + " unexpected\n" + currentToken.getValue() + " - function name incorect or already exists\n";
				throw std::runtime_error(message);
			}
			Label* funcLabel = new Label(currentToken.getValue(), instructionNum);
			labelList.push_back(funcLabel);
			functionList.push_back(funcLabel);
			eat(T_ID);
		}
		else if (currentToken.getType() == T_ID) {
			Label* idLabel = new Label(currentToken.getValue(), instructionNum);
			if (labelInList(idLabel)) {
				errorFound = true;
				std::string message = "\nSyntax error! Token: " + idLabel->getName() + " unexpected\n" + idLabel->getName() + " - label already exists\n";
				throw std::runtime_error(message);
			}
			labelList.push_back(idLabel);
			findAndDelete(neededLabels, currentToken.getValue());
			eat(T_ID);
			eat(T_COL);
			E();
		}
		else {
			E();
		}
	}
}

void SyntaxAnalysis::findAndDelete(std::vector<string>& vector, std::string& val){
	std::vector<string> temp;
	std::vector<string>::iterator first = vector.begin();
	std::vector<string>::iterator last = vector.end();
	int counter = 0;
	while (first != last) {
		if (*first != val) {
			temp.push_back(*first);
		}
		else {
			for (auto lbl = labelList.begin(); lbl != labelList.end(); lbl++) {
				if ((*lbl)->getName() == val) {
					tempInstructions[counter]->setLabel(*lbl);
				}
			}
		}
		++first;
		counter++;
	}
	vector.clear();
	for (string s : temp)
		vector.push_back(s);
}

bool SyntaxAnalysis::funcNameCorrect(std::string name) {
	if (!isalpha(name[0])) {
		return false;
	}
	if (name.length() > 1) {
		for (size_t i = 1; i < name.length(); i++) {
			if (!isalnum(name[i])) {
				return false;
			}
		}
	}
	for (auto it = functionList.begin(); it != functionList.end(); it++) {
		if ((*it)->getName() == name) {
			return false;
		}
	}
	return true;
}


bool SyntaxAnalysis::labelInList(Label* label) {
	for (auto it = labelList.begin(); it != labelList.end(); it++) {
		if (label->getName() == (*it)->getName())
			return true;
	}
	return false;
}

bool SyntaxAnalysis::functionInList(std::string name) {
	for (auto it = functionList.begin(); it != functionList.end(); it++) {
		if ((*it)->getName() == name) {
			return true;
		}
	}
	return false;
}

void SyntaxAnalysis::L()
{
	if (errorFound == false)
	{
		if (currentToken.getType() == T_END_OF_FILE) {
			eat(T_END_OF_FILE);
			if (!functionInList("main")) {
				std::string message = "\nSyntax error! main function expected but not found\n";
				throw std::runtime_error(message);
			}
			if (!neededLabels.empty()) {
				std::string message = "";
				for (auto lab : neededLabels) {
					message += "\nSyntax error! " + lab + " label not found\n";
				}
				throw std::runtime_error(message);
			}
			fillSuccAndPred(instructionList);
		}
		else {
			Q();
		}
	}
}


void SyntaxAnalysis::E()
{
	if (errorFound == false)
	{
		TokenType currToken = currentToken.getType();
		if (currToken == T_ADD) {
			eat(T_ADD);
			Instruction* addInstruction = new Instruction(I_ADD);
			// napuni src, def, use, dst
			fillVariableLists(addInstruction, REG_VAR, true);
			eat(T_R_ID);
			eat(T_COMMA);
			fillVariableLists(addInstruction, REG_VAR, false);
			eat(T_R_ID);
			eat(T_COMMA);
			fillVariableLists(addInstruction, REG_VAR, false);
			eat(T_R_ID);
			addInstruction->setPosition(instructionNum++);
			instructionList.push_back(addInstruction);
		}
		else if (currToken == T_ADDI) {
			eat(T_ADDI);
			Instruction* addiInstruction = new Instruction(I_ADDI);
			fillVariableLists(addiInstruction, REG_VAR, true);
			eat(T_R_ID);
			eat(T_COMMA);
			fillVariableLists(addiInstruction, REG_VAR, false);
			eat(T_R_ID);
			eat(T_COMMA);
			addiInstruction->setNumber(atoi(&currentToken.getValue()[0]));
			eat(T_NUM);
			addiInstruction->setPosition(instructionNum++);
			instructionList.push_back(addiInstruction);
		}
		else if (currToken == T_B) {
			eat(T_B);
			Instruction* branchInstruction = new Instruction(I_B);
			bool labelExist = branchInstructionLabel(branchInstruction);
			if (!labelExist) {
				neededLabels.push_back(currentToken.getValue());
			}
			eat(T_ID);
			branchInstruction->setPosition(instructionNum++);
			instructionList.push_back(branchInstruction);
			if (!labelExist) {
				tempInstructions.push_back(branchInstruction);
			}
		}
		else if (currToken == T_BLTZ) {
			eat(T_BLTZ);
			Instruction* bltzInstruction = new Instruction(I_BLTZ);
			fillVariableLists(bltzInstruction, REG_VAR, false);
			eat(T_R_ID);
			eat(T_COMMA);
			bool labelExist = branchInstructionLabel(bltzInstruction);
			if (!labelExist) {
				neededLabels.push_back(currentToken.getValue());
			}
			eat(T_ID);
			bltzInstruction->setPosition(instructionNum++);
			instructionList.push_back(bltzInstruction);
			if (!labelExist) {
				tempInstructions.push_back(bltzInstruction);
			}
		}
		else if (currToken == T_LA) {
			eat(T_LA);
			Instruction* laInstruction = new Instruction(I_LA);
			fillVariableLists(laInstruction, REG_VAR, true);
			eat(T_R_ID);
			eat(T_COMMA);
			fillVariableLists(laInstruction, MEM_VAR, false);
			eat(T_M_ID);
			laInstruction->setPosition(instructionNum++);
			instructionList.push_back(laInstruction);
		}
		else if (currToken == T_LI) {
			eat(T_LI);
			Instruction* liInstruction = new Instruction(I_LI);
			fillVariableLists(liInstruction, REG_VAR, true);
			eat(T_R_ID);
			eat(T_COMMA);
			liInstruction->setNumber(atoi(&currentToken.getValue()[0]));
			eat(T_NUM);
			liInstruction->setPosition(instructionNum++);
			instructionList.push_back(liInstruction);
		}
		else if (currToken == T_LW) {
			eat(T_LW);
			Instruction* lwInstruction = new Instruction(I_LW);
			fillVariableLists(lwInstruction, REG_VAR, true);
			eat(T_R_ID);
			eat(T_COMMA);
			lwInstruction->setNumber(atoi(&currentToken.getValue()[0]));
			eat(T_NUM);
			eat(T_L_PARENT);
			fillVariableLists(lwInstruction, REG_VAR, false);
			eat(T_R_ID);
			eat(T_R_PARENT);
			lwInstruction->setPosition(instructionNum++);
			instructionList.push_back(lwInstruction);
		}
		else if (currToken == T_NOP) {
			eat(T_NOP);
			Instruction* nopInstruction = new Instruction(I_NOP);
			nopInstruction->setPosition(instructionNum++);
			instructionList.push_back(nopInstruction);
		}
		else if (currToken == T_SUB) {
			eat(T_SUB);
			Instruction* subInstruction = new Instruction(I_SUB);
			fillVariableLists(subInstruction, REG_VAR, true);
			eat(T_R_ID);
			eat(T_COMMA);
			fillVariableLists(subInstruction, REG_VAR, false);
			eat(T_R_ID);
			eat(T_COMMA);
			fillVariableLists(subInstruction, REG_VAR, false);
			eat(T_R_ID);
			subInstruction->setPosition(instructionNum++);
			instructionList.push_back(subInstruction);
		}
		else if (currToken == T_SW) {
			eat(T_SW);
			Instruction* swInstruction = new Instruction(I_SW);
			fillVariableLists(swInstruction, REG_VAR, true);
			eat(T_R_ID);
			eat(T_COMMA);
			swInstruction->setNumber(atoi(&currentToken.getValue()[0]));
			eat(T_NUM);
			eat(T_L_PARENT);
			fillVariableLists(swInstruction, REG_VAR, false);
			eat(T_R_ID);
			eat(T_R_PARENT);
			swInstruction->setPosition(instructionNum++);
			instructionList.push_back(swInstruction);
		}
		else if (currToken == T_AND) {
			eat(T_AND);
			Instruction* andInstruction = new Instruction(I_AND);
			fillVariableLists(andInstruction, REG_VAR, true);
			eat(T_R_ID);
			eat(T_COMMA);
			fillVariableLists(andInstruction, REG_VAR, false);
			eat(T_R_ID);
			eat(T_COMMA);
			fillVariableLists(andInstruction, REG_VAR, false);
			eat(T_R_ID);
			andInstruction->setPosition(instructionNum++);
			instructionList.push_back(andInstruction);
		}
		else if (currToken == T_OR) {
			eat(T_OR);
			Instruction* orInstruction = new Instruction(I_OR);
			fillVariableLists(orInstruction, REG_VAR, true);
			eat(T_R_ID);
			eat(T_COMMA);
			fillVariableLists(orInstruction, REG_VAR, false);
			eat(T_R_ID);
			eat(T_COMMA);
			fillVariableLists(orInstruction, REG_VAR, false);
			eat(T_R_ID);
			orInstruction->setPosition(instructionNum++);
			instructionList.push_back(orInstruction);
		}
		else if (currToken == T_LB) {
			eat(T_LB);
			Instruction* lbInstruction = new Instruction(I_LB);
			fillVariableLists(lbInstruction, REG_VAR, true);
			eat(T_R_ID);
			eat(T_COMMA);
			lbInstruction->setNumber(atoi(&currentToken.getValue()[0]));
			eat(T_NUM);
			eat(T_L_PARENT);
			fillVariableLists(lbInstruction, REG_VAR, false);
			eat(T_R_ID);
			eat(T_R_PARENT);
			lbInstruction->setPosition(instructionNum++);
			instructionList.push_back(lbInstruction);
		}
		else {
			errorFound = true; 
			std::string message = currentToken.getValue()+ " - function name not recognized.";
			throw std::runtime_error(message);
		}
	}
}

void SyntaxAnalysis::fillVariableLists(Instruction* instruction, VariableType type, bool dstOrSrc) {
	if (!variableInList(currentToken.getValue(), type)) {
		errorFound = true;
		if (type == REG_VAR) {
			std::string message = "\nSyntax error! Token: " + currentToken.getValue() + "\n" + currentToken.getValue() + " - variable doesn't exist\n";
			throw std::runtime_error(message);
		}
		else if (type == MEM_VAR) {
			std::string message = "\nSyntax error! Token: " + currentToken.getValue() + "\n" + currentToken.getValue() + " - variable doesn't exist\n";
			throw std::runtime_error(message);
		}
	}
	if (dstOrSrc) {
		instruction->addVariable(instruction->getDst(), findVariable(currentToken.getValue()));
	}
	else {
		instruction->addVariable(instruction->getSrc(), findVariable(currentToken.getValue()));
	}
	if (instruction->getType() == I_ADD || instruction->getType() == I_SUB || instruction->getType() == I_ADDI || instruction->getType() == I_AND || instruction->getType() == I_OR) {
		if (dstOrSrc)
			instruction->addVariable(instruction->getDef(), findVariable(currentToken.getValue()));
		else
			instruction->addVariable(instruction->getUse(), findVariable(currentToken.getValue()));
	}
	else if (instruction->getType() == I_LA || instruction->getType() == I_LB || instruction->getType() == I_LI || instruction->getType() == I_LW || instruction->getType() == I_SW) {
		if (dstOrSrc)
			instruction->addVariable(instruction->getDef(), findVariable(currentToken.getValue()));
	}
	else if (instruction->getType() == I_BLTZ) {
		if (!dstOrSrc)
			instruction->addVariable(instruction->getUse(), findVariable(currentToken.getValue()));
	}
}

bool SyntaxAnalysis::branchInstructionLabel(Instruction* instruction) {
	for (auto lbl = labelList.begin(); lbl != labelList.end(); lbl++) {
		if ((*lbl)->getName() == currentToken.getValue()) {
			instruction->setLabel(*lbl);
			return true;
		}
	}
	return false;
}

Variable* SyntaxAnalysis::findVariable(std::string name) {
	for (auto it = variableList.begin(); it != variableList.end(); it++) {
		if ((*it)->getName() == name)
			return *it;
	}
	return nullptr;
}

bool SyntaxAnalysis::variableInList(std::string name, VariableType type) {
	for (auto it = variableList.begin(); it != variableList.end(); it++) {
		if ((*it)->getName()== name && (*it)->getVarType() == type)
			return true;
	}
	return false;
}

void SyntaxAnalysis::fillSuccAndPred(Instructions& list) {
	for (auto i = list.begin(); i != list.end(); i++) {
		if ((*i)->getType() == InstructionType::I_B) {
			for (auto j = list.begin(); j != list.end(); j++) {
				if ((*j)->getPosition() == (*i)->getLabel()->getNum()) {
					(*i)->getSucc().push_back(*j);
					(*j)->getPred().push_back(*i);
					//cout << endl << endl << "New succ of   [" << (*i)->getText() << "]" << endl <<  "     IS       [" << (*j)->getText() << "]" << endl;
				}
			}
		}
		else if ((*i)->getType() == InstructionType::I_BLTZ) {
			for (auto j = list.begin(); j != list.end(); j++) {
				if ((*j)->getPosition() == (*i)->getLabel()->getNum()) {
					(*i)->getSucc().push_back(*j);
					(*j)->getPred().push_back(*i);
					//cout << endl << endl << "New succ of   [" << (*i)->getText() << "]" << endl << "     IS       [" << (*j)->getText() << "]" << endl;
				}
			}
		}
		else {
			for (auto j = list.begin(); j != list.end(); j++) {
				if ((*i)->getPosition() + 1 == (*j)->getPosition()) {
					(*i)->getSucc().push_back(*j);
					(*j)->getPred().push_back(*i);
					//cout << endl << endl << "New succ of   [" << (*i)->getText() << "]" << endl << "     IS       [" << (*j)->getText() << "]" << endl;
					break;
				}
			}
		}
	}
	cout << endl << endl;
}