#include "IR.h"

std::ostream & operator<<(std::ostream & os, Instruction * instr)
{
	InstructionType type = instr->getType();
	if (type == I_ADD) {
		os << "add $t" << instr->getDst().front()->getAssignment();
		for (auto var : instr->getSrc())
			os << ", $t" << var->getAssignment();
	}
	else if (type == I_ADDI) {
		os << "addi $t" << instr->getDst().front()->getAssignment();
		os << ", $t" << instr->getSrc().front()->getAssignment();
		os << ", " << instr->getNumber();
	}
	else if (type == I_B) {
		os << "b " << instr->getLabel()->getName();
	}
	else if (type == I_BLTZ) {
		os << "bltz $t" << instr->getSrc().front()->getAssignment();
		os << ", " << instr->getLabel()->getName();
	}
	else if (type == I_LA) {
		os << "la $t" << instr->getDst().front()->getAssignment();
		os << ", " << instr->getSrc().front()->getName();
	}
	else if (type == I_LI) {
		os << "li $t" << instr->getDst().front()->getAssignment();
		os << ", " << instr->getNumber();
	}
	else if (type == I_LW) {
		os << "lw $t" << instr->getDst().front()->getAssignment();
		os << ", " << instr->getNumber() << "($t" << instr->getSrc().front()->getAssignment() << ")";
	}
	else if (type == I_NOP) {
		os << "nop";
	}
	else if (type == I_SUB) {
		os << "sub $t" << instr->getDst().front()->getAssignment();
		for (auto var : instr->getSrc())
			os << ", $t" << var->getAssignment();
	}
	else if (type == I_SW) {
		os << "sw $t" << instr->getDst().front()->getAssignment();
		os << ", " << instr->getNumber() << "($t" << instr->getSrc().front()->getAssignment() << ")";
	}
	else if (type == I_AND) {
		os << "and $t" << instr->getDst().front()->getAssignment();
		for (auto var : instr->getSrc())
			os << ", $t" << var->getAssignment();
	}
	else if (type == I_OR) {
		os << "or $t" << instr->getDst().front()->getAssignment();
		for (auto var : instr->getSrc())
			os << ", $t" << var->getAssignment();
	}
	else if (type == I_LB) {
		os << "lb $t" << instr->getDst().front()->getAssignment();
		os << ", " << instr->getNumber() << "($t" << instr->getSrc().front()->getAssignment() << ")";
	}
	return os;
}
