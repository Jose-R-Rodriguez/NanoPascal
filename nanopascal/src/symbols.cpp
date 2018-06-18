#include <iomanip>
#include "symbols.hpp"
#include <iostream>
Symbol::Symbol(int id, std::string name){
	this->id= id;
	this->name= name;
}

std::ostream& operator<<(std::ostream& os, const Symbol& symb){
	os<<symb.id<<std::setw(25)<<symb.name;
	return os;
}
PreprocessorSymbols::PreprocessorSymbols(int id, std::string name, Symbol& symb):Symbol(id, name), els(symb){
}
namespace Symbols{
	Symbol Pre_Empty(0, "Empty");
	Terminal
				T_STR_LIT(104, "String Literal"),
				T_EOF(254, "End of File"),
				T_EOE(253, "End of Expression"),
				T_PROG(101, "Program Keyword"),
				T_BEGIN(102, "Begin Keyword"),
				T_END(103, "End Keyword"),
				T_OP_ADD(105, "Operator Add"),
				T_OP_SUB(106, "Operator Subtract"),
				T_EPSILON(1, "Epsilon"),
				T_ID(107, "Identifier"),
				T_NUM(108, "Number"),
				T_WRTLN(109, "Writeln Keyword"),
				T_OPEN_PAR(110, "Open Parenthesis"),
				T_CLOSE_PAR(111, "Close Parenthesis"),
				T_COMMA(112, "Comma"),
				T_COLON(113, "Colon"),
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
				T_HEX_NUM(135, "Hexdecimal Num"),
				T_NOT_ET(136, "Not Equals To"),
				T_BINARY_NUM(137, "Binary Number"),
				T_IF(138, "If Keyword"),
				T_BREAK(139, "Break Keyword"),
				T_ELSE(140, "Else Keyword"),
				T_UNTIL(141, "Until Keyword"),
				T_FOR(142, "For Keyword"),
				T_DO(143, "Do Keyword"),
				T_FUNCTION(144, "Function Keyword"),
				T_PROCEDURE(145, "Procedure Keyword"),
				T_READ(146, "Read Keyword"),
				T_TRUE(147, "True Keyword"),
				T_FALSE(148, "False Keyword"),
				T_OF(149, "Of Keyword"),
				T_ARRAY(150, "Array Keyword"),
				T_WHILE(151, "While Keyword"),
				T_THEN(152, "Then Keyword"),
				T_WRITE(153, "Write Keyword"),
				T_REPEAT(154, "Repeat Keyword"),
				T_TO(155, "To Keyword"),T_DOT_SET(158, "Dot Set"),
				T_OPEN_BRACK(156, "Open Bracket"), T_CLOSE_BRACK(157, "Close Bracket"),
				T_CONTINUE(158, "Continue Keyword");
	NonTerminal
				NT_VAR(256, "placeholder");
	PreprocessorSymbols
				Pre_If_Def(1, "Preprocessor If Def", Pre_Empty),
				Pre_If_N_Def(2, "Preprocessor If not Def", Pre_Empty),
				Pre_Else(3, "Preprocessor Else", Pre_Empty),
				Pre_End_If(4, "Preprocessor End If", Pre_Empty);
	void PrintLitAddress(){
		std::cout<<"Address of lit in symbols"<<&T_STR_LIT<<std::endl;
	}
}
