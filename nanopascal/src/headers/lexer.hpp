#ifndef LEXER_HPP
#define LEXER_HPP
#include <fstream>
#include <map>
#include <functional>
#include "common.hpp"
#include "symbols.hpp"

class Lexer{
public:
	Lexer(std::ifstream& input);
	Symbol& ResolveToken();
	std::string GetCurrentLexeme();

private:
	char GetNextChar();
	void ConsumeSequence(std::function <bool(char)> func, bool);
	char current_char;
	int current_column, current_row;
	std::ifstream& input;
	std::string lexeme;
	std::map <std::string, Symbol> Keywords{
		{"program", Symbols::T_PROG},
		{"begin", Symbols::T_BEGIN},
		{"end", Symbols::T_END},
		{"writeln", Symbols::T_WRTLN}
	};
};
#endif
