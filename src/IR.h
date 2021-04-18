#ifndef __IR__
#define __IR__

#include "Types.h"

enum VariableType
{
	MEM_VAR,
	REG_VAR,
	NO_TYPE
};

/**
 * This class represents one variable from program code.
 */
class Variable
{
public:
	Variable() : m_type(NO_TYPE), m_name(""), m_position(-1), m_assignment(no_assign) {}
	Variable(std::string name, VariableType type, int position) : m_type(type), m_name(name), m_position(position), m_assignment(no_assign) {}
	Variable(std::string name, VariableType type) : m_type(type), m_name(name), m_position(0), m_assignment(no_assign) {}
	~Variable() {}
	void setNum(int num) { m_num = num; }
	void setPosition(int pos) { m_position = pos; }
	void setAssignment(Regs reg) { m_assignment = reg; }
	Regs getAssignment() { return m_assignment; }
	std::string getName() { return m_name; }
	int getPosition() { return m_position; }
	VariableType getVarType() { return m_type; }
	int getNum() { return m_num; }
	bool operator<(Variable* rhs) { return this->getName() < rhs->getName(); }
	bool operator==(Variable* rhs) { return this->getName() == rhs->getName(); }
	friend std::ostream& operator<<(std::ostream& os, Variable* variable) {	
		os << variable->getName() << " [" << variable->getVarType() << "] ";
		return os;
	}
	bool operator!=(Variable* var) { return this->getName() != var->getName(); }
private:
	VariableType m_type;
	std::string m_name;
	int m_num;
	int m_position;
	Regs m_assignment;
};

/**
 * This type represents list of variables from program code.
 */
typedef std::list<Variable*> Variables;

/**
* This class represents label from program code.
*/
class Label {
	std::string name;
	int number;
public:
	Label() {}
	Label(std::string labelName, int instructionNum) : name(labelName), number(instructionNum) {}
	std::string getName() { return name; }
	void setName(std::string labelName) { name = labelName; }
	int getNum() { return number; }
	void setNum(int instructionNum) { number = instructionNum; }
};


/**
* This type represents list of labels from program code.
*/
typedef std::list<Label*> Labels;

/**
 * This class represents one instruction in program code.
 */
class Instruction
{
public:
	Instruction() : m_position(0), m_type(I_NO_TYPE) {}
	Instruction(int pos, InstructionType type, Variables& dst, Variables& src) :
		m_position(pos), m_type(type), m_dst(dst), m_src(src) {}
	Instruction(InstructionType type) : m_position(0), m_type(type) {}

	Variables& getDst() { return m_dst; }
	Variables& getSrc() { return m_src; }
	Variables& getUse() { return m_use; }
	Variables& getDef() { return m_def; }
	Variables& getIn() { return m_in; }
	Variables& getOut() { return m_out; }
	std::string getText() { return m_text; }
	int getNumber() { return m_number; }
	Label* getLabel() { return m_label; }
	InstructionType getType() { return m_type; }
	std::list<Instruction*>& getSucc() { return m_succ; }
	std::list<Instruction*>& getPred() { return m_pred; }
	int getPosition() { return m_position; }
	void setDst(Variables& dst) { m_dst = dst; }
	void setSrc(Variables& src) { m_src = src; }
	void setUse(Variables& use) { m_use = use; }
	void setDef(Variables& def) { m_def = def; }
	void setIn(Variables& in) { m_in = in; }
	void setOut(Variables& out) { m_out = out; }
	void addVariable(Variables& whichList, Variable* variable) { whichList.push_back(variable); }
	void setPosition(int pos) { m_position = pos; }
	void setNumber(int num) { m_number = num; }
	void setText(std::string text) { m_text = text; }
	void setLabel(Label* lbl) { m_label = lbl; }
	friend std::ostream& operator<<(std::ostream& os, Instruction* instr);

private:
	int m_position;
	InstructionType m_type;
	std::string m_text;
	int m_number;
	Label* m_label;
	
	Variables m_dst;
	Variables m_src;

	Variables m_use;
	Variables m_def;
	Variables m_in;
	Variables m_out;
	std::list<Instruction*> m_succ;
	std::list<Instruction*> m_pred;
};


/**
 * This type represents list of instructions from program code.
 */
typedef std::list<Instruction*> Instructions;

/**
* This type represents matrix of integers.
*/
typedef std::vector<std::vector<int>> InterferenceMatrix;


/**
* This type represents stack of pointers to variables.
*/
typedef std::stack<Variable*> SimplificationStack;

/**
* This type represents interference graph.
*/
class InterferenceGraph
{
public:
	Variables* variables;		// all variables from instructions
	InterferenceMatrix matrix;	// interference matrix
	InterferenceGraph() {}
};
#endif
