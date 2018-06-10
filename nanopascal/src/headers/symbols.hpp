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
};

namespace Symbols{
	static Symbol Pre_Empty(0, "Empty");
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
				T_CLOSE_PAR(111, "Close Parenthesis"),
				T_COMMA(112, "Comma"),
				T_SEMICOLON(113, "Colon"),
				T_OPEN_CURLY(114, "Open Curly Brackets"),
				T_CLOSE_CURLY(115, "Close Curly Brackets"),
				T_EQUALS(116, "Equals sign"),
				T_OP_MULT(117, "Multiplication Operator"),
				T_OP_DIV(118, "Division Operator"),
				T_OP_MOD(119, "Modular Operator"),
				T_VAR(120, "Var keyword"),
				T_INT_TYPE(121, "Integer keyword"),
				T_CHAR_TYPE(122, "Character keyword"),
				T_BOOL_TYPE(123, "Boolean keyword"),
				T_END_OF_PROG(124, "End of Program"),
				T_OP_AND(125, "Bitwise AND Operator"),
				T_OP_OR(126, "Bitwise OR Operator"),
				T_LESS_THAN(127, "Less Than"),
				T_GREATER_THAN(128, "Greater Than"),
				T_EQUALS_TO(134, "Equals To"),
				T_GT_OR_ET(129, "Greater Than or Equals To"),
				T_LT_OR_ET(130, "Less Than or Equals To"),
				T_OP_SHL(131, "Shift Left"),
				T_OP_SHR(132, "Shift Right"),
				T_OP_XOR(133, "Bitwise XOR Operator"),
				T_OP_NOT(134, "Bitwise NOT Operator"),
				T_Hex_Num(135, "Hexdecimal Num"),
				T_NOT_ET(136, "Not Equals To");
	static NonTerminal
				NT_VAR(256, "placeholder");
	static PreprocessorSymbols
				Pre_If_Def(1, "Preprocessor If Def", Pre_Empty),
				Pre_If_N_Def(2, "Preprocessor If not Def", Pre_Empty),
				Pre_Else(3, "Preprocessor Else", Pre_Empty),
				Pre_End_If(4, "Preprocessor End If", Pre_Empty);
}
#endif
