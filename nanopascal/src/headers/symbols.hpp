#ifndef SYMBOLS_HPP
#define SYMBOLS_HPP
#include <string>
class Symbol{
public:
	Symbol(int, std::string);
	int id;
	std::string name;
	std::string current_lexeme;
	Symbol& SetSymbolLexeme(std::string);
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
				T_PROG(101, "Program"),
				T_BEGIN(102, "Begin"),
				T_END(103, "End"),
				T_STR_LIT(104, "String Literal"),
				T_OP_ADD(105, "Operator Add"),
				T_OP_SUB(106, "Operator Subtract"),
				T_EPSILON(1, "Epsilon");
	static NonTerminal
				NT_VAR(256, "Variable"),
				NT_ID(257, "Identifier"),
				NT_WRTLN_STMNT(258, "Writeln Statement");
}
#endif
