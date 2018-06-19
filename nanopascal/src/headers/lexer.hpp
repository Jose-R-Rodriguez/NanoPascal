#ifndef LEXER_HPP
#define LEXER_HPP
#include <fstream>
#include <map>
#include <stack>
#include <set>
#include <tuple>
#include <functional>
#include "common.hpp"

class Lexer{
public:
	using directive_structure= std::tuple<std::string, bool, bool>;
	Lexer(std::ifstream& input);
	Symbol& ResolveToken();
	std::string GetCurrentLexeme();
	void AddDirective(std::string);
	int GetCurrentRow(){return current_row;};
	int GetCurrentColumn(){return current_column;};

private:
	char GetNextChar(bool);
	bool PeekAndCompare(char t);
	void ConsumeSequence(std::function <bool(char)> func, bool);
	Symbol& EvaluateDirective();
	Symbol& ProcessDirective();
	void PrintActiveDirectives();
	Symbol& DoIfDef();
	Symbol& DoElse();
	Symbol& DoEndIf();
	char current_char;
	int current_column, current_row;
	std::ifstream& input;
	std::string lexeme;
	std::stack<directive_structure> active_directives;
	std::set<std::string> declared_directives{
		{"nanopascal"},
		{"random"}
	};
	std::map <std::string, Symbol&> Keywords{
		{"program", 	Symbols::T_PROG},
		{"begin", 		Symbols::T_BEGIN},
		{"end", 			Symbols::T_END},
		{"writeln", 	Symbols::T_WRTLN},
		{"div", 			Symbols::T_OP_DIV},
		{"mod", 			Symbols::T_OP_MOD},
		{"var", 			Symbols::T_VAR},
		{"integer", 	Symbols::T_INT_TYPE},
		{"char", 			Symbols::T_CHAR_TYPE},
		{"boolean", 	Symbols::T_BOOL_TYPE},
		{"not", 			Symbols::T_OP_NOT},
		{"and", 			Symbols::T_OP_AND},
		{"or", 				Symbols::T_OP_OR},
		{"xor", 			Symbols::T_OP_XOR},
		{"shl", 			Symbols::T_OP_SHL},
		{"shr", 			Symbols::T_OP_SHR},
		{"{$ifdef ", 	Symbols::Pre_If_Def},
		{"{$ifndef ",	Symbols::Pre_If_N_Def},
		{"{$else}", 	Symbols::Pre_Else},
		{"{$endif}", 	Symbols::Pre_End_If},
		{"true", 			Symbols::T_TRUE},
		{"false",			Symbols::T_FALSE},
		{"read",			Symbols::T_READ},
		{"procedure",	Symbols::T_PROCEDURE},
		{"function",	Symbols::T_FUNCTION},
		{"if",				Symbols::T_IF},
		{"else",			Symbols::T_ELSE},
		{"while",			Symbols::T_WHILE},
		{"until",			Symbols::T_UNTIL},
		{"then",			Symbols::T_THEN},
		{"array",			Symbols::T_ARRAY},
		{"for",				Symbols::T_FOR},
		{"do",				Symbols::T_DO},
		{"write",			Symbols::T_WRITE},
		{"repeat",		Symbols::T_REPEAT},
		{"break",			Symbols::T_BREAK},
		{"to",				Symbols::T_TO},
		{"of",				Symbols::T_OF},
		{"continue",	Symbols::T_CONTINUE}
	};
};
#endif
