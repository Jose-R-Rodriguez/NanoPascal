#ifndef SYMBOLS_HPP
#define SYMBOLS_HPP
#include <string>
class Symbol{
public:
	Symbol(int id, std::string name);
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
	static const Terminal
				T_EOF(100, "End of File"),
				T_EOE(101, "End of Expression"),
				T_PROG(102, "Program");
	static const NonTerminal
				NT_VAR(256, "Variable")
				;
}
#endif
