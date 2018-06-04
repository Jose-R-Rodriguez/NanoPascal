#ifndef LEXER_HPP
#define LEXER_HPP
#include <fstream>
#include "symbols.hpp"
class Lexer{
public:
	char current_char;
	Lexer(std::ifstream& input);
	Symbol ResolveToken();

private:
	int current_column, current_row;
	std::ifstream& input;
	char GetNextChar();
};
#endif
