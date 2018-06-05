#ifndef LEXER_HPP
#define LEXER_HPP
#include <fstream>
#include <map>
#include <string>
#include "symbols.hpp"

class Lexer{
public:
	Lexer(std::ifstream& input);
	Symbol& ResolveToken();

private:
	char GetNextChar();
	char current_char;
	int current_column, current_row;
	std::ifstream& input;
	std::string lexeme;
	std::map <std::string, Symbol> Keywords{
		{"program", Symbols::T_PROG},
		{"begin", Symbols::T_BEGIN},
		{"end", Symbols::T_END}
	};
};
#endif
