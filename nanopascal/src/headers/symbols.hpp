#ifndef SYMBOLS_HPP
#define SYMBOLS_HPP
#include "common.hpp"

class Symbol{
public:
	friend std::ostream& operator<<(std::ostream& os, const Symbol& symb);
	Symbol(int, std::string);
	int id;
	std::string name;
};

class Terminal: public Symbol{
public:
	Terminal(int id, std::string name):Symbol(id, name){};
};

class NonTerminal: public Symbol{
public:
	NonTerminal(int id, std::string name):Symbol(id, name){};
};

class PreprocessorSymbols: public Symbol{
public:
	Symbol& els;
	PreprocessorSymbols(int, std::string, Symbol&);
	int x= 44;
};

namespace Symbols{
	extern Symbol Pre_Empty;
	extern Terminal
				T_EOF,
				T_EOE,
				T_PROG,
				T_BEGIN,
				T_END,
				T_STR_LIT,
				T_OP_ADD,
				T_OP_SUB,
				T_EPSILON,
				T_ID,
				T_NUM,
				T_WRTLN,
				T_OPEN_PAR,
				T_CLOSE_PAR,
				T_COMMA,
				T_SEMICOLON,
				T_OPEN_CURLY,
				T_CLOSE_CURLY,
				T_EQUALS,
				T_OP_MULT,
				T_OP_DIV,
				T_OP_MOD,
				T_VAR,
				T_INT_TYPE,
				T_CHAR_TYPE,
				T_BOOL_TYPE,
				T_END_OF_PROG,
				T_OP_AND,
				T_OP_OR,
				T_LESS_THAN,
				T_GREATER_THAN,
				T_EQUALS_TO,
				T_GT_OR_ET,
				T_LT_OR_ET,
				T_OP_SHL,
				T_OP_SHR,
				T_OP_XOR,
				T_OP_NOT,
				T_HEX_NUM,
				T_NOT_ET,
				T_BINARY_NUM,
				T_IF,
				T_BREAK,
				T_ELSE,
				T_UNTIL,
				T_FOR,
				T_DO,
				T_FUNCTION,
				T_PROCEDURE,
				T_READ,
				T_TRUE,
				T_FALSE,
				T_OF,
				T_ARRAY,
				T_WHILE,
				T_THEN,
				T_WRITE,
				T_REPEAT,
				T_TO;
	extern NonTerminal
				NT_VAR;
	extern PreprocessorSymbols
				Pre_If_Def,
				Pre_If_N_Def,
				Pre_Else,
				Pre_End_If;
	extern void PrintLitAddress();
}
#endif
