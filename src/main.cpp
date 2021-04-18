#include "main.h"

int main()
{
	std::string chosenFile = getFilePath();
	std::string chosenFilePath = inFilePath + chosenFile;
	size_t lastindex = chosenFile.find_last_of(".");
	chosenFile = chosenFile.substr(0, lastindex);
	consoleOutFile += chosenFile + "_console_print" + ".txt";
	
	try
	{
		bool retVal = false;


		LexicalAnalysis lex;
		if (!lex.readInputFile(chosenFilePath))
			throw std::runtime_error("\nException! Failed to open input file!\n");
		lex.initialize();
		retVal = lex.Do();
		if (retVal)
		{
			consolePrint.open(consoleOutFile);
			consolePrint << "Lexical analysis :" << std::endl << std::string(15, '-') << std::endl;
			std::cout << std::string(15,'-') << std::endl <<  "Lexical analysis finished successfully!" << std::endl;
			//lex.printTokens();
			lex.printTokens(consolePrint);
			consolePrint << std::endl << std::endl << std::endl;
			consolePrint.close();
		}
		else
		{
			lex.printLexError(std::cout);
			throw std::runtime_error("\nException! Lexical analysis failed!\n");
		}


		SyntaxAnalysis syntax(lex);
		retVal = syntax.Do();
		if (retVal) {
			consolePrint.open(consoleOutFile, std::ios_base::app);
			consolePrint << "Syntax analysis :" << std::endl << std::string(15, '-') << std::endl;
			std::cout << std::string(15, '-') << std::endl << "Syntax analysis finished successfully!" << std::endl;
			printInstructions(consolePrint, syntax.getInstructions());
			consolePrint << std::endl << std::endl << std::endl;
			consolePrint.close();
		}
		else
			throw std::runtime_error("Syntax analysis failed!");
		//printInstructions(syntax.getInstructions());


		livenessAnalysis(syntax.getInstructions());
		std::cout << std::string(15, '-') << std::endl << "Liveness analysis finished successfully!" << std::endl;
		consolePrint.open(consoleOutFile, std::ios_base::app);
		consolePrint << "Liveness analysis :" << std::endl << std::string(15, '-') << std::endl;
		printInstructions(consolePrint, syntax.getInstructions());
		consolePrint << std::endl << std::endl << std::endl;
		consolePrint.close();
		//printInstructions(syntax.getInstructions());
		

		InterferenceGraph* ig = buildInterferenceGraph(syntax.getInstructions(), syntax.getVariables());
		std::cout << std::string(15, '-') << std::endl << "Interference graph build succeeded!" << std::endl;
		//printInterferenceMatrix(*ig);
		consolePrint.open(consoleOutFile, std::ios_base::app);
		consolePrint << "Interference matrix :" << std::endl << std::string(15, '-') << std::endl;
		printInterferenceMatrix(consolePrint, *ig);
		consolePrint << std::endl << std::endl << std::endl;
		consolePrint.close();


		SimplificationStack* stack = doSimplification(ig, __REG_NUMBER__);
		bool success = doResourceAllocation(stack, ig);
		if (success) {
			std::cout << std::string(15, '-') << std::endl << "Allocation successful!" << std::endl;
			consolePrint.open(consoleOutFile, std::ios_base::app);
			consolePrint << "Resource allocation :" << std::endl << std::string(15, '-') << std::endl;
			printInstructions(consolePrint, syntax.getInstructions());
			consolePrint << std::endl << std::endl << std::endl;
			consolePrint.close();
		}
		else 
			throw std::runtime_error("Allocation failed! Not enough registers!");
		//printInstructions(syntax.getInstructions());

		toMips(syntax, chosenFile);
		std::cout << std::string(15, '-') << std::endl << "MIPS file writen successfully!" << std::endl << std::string(15, '-') << std::endl << std::endl;

	}
	catch (std::runtime_error e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
	
	return 0;
}
