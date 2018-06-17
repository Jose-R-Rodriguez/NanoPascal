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
	if (NextIsAnyOfThese(Symbols::T_FUNCTION, Symbols::T_PROCEDURE)){
		(*current_token==Symbols::T_FUNCTION) ?
		(GetNextToken(), Function_Header()) : (GetNextToken(), Procedure_Header());
		Variables();
		CheckSequence(Symbols::T_BEGIN);
		//Statement_List();
		CheckSequence(Symbols::T_END, Symbols::T_EOE);
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
