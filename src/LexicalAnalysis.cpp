#include <iostream>
#include <iomanip>
#include <fstream>

#include "Constants.h"
#include "LexicalAnalysis.h"
#include "Token.h"

using namespace std;


void LexicalAnalysis::initialize()
{
	programBufferPosition = 0;
	fsm.initStateMachine();
}


bool LexicalAnalysis::Do()
{
	while (true)
	{
		Token token = getNextTokenLex();
		switch (token.getType())
		{
			case T_ERROR:
				errorToken = token;
				tokenList.push_back(token);
				return false;
			case T_END_OF_FILE:
				tokenList.push_back(token);
				return true;
			case T_WHITE_SPACE:
				continue;
			default:
				tokenList.push_back(token);
				break;
		}
	}
}


bool LexicalAnalysis::readInputFile(string fileName)
{
	inputFile.open(fileName, ios_base::binary);

	if (!inputFile)
		return false;
	
	inputFile.seekg(0, inputFile.end);
	int length = (int)inputFile.tellg();
	inputFile.seekg (0, inputFile.beg);
	programBuffer.resize(length);
	if (length > 0)
		inputFile.read(&programBuffer.front(), length);
	inputFile.close();
	return true;
}


Token LexicalAnalysis::getNextTokenLex()
{
	int currentState = START_STATE;
	int nextState = 0;
	int lastFiniteState = 0;

	// position in stream
	int counter = 0;
	int lastLetterPos = programBufferPosition;

	Token token;

	while (true)
	{
		char letter;
		unsigned int letterIndex = programBufferPosition + counter;

		if (letterIndex < programBuffer.size())
		{
			letter = programBuffer[letterIndex];
		}
		else
		{
			// we have reached the end of input file, force the search letter to invalid value
			letter = -1;
			if (programBufferPosition >= programBuffer.size())
			{
				// if we have reached end of file and printed out the last correct token
				// create EOF token and exit
				token.makeEofToken();
				return token;
			}
		}
		
		nextState = this->fsm.getNextState(currentState, letter);
		counter ++;

		if (nextState > IDLE_STATE)
		{
			// change the current state
			currentState = nextState;

			if (nextState == START_STATE)
				throw runtime_error("\nException: Infinite state detected! There is something very wrong with the code !\n");

			// remember last finite state
			lastFiniteState = nextState;
			lastLetterPos = programBufferPosition + counter;
		}
		else if (nextState == INVALID_STATE)
		{
			// eof character read, generate token defined with last finite state
			if (lastFiniteState != IDLE_STATE)
			{
				// token recognized, make token
				token.makeToken(programBufferPosition, lastLetterPos, programBuffer, lastFiniteState);
				programBufferPosition = lastLetterPos;
				return token;
			}
			else
			{
				// error occurred, create error token
				token.makeErrorToken(programBufferPosition + counter - 1, programBuffer);
				programBufferPosition = programBufferPosition + counter - 1;
				return token;
			}
		}
		else
		{
			// final state reached, state machine is in IDLE state
			// calculate the number of characters needed for the recognized token
			int len = lastLetterPos - programBufferPosition;

			// create the token
			if (len > 0)
			{
				// token recognized, make token
				token.makeToken(programBufferPosition, lastLetterPos, programBuffer, lastFiniteState);
				programBufferPosition = lastLetterPos;
				return token;
			}
			else
			{
				// error occurred, create error token
				token.makeErrorToken(programBufferPosition + counter - 1, programBuffer);
				programBufferPosition = programBufferPosition + counter - 1;
				return token;
			}
		}
	}

	return token;
}


TokenList& LexicalAnalysis::getTokenList()
{
	return tokenList;
}


void LexicalAnalysis::printTokens(std::ostream& out)
{
	if (tokenList.empty())
	{
		out << "Token list is empty!" << endl;
	}
	else
	{
		printMessageHeader(out);
		TokenList::iterator it = tokenList.begin();
		for (; it != tokenList.end(); it++)
		{
			(*it).printTokenInfo(out);
		}
	}
}


void LexicalAnalysis::printLexError(std::ostream& out)
{
	if (errorToken.getType() != T_NO_TYPE)
	{
		printMessageHeader(out);
		errorToken.printTokenInfo(out);
	}
	else
	{
		out << "There are no lexical errors!" << endl;
	}
}

void LexicalAnalysis::printMessageHeader(std::ostream& out)
{
	out << setw(LEFT_ALIGN) << left << "Type:";
	out << setw(RIGHT_ALIGN) << right << "Value:" << endl;
	out << setfill('-') << setw(LEFT_ALIGN + RIGHT_ALIGN + 1) << " " << endl;
	out << setfill(' ');
}
