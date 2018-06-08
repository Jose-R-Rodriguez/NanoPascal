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
	bool PeekAndCompare(char t);
	void ConsumeSequence(std::function <bool(char)> func, bool);
	char current_char;
	int current_column, current_row;
	std::ifstream& input;
	std::string lexeme;
	std::map <std::string, Symbol> Keywords{
		{"program", Symbols::T_PROG},
		{"begin", Symbols::T_BEGIN},
		{"end", Symbols::T_END},
		{"writeln", Symbols::T_WRTLN},
		{"div", Symbols::T_OP_DIV},
		{"mod", Symbols::T_OP_MOD},
		{"var", Symbols::T_VAR},
		{"integer", Symbols::T_INT_TYPE},
		{"char", Symbols::T_CHAR_TYPE},
		{"boolean", Symbols::T_BOOL_TYPE},
		{"not", Symbols::T_OP_NOT},
		{"and", Symbols::T_OP_AND},
		{"or", Symbols::T_OP_OR},
		{"xor", Symbols::T_OP_XOR},
		{"shl", Symbols::T_OP_SHL},
		{"shr", Symbols::T_OP_SHR}
	};
};
#endif
