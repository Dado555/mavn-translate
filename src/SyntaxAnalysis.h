#pragma once

#include "LexicalAnalysis.h"
#include "Token.h"
#include "IR.h"

class SyntaxAnalysis
{
public:

	/**
	* Constructor
	*/
	SyntaxAnalysis(LexicalAnalysis& lex);

	/**
	* Method which performs syntax analysis
	*/
	bool Do();

	/**
	* Method which returns instructions list
	*/
	Instructions& getInstructions() { return instructionList; }

	/**
	* Method which returns variable list
	*/
	Variables& getVariables() { return variableList; }

	/**
	* Method which returns functions list
	*/
	Labels&  getFunctions() { return functionList; }

	/**
	* Method which returns labels list
	*/
	Labels& getLabels() { return labelList; }

	/**
	* Container for labels that weren't created yet, but used in branch instruction.
	*/
	std::vector<std::string> neededLabels;

	/**
	* Container for instructions on in front of which are branch labels from neededLabels.
	*/
	std::vector<Instruction*> tempInstructions;

	/**
	* Method which assign branch label to function when label is recognized. 
	*/
	void findAndDelete(std::vector<std::string>& vector, std::string& val);

private:
	/**
	* Prints the error message, and token that caused the syntax error
	*/
	void printSyntaxError(Token token);

	/**
	* Prints the token info
	*/
	void printTokenInfo(Token token);

	/**
	* Eats the current token if its type is "t"
	* otherwise reports syntax error
	*
	* param[in] - t - the expected token type
	*/
	void eat(TokenType t);

	/**
	* Returns the next token from the token list
	*/
	Token getNextToken();

	/**
	* Nonterminal Q
	*/
	void Q();

	/**
	* Nonterminal S
	*/
	void S();

	/**
	* Nonterminal L
	*/
	void L();

	/**
	* Nonterminal E
	*/
	void E();

	/**
	* Reference to lexical analysis module
	*/
	LexicalAnalysis& lexicalAnalysis;

	/**
	* Syntax error indicator
	*/
	bool errorFound;

	/**
	* Iterator to the token list which represents the output of the lexical analysis
	*/
	TokenList::iterator tokenIterator;

	/**
	* Current token that is being analyzed
	*/
	Token currentToken;

	/**
	* Fill m_dst, m_src, m_use, m_def with variables found in instruction
	*/
	void fillVariableLists(Instruction* instruction, VariableType type, bool dstOrSrc);

	/**
	* Find variable from variableList by name
	*/
	Variable* findVariable(std::string name);

	/**
	* Check is variable in variableList
	*/
	bool variableInList(std::string name, VariableType type);

	/**
	* Check is function name correct
	*/
	bool funcNameCorrect(std::string name);

	/**
	* Check is label in labelList
	*/
	bool labelInList(Label* label);

	/**
	* Check is function in functionList
	*/
	bool functionInList(std::string label);

	/**
	* Assign label to instruction ( result of branch instruction: b label )
	*/
	bool SyntaxAnalysis::branchInstructionLabel(Instruction* instruction);

	/**
	* Fill m_succ and m_pred of every instruction in instructionList
	*/
	void fillSuccAndPred(Instructions& list);

	/**
	* Variables recognized by syntax analysis, more specific, by method S().
	*/
	Variables variableList;

	/**
	* Registry variables counter.
	*/
	int registryNum;

	/**
	* Instruction counter.
	*/
	int instructionNum;

	/**
	* All labels recognized, including function labels.
	*/
	Labels labelList;

	/**
	* All instructions recognized by analysis, in method E().
	*/
	Instructions instructionList;

	/**
	* Only function labels.
	*/
	Labels functionList;
};