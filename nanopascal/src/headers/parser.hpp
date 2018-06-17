#ifndef PARSER_HPP
#define PARSER_HPP
#include "lexer.hpp"
#include "common.hpp"
class Parser{
public:
	Parser(Lexer&);
	void Parse();
private:
	Lexer& mylexer;
	void Start();
	void Variables();
	void Variable_decls();
	void Id_List();
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
