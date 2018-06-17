#ifndef PARSER_HPP
#define PARSER_HPP
#include "lexer.hpp"
#include "common.hpp"
class Parser{
public:
	Parser(Lexer&);
	void Parse();
private:
	Symbol* current_token;
	Lexer& mylexer;
	void CheckSequence(){}
	template<typename First, typename ... Symbols>
	void CheckSequence(First& symb, Symbols&... rest);

	template <typename Symbols>
	bool NextIsAnyOfThese(Symbols& symbol){return symbol == *current_token;}
	template<typename First, typename ... Symbols>
	bool NextIsAnyOfThese(First& symb, Symbols&... rest);

	bool PrimitiveType();
	bool ArrayType();

	void DisplayErr(std::ostringstream& err);
	void GetNextToken(){current_token= &mylexer.ResolveToken();};
	void Start();
	void Variables();
	void Variable_decls();
	void Id_List();
	void DataType();
	void Operations_List();
	void Function_Header();
	void Procedure_Header();
	void Argument_decl();
	void Argument_decl_List();
	void Id_List_B();
	void Statement_List();
	void Assign();
	void Operation_Call();
};
#endif
