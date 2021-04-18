#pragma once

#include <iostream>
#include <exception>

#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"
#include "LivenessAnalysis.h"
#include "InterferenceGraph.h"
#include "Simplification.h" 
#include "ResourceAllocation.h"
#include "dirent.h"
#include <algorithm>

static std::string outFilePath = "../examples/outFiles/";
static std::string inFilePath = "../examples/";

std::string consoleOutFile = outFilePath;
std::ofstream consolePrint;

void printVars(std::ostream& out, Variables& vars) {
	for (Variable* var : vars) {
		out << var;
	}
}

void printInstructions(std::ostream& out, Instructions& instruc)
{
	for (auto instruction : instruc) {
		out << "Instruction (type:" << instruction->getType() << ", position:"<<instruction->getPosition() << ")"<< std::endl;
		out << "m_src: "; 
		printVars(out, instruction->getSrc());
		out << std::endl << "m_dst: ";
		printVars(out, instruction->getDst());
		out << std::endl << "m_use: ";
		printVars(out, instruction->getUse());
		out << std::endl << "m_def: ";
		printVars(out, instruction->getDef());
		out << std::endl << "m_in: ";
		printVars(out, instruction->getIn());
		out << std::endl << "m_out: ";
		printVars(out, instruction->getOut());
		out << std::endl;
	}
}

void toMips(SyntaxAnalysis& syntax, std::string outFile) {
	std::string resultFile = outFilePath + outFile + "_result" + ".s";
	std::ofstream outStream(resultFile);
	for (auto function : syntax.getFunctions())
		outStream << ".globl " << function->getName() << std::endl;
	outStream << std::endl << ".data" << std::endl;
	for (auto variable : syntax.getVariables()) {
		if (variable->getVarType() == MEM_VAR) {
			outStream << variable->getName() << ": " << ".word " << variable->getNum() << std::endl;
		}
	}
	outStream << std::endl << ".text" << std::endl;
	for (auto instruction : syntax.getInstructions()) {
		for (auto label : syntax.getLabels()) {
			if (label->getNum() == instruction->getPosition()) {
				outStream << label->getName() << ":" << std::endl;
			}
		}
		outStream << "\t" << instruction << std::endl;
	}
	/*
	li $v0, 10
	syscall
	*/
	outStream << "\t" << "li $v0, 10" << std::endl;
	outStream << "\t" << "syscall";
	outStream.close();
}

bool hasSuffix(const std::string& s, const std::string& suffix)
{
	return (s.size() >= suffix.size()) && equal(suffix.rbegin(), suffix.rend(), s.rbegin());
}

void readDirectory(const std::string& name, std::vector<std::string>& v)
{
	DIR* dirp = opendir(name.c_str());
	struct dirent * dp;
	std::string temp;
	short i = 1;
	while ((dp = readdir(dirp)) != NULL) {
		temp = dp->d_name;
		if (hasSuffix(temp, ".mavn")) {
			v.push_back(temp);
			i++;
		}
	}
	closedir(dirp);
}


std::string getFilePath() {
	std::cout << "---------------------------------------------------------------------" << std::endl
			  << "                      Choose input file (number) :                   " << std::endl
			  << "---------------------------------------------------------------------" << std::endl << std::endl;
	std::vector<std::string> vec;
	readDirectory(inFilePath, vec);
	std::sort(vec.begin(), vec.end(), [](const std::string& first, const std::string& second) { return first.size() < second.size(); });
	short cnt = 1;
	for (std::string name : vec) {
		std::cout << cnt << ". " << name << std::endl;
		cnt++;
	}

	short input = 1;
	std::cout << " >> ";
	while (!(std::cin >> input) || input > vec.size() || input < 1)
	{
		std::cin.clear();
		std::cin.ignore();
		std::cout << "Wrong input!  Try again >> ";
	}
	std::string choice = vec[input - 1];
	std::cout << "Reading : " << choice << std::endl;
	std::cout << "     ...     " << std::endl;
	Sleep(500);
	std::cout << "     ...     " << std::endl;
	Sleep(500);
	std::cout << "     ...     " << std::endl;
	return choice;
}