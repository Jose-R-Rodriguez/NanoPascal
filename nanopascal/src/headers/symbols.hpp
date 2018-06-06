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
namespace Symbols{
	static Terminal
				T_EOF(254, "End of File"),
				T_EOE(253, "End of Expression"),
				T_PROG(101, "Program Keyword"),
				T_BEGIN(102, "Begin Keyword"),
				T_END(103, "End Keyword"),
				T_STR_LIT(104, "String Literal"),
				T_OP_ADD(105, "Operator Add"),
				T_OP_SUB(106, "Operator Subtract"),
				T_EPSILON(1, "Epsilon"),
				T_ID(107, "Identifier"),
				T_NUM(108, "Number"),
				T_WRTLN(109, "Writeln Keyword"),
				T_OPEN_PAR(110, "Open Parenthesis"),
				T_CLOSE_PAR(111, "Close Parenthesis");
	static NonTerminal
				NT_VAR(256, "Variable");
}
#endif
