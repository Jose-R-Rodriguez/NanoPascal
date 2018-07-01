#ifndef PARSER_HPP
#define PARSER_HPP
#include "lexer.hpp"
#include "common.hpp"
#include "ast.hpp"
class Parser{
public:
	Parser(Lexer&);
	void Parse();
private:
	Symbol* current_token;
	Lexer& mylexer;
	/*void CheckSequence(){}
	template<typename First, typename ... Symbols>
	void CheckSequence(First& symb, Symbols&... rest);*/
	template<typename ... Symbols>
	NP_List CheckSequence(Symbols&... args);
	template <typename Symbols>
	bool NextIsAnyOfThese(Symbols& symbol){return symbol == *current_token;}
	template<typename First, typename ... Symbols>
	bool NextIsAnyOfThese(First& symb, Symbols&... rest);

	Node_Pointer PrimitiveType();
	Node_Pointer ArrayType();

	void DisplayErr(std::ostringstream& err);
	void GetNextToken(){current_token= &mylexer.ResolveToken();};
	NP_List Start();
	Node_Pointer Variables();
	Node_Pointer Variable_decls();
	Node_Pointer Id_List();
	Node_Pointer DataType();
	Node_Pointer Operations_List();
	Node_Pointer Function_Header();
	Node_Pointer Procedure_Header();
	Node_Pointer Argument_decl();
	Node_Pointer Argument_decl_List();
	Node_Pointer Id_List_B();
	Node_Pointer Statement_List();
	Node_Pointer Statement();
	void Assign();
	Node_Pointer Operation_Call();
	void Left_Value_or_Oper_Call();
	Node_Pointer Expression();
	Node_Pointer Term0();
	Node_Pointer Term1();
	Node_Pointer Term2();
	Node_Pointer Final();
	Node_Pointer Constant();
	Node_Pointer BoolConstant();
	void Block();
	Node_Pointer Assign_or_Oper_Call();
	Node_Pointer Opt_Expr();
	Node_Pointer Opt_Args();
	Node_Pointer Args();
	Node_Pointer Args_List();
	Node_Pointer Expression_List();
	Node_Pointer Expression_B();
	void Left_Value();
};
#endif
