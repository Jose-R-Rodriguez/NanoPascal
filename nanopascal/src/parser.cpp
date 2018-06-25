#include "parser.hpp"
Parser::Parser(Lexer& mylexer) : mylexer(mylexer), current_token(&mylexer.ResolveToken()){
	Parse();
}

/*template<typename First, typename ... Symbols>
void Parser::CheckSequence(First& symb, Symbols&... rest){
	if (symb != *current_token){
		err<<"Expected:"<<std::endl<<symb<<std::endl;
		DisplayErr(err);
	}
	GetNextToken();
	CheckSequence(rest...);
}*/
template<typename ... Symbols>
NP_List Parser::CheckSequence(Symbols&... args){
	NP_List sequence_pointers;
	Node_Pointer temp;
	for (auto& symb : {args...}){
		if (symb.id != current_token->id){
			err<<"Expected:"<<std::endl<<symb<<std::endl;
			DisplayErr(err);
		}
		temp= CreatePrimitiveNode(*current_token, mylexer.GetCurrentLexeme());
		if (temp){
			sequence_pointers.push_back(std::move(temp));
		}
		GetNextToken();
	}
	return sequence_pointers;
}

template<typename First, typename ... Symbols>
bool Parser::NextIsAnyOfThese(First& symb, Symbols&... rest){
	return (symb==*current_token) || NextIsAnyOfThese(rest...);
}

NP_List Parser::Start(){
	NP_List pointers_list, temp;
	temp= CheckSequence(Symbols::T_PROG, Symbols::T_ID, Symbols::T_EOE);
	pointers_list.push_back(std::make_unique<StartNode>(temp[0]));
	auto t= Variables();
	if (t)
		pointers_list.push_back(std::move(t));
	Operations_List();
	CheckSequence(Symbols::T_BEGIN);
	pointers_list.push_back(std::make_unique<BeginBodyNode>(""));
	Statement_List();
	CheckSequence(Symbols::T_END, Symbols::T_END_OF_PROG);
	pointers_list.push_back(std::make_unique<EndBodyNode>(""));
	return pointers_list;
}

Node_Pointer Parser::Operations_List(){
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
	while (NextIsAnyOfThese(Symbols::T_ID, Symbols::T_IF, Symbols::T_WHILE, Symbols::T_REPEAT, Symbols::T_FOR, Symbols::T_BREAK, Symbols::T_CONTINUE, Symbols::T_WRITE, Symbols::T_WRTLN, Symbols::T_READ)){
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
		Expression();
		CheckSequence(Symbols::T_DO);
		Block();
	}
	else if (*current_token == Symbols::T_REPEAT){
		GetNextToken();
		Block();
		CheckSequence(Symbols::T_UNTIL);
		Expression();
		CheckSequence(Symbols::T_EOE);
	}
	else if (*current_token == Symbols::T_FOR){
		GetNextToken();
		Assign();
		Expression();
		CheckSequence(Symbols::T_TO);
		Expression();
		CheckSequence(Symbols::T_DO);
		Block();
	}
	else if (NextIsAnyOfThese(Symbols::T_BREAK, Symbols::T_CONTINUE)){
		GetNextToken();
	}
	else{
		err<<"Expected Assignment or statement call ";
		DisplayErr(err);
	}
}

void Parser::Assign(){
	Left_Value();
	CheckSequence(Symbols::T_EQUALS_TO);
	Expression();
}

void Parser::Left_Value(){
	CheckSequence(Symbols::T_ID);
	if (*current_token == Symbols::T_OPEN_BRACK){
		GetNextToken();
		Expression();
		CheckSequence(Symbols::T_CLOSE_BRACK);
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
			Opt_Expr();
		}
	}
	else if (*current_token == Symbols::T_OPEN_PAR){
		Opt_Expr();
	}
	else{
		Operation_Call();
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
	if (*current_token != Symbols::T_CLOSE_PAR){
		Expression_List();
	}
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
	else if (*current_token != Symbols::T_CLOSE_PAR){
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
	if (NextIsAnyOfThese(Symbols::T_ID, Symbols::T_WRITE, Symbols::T_WRTLN, Symbols::T_READ)){
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
		GetNextToken();
		Expression();
		CheckSequence(Symbols::T_CLOSE_PAR);
	}
	else{
		err<<"Expected final ";
		DisplayErr(err);
	}
}

Node_Pointer Parser::Constant(){
	Node_Pointer NP;
	if (NextIsAnyOfThese(Symbols::T_NUM, Symbols::T_CHAR_CONSTANT)){
		NP= CreatePrimitiveNode(*current_token, mylexer.GetCurrentLexeme());
		GetNextToken();
	}
	else{
		BoolConstant();
	}
	return NP;
}

Node_Pointer Parser::BoolConstant(){
	Node_Pointer NP;
	if (NextIsAnyOfThese(Symbols::T_TRUE, Symbols::T_FALSE)){
		NP= CreatePrimitiveNode(*current_token, mylexer.GetCurrentLexeme());
		GetNextToken();
	}
	else{
		err<<"Expected Constant";
		DisplayErr(err);
	}
	return NP;
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

Node_Pointer Parser::DataType(){
	auto t1= PrimitiveType();
	auto t2= ArrayType();
	if (!(t1  || t2)){
		err<<"Expected Primitive or Array type ";
		DisplayErr(err);
	}
	//(t1) ? std::cout<<t1->toString()<<std::endl : std::cout<<t2->toString()<<std::endl;
	return (t1) ? std::move(t1) : std::move(t2);
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

Node_Pointer Parser::Variables(){
	if (*current_token == Symbols::T_VAR){
		GetNextToken();
		std::unique_ptr<VariablesNode> variables_ptr= std::make_unique<VariablesNode>();
		variables_ptr->child_list.clear();
		do {
			auto temp= CheckSequence(Symbols::T_ID);
				variables_ptr->child_list.push_back(std::move(temp[0]));
			auto temp1= Variable_decls();
			if (temp1){
				variables_ptr->child_list.push_back(std::move(temp1));
			}
		} while(*current_token == Symbols::T_ID);
		return variables_ptr;
	}
	else{
		return nullptr;
	}

}

Node_Pointer Parser::Variable_decls(){
	auto idlst= Id_List();
	std::unique_ptr<VariableDeclNode> variable_decls_ptr= std::make_unique<VariableDeclNode>(idlst);
	CheckSequence(Symbols::T_COLON);
	variable_decls_ptr->child_list.push_back(DataType());
	CheckSequence(Symbols::T_EOE);
	return variable_decls_ptr;
}

Node_Pointer Parser::PrimitiveType(){
	if (NextIsAnyOfThese(Symbols::T_INT_TYPE, Symbols::T_BOOL_TYPE, Symbols::T_CHAR_TYPE)){
		auto t= CreatePrimitiveNode(*current_token, "");
		GetNextToken();
		return t;
	}
	else{
		return nullptr;
	}
}
//This will freak and exit if the sequence to declare an array is improper
//it will also freak and exit if we dont get a primitivetype
Node_Pointer Parser::ArrayType(){
	std::unique_ptr<ArrayTypeNode> array_ptr= std::make_unique<ArrayTypeNode>();
	array_ptr->child_list.empty();
	if (*current_token == Symbols::T_ARRAY){
		auto list= CheckSequence(
			Symbols::T_ARRAY, Symbols::T_OPEN_BRACK, Symbols::T_NUM, Symbols::T_DOT_SET,
			Symbols::T_NUM, Symbols::T_CLOSE_BRACK, Symbols::T_OF
		);
		array_ptr->child_list.push_back(std::move(list[0]));
		array_ptr->child_list.push_back(std::move(list[1]));
		auto temp= PrimitiveType();
		if (!temp){
			err<<"Expected Primitive type ";
			DisplayErr(err);
		}
		array_ptr->child_list.push_back(std::move(temp));
		return array_ptr;
	}
	else{
		return nullptr;
	}
}


Node_Pointer Parser::Id_List(){
	std::unique_ptr<IdListNode> listnode_ptr= std::make_unique<IdListNode>();
	NP_List temp_lst;
	listnode_ptr->child_list.clear();
	while (*current_token == Symbols::T_COMMA){
		GetNextToken();
		temp_lst= CheckSequence(Symbols::T_ID);
		listnode_ptr->child_list.push_back(std::move(temp_lst[0]));
	}
	return listnode_ptr;
}

void Parser::Parse(){
  AST myast(Start());
  if (*current_token == Symbols::T_EOF){
    std::cout<<"Syntax ...ok"<<std::endl;
		myast.Print();
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
