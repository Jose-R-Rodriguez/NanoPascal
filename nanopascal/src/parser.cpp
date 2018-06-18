#include "parser.hpp"
Parser::Parser(Lexer& mylexer) : mylexer(mylexer), current_token(&mylexer.ResolveToken()){
	Parse();
}

template<typename First, typename ... Symbols>
void Parser::CheckSequence(First& symb, Symbols&... rest){
	if (symb != *current_token){
		err<<"Expected:"<<std::endl<<symb<<std::endl;
		DisplayErr(err);
	}
	GetNextToken();
	CheckSequence(rest...);
}

template<typename First, typename ... Symbols>
bool Parser::NextIsAnyOfThese(First& symb, Symbols&... rest){
	return (symb==*current_token) || NextIsAnyOfThese(rest...);
}

void Parser::Start(){
	CheckSequence(Symbols::T_PROG, Symbols::T_ID, Symbols::T_EOE);
	Variables();
	Operations_List();
}

void Parser::Operations_List(){
	while (NextIsAnyOfThese(Symbols::T_FUNCTION, Symbols::T_PROCEDURE)){
		//if its a function then get next token and call function header otherwise get token and call procedure header
		(*current_token==Symbols::T_FUNCTION) ?
		(GetNextToken(), Function_Header()) : (GetNextToken(), Procedure_Header());
		Variables();
		CheckSequence(Symbols::T_BEGIN);
		Statement_List();
		CheckSequence(Symbols::T_END, Symbols::T_EOE);
	}
}

void Parser::Statement_List(){
	while (NextIsAnyOfThese(Symbols::T_ID, Symbols::T_IF, Symbols::T_WHILE, Symbols::T_REPEAT, Symbols::T_FOR, Symbols::T_BREAK, Symbols::T_CONTINUE)){
		Statement();
	}
}

void Parser::Statement(){
	if (NextIsAnyOfThese(Symbols::T_ID, Symbols::T_WRITE, Symbols::T_WRTLN, Symbols::T_READ)){
		Assign_or_Oper_Call();
		CheckSequence(Symbols::T_EOE);
	}
	else if (*current_token == Symbols::T_IF){
		GetNextToken();
		Expression();
		CheckSequence(Symbols::T_THEN);
		Block();
		if (*current_token == Symbols::T_OPEN_PAR){
			GetNextToken();
			CheckSequence(Symbols::T_ELSE);
			Block();
			CheckSequence(Symbols::T_CLOSE_PAR);
		}
	}
	else if (*current_token == Symbols::T_WHILE){
		GetNextToken();
	}
	else if (*current_token == Symbols::T_REPEAT){
		GetNextToken();
	}
	else if (*current_token == Symbols::T_FOR){
		GetNextToken();
	}
	else if (*current_token == Symbols::T_BREAK){
		GetNextToken();
	}
	else if (*current_token == Symbols::T_CONTINUE){
		GetNextToken();
	}
	else{
		err<<"Expected Assignment or statment call ";
		DisplayErr(err);
	}
}

void Parser::Left_Value_or_Oper_Call(){
	if (*current_token == Symbols::T_ID){
		GetNextToken();
		if (*current_token == Symbols::T_OPEN_BRACK){
			GetNextToken();
			Expression();
			CheckSequence(Symbols::T_CLOSE_BRACK);
		}
		else if (*current_token == Symbols::T_OPEN_PAR){
			std::cout<<"Found a function call"<<std::endl;

		}
	}
	else{
		err<<"Expected Identifier ";
		DisplayErr(err);
	}
}

void Parser::Expression(){
	Term0();
	Tier4_Loop();
}
void Parser::Term0(){
	Term1();
	Tier3_Loop();
}
void Parser::Term1(){
	Term2();
	Tier2_Loop();
}

void Parser::Term2(){
	Final();
	Tier1_Loop();
}

void Parser::Assign_or_Oper_Call(){
	if (*current_token == Symbols::T_ID){
		GetNextToken();
		if (*current_token == Symbols::T_EQUALS){
			GetNextToken();
			Expression();
		}
		else if(*current_token == Symbols::T_OPEN_BRACK){
			GetNextToken();
			Expression();
			CheckSequence(Symbols::T_CLOSE_BRACK, Symbols::T_EQUALS);
			Expression();
		}
		else if (*current_token == Symbols::T_OPEN_PAR){
			Opt_Expr();
		}
	}
	else{
		Operation_Call();
	}
}

void Parser::Opt_Expr(){
	CheckSequence(Symbols::T_OPEN_PAR);
	if (*current_token != Symbols::T_CLOSE_PAR)
		Expression_List();
	CheckSequence(Symbols::T_CLOSE_PAR);
}

void Parser::Expression_List(){
	Expression();
	Expression_B();
}

void Parser::Expression_B(){
	while (*current_token == Symbols::T_COMMA){
		GetNextToken();
		Expression();
	}
}

void Parser::Operation_Call(){
	if (NextIsAnyOfThese(Symbols::T_WRITE, Symbols::T_READ)){
		GetNextToken();
		CheckSequence(Symbols::T_OPEN_PAR);
		Args();
		CheckSequence(Symbols::T_CLOSE_PAR);
	}
	else if(*current_token == Symbols::T_WRTLN){
		GetNextToken();
		if (*current_token == Symbols::T_OPEN_PAR){
			Opt_Args();
		}
	}
	else{
		err<<"Expected Function call ";
		DisplayErr(err);
	}
}

void Parser::Opt_Args(){
	CheckSequence(Symbols::T_OPEN_PAR);
	Args();
	CheckSequence(Symbols::T_CLOSE_PAR);
}

void Parser::Args(){
	if (*current_token == Symbols::T_STR_LIT){
		GetNextToken();
	}
	else{
		Expression();
	}
	Args_List();
}

void Parser::Args_List(){
	while (*current_token == Symbols::T_COMMA){
		GetNextToken();
		Args();
	}
}

void Parser::Block(){
	if (*current_token == Symbols::T_BEGIN){
		GetNextToken();
		Statement_List();
		CheckSequence(Symbols::T_END);
	}
	else{
		Statement();
	}
}

void Parser::Final(){
	if (*current_token == Symbols::T_ID){
		Left_Value_or_Oper_Call();
	}
	else if (NextIsAnyOfThese(Symbols::T_NUM, Symbols::T_TRUE, Symbols::T_FALSE)){
		Constant();
	}
	else if (*current_token == Symbols::T_OP_SUB){
		GetNextToken();
		Expression();
	}
	else if (*current_token == Symbols::T_OP_NOT){
		GetNextToken();
		Expression();
	}
	else if (*current_token == Symbols::T_OPEN_PAR){
		Expression();
		CheckSequence(Symbols::T_CLOSE_PAR);
	}
	else{
		err<<"Expected final ";
		DisplayErr(err);
	}
}

void Parser::Constant(){
	if (NextIsAnyOfThese(Symbols::T_NUM)){
		GetNextToken();
	}
	else{
		BoolConstant();
	}
}

void Parser::BoolConstant(){
	if (NextIsAnyOfThese(Symbols::T_TRUE, Symbols::T_FALSE))
		GetNextToken();
	else{
		err<<"Expected Boolean";
		DisplayErr(err);
	}
}

void Parser::Tier1_Loop(){
	while (NextIsAnyOfThese(Symbols::T_OP_NOT)){
		GetNextToken();
		Final();
	}
}

void Parser::Tier2_Loop(){
	while (NextIsAnyOfThese(Symbols::T_OP_MULT, Symbols::T_OP_DIV, Symbols::T_OP_MOD, Symbols::T_OP_AND, Symbols::T_OP_SHL, Symbols::T_OP_SHR)){
		GetNextToken();
		Term2();
	}
}

void Parser::Tier3_Loop(){
	while (NextIsAnyOfThese(Symbols::T_OP_ADD, Symbols::T_OP_SUB, Symbols::T_OP_OR, Symbols::T_OP_XOR)){
		GetNextToken();
		Term1();
	}
}

void Parser::Tier4_Loop(){
	while (NextIsAnyOfThese(Symbols::T_EQUALS_TO, Symbols::T_NOT_ET, Symbols::T_LESS_THAN, Symbols::T_GREATER_THAN, Symbols::T_LT_OR_ET, Symbols::T_GT_OR_ET)){
		GetNextToken();
		Term0();
	}
}

void Parser::Function_Header(){
	CheckSequence(Symbols::T_ID);
	Argument_decl();
	CheckSequence(Symbols::T_COLON);
	DataType();
	CheckSequence(Symbols::T_EOE);
}

void Parser::DataType(){
	if (!(PrimitiveType() || ArrayType())){
		err<<"Expected Primitive or Array type ";
		DisplayErr(err);
	}
}

void Parser::Procedure_Header(){
	CheckSequence(Symbols::T_ID);
	Argument_decl();
	CheckSequence(Symbols::T_EOE);
}

void Parser::Argument_decl(){
	if (*current_token == Symbols::T_OPEN_PAR){
		GetNextToken();
		Argument_decl_List();
		CheckSequence(Symbols::T_CLOSE_PAR);
	}
}

void Parser::Argument_decl_List(){
	if (NextIsAnyOfThese(Symbols::T_VAR, Symbols::T_ID)){
		if (*current_token==Symbols::T_VAR)
			GetNextToken();
		CheckSequence(Symbols::T_ID, Symbols::T_COLON);
		DataType();
		Id_List_B();
	}
}

void Parser::Id_List_B(){
	while (*current_token == Symbols::T_EOE){
		GetNextToken();
		if (NextIsAnyOfThese(Symbols::T_VAR, Symbols::T_ID)){
			if (*current_token==Symbols::T_VAR)
				GetNextToken();
			CheckSequence(Symbols::T_ID, Symbols::T_COLON);
			DataType();
		}
		else{
			err<<"Expected var or identifier ";
			DisplayErr(err);
		}
	}
}

void Parser::Variables(){
	if (*current_token == Symbols::T_VAR){
		GetNextToken();
		do {
			CheckSequence(Symbols::T_ID);
			Variable_decls();
		} while(*current_token == Symbols::T_ID);
	}
}

void Parser::Variable_decls(){
	Id_List();
	CheckSequence(Symbols::T_COLON);
	DataType();
	CheckSequence(Symbols::T_EOE);
}

bool Parser::PrimitiveType(){
	bool t= NextIsAnyOfThese(Symbols::T_INT_TYPE, Symbols::T_BOOL_TYPE, Symbols::T_CHAR_TYPE);
	if (t)
		GetNextToken();
	return t;
}
//This will freak and exit if the sequence to declare an array is improper
//it will also freak and exit if we dont get a primitivetype
bool Parser::ArrayType(){
	//bool t= NextIsAnyOfThese(Symbols::T_ARRAY);
	bool t= (*current_token == Symbols::T_ARRAY);
	if (t){
		CheckSequence(
			Symbols::T_ARRAY, Symbols::T_OPEN_BRACK, Symbols::T_NUM, Symbols::T_DOT_SET,
			Symbols::T_NUM, Symbols::T_CLOSE_BRACK, Symbols::T_OF
		);
		if (!PrimitiveType()){
			err<<"Expected Primitive type ";
			DisplayErr(err);
		}
	}
	return t;
}


void Parser::Id_List(){
	while (*current_token == Symbols::T_COMMA){
		GetNextToken();
		CheckSequence(Symbols::T_ID);
	}
}

void Parser::Parse(){
  Start();
  if (*current_token == Symbols::T_EOF){
    std::cout<<"Syntax ...ok"<<std::endl;
  }
  else{
		err<<"Extra Token\n"<<mylexer.GetCurrentLexeme();
		DisplayError(err, mylexer.GetCurrentRow(),mylexer.GetCurrentColumn());
    std::cout<<std::endl;
  }
}
void Parser::DisplayErr(std::ostringstream& err){
	err<<"Got: "<<std::endl<<*current_token<<std::endl<<mylexer.GetCurrentLexeme();
	DisplayError(err, mylexer.GetCurrentRow(), mylexer.GetCurrentColumn());
};
