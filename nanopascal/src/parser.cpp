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
	auto t1= Operations_List();
	if (t1)
		pointers_list.push_back(std::move(t1));
	CheckSequence(Symbols::T_BEGIN);
	pointers_list.push_back(std::make_unique<BeginBodyNode>(""));
	auto t2= Statement_List();
	if (t2){
		pointers_list.push_back(std::move(t2));
	}
	CheckSequence(Symbols::T_END, Symbols::T_END_OF_PROG);
	pointers_list.push_back(std::make_unique<EndBodyNode>(""));
	return pointers_list;
}

Node_Pointer Parser::Operations_List(){
	std::unique_ptr<OperationsListNode> oper_lst_ptr= std::make_unique<OperationsListNode>();
	oper_lst_ptr->child_list.clear();
	if (NextIsAnyOfThese(Symbols::T_FUNCTION, Symbols::T_PROCEDURE)){
		while (NextIsAnyOfThese(Symbols::T_FUNCTION, Symbols::T_PROCEDURE)){
			//if its a function then get next token and call function header otherwise get token and call procedure header
			Node_Pointer temp;
			(*current_token==Symbols::T_FUNCTION) ?
			(GetNextToken(), temp= Function_Header()) : (GetNextToken(), temp= Procedure_Header());
			oper_lst_ptr->child_list.push_back(std::move(temp));
			auto v_temp_ptr= Variables();
			if (v_temp_ptr){
				oper_lst_ptr->child_list.push_back(std::move(v_temp_ptr));
			}
			CheckSequence(Symbols::T_BEGIN);
			oper_lst_ptr->child_list.push_back(std::make_unique<BeginBodyNode>(""));
			auto stmt_lst_ptr= Statement_List();
			if (stmt_lst_ptr)
				oper_lst_ptr->child_list.push_back(std::move(stmt_lst_ptr));
			CheckSequence(Symbols::T_END, Symbols::T_EOE);
			oper_lst_ptr->child_list.push_back(std::make_unique<EndOperationNode>(""));
		}
		return oper_lst_ptr;
	}
	else{
		return nullptr;
	}
}

Node_Pointer Parser::Statement_List(){
	if (NextIsAnyOfThese(Symbols::T_ID, Symbols::T_IF, Symbols::T_WHILE, Symbols::T_REPEAT, Symbols::T_FOR, Symbols::T_BREAK, Symbols::T_CONTINUE, Symbols::T_WRITE, Symbols::T_WRTLN, Symbols::T_READ)){
		std::unique_ptr<StatementListNode> stmt_lst_ptr= std::make_unique<StatementListNode>();
		stmt_lst_ptr->child_list.clear();
		while (NextIsAnyOfThese(Symbols::T_ID, Symbols::T_IF, Symbols::T_WHILE, Symbols::T_REPEAT, Symbols::T_FOR, Symbols::T_BREAK, Symbols::T_CONTINUE, Symbols::T_WRITE, Symbols::T_WRTLN, Symbols::T_READ)){
			stmt_lst_ptr->child_list.push_back(Statement());
		}
		return stmt_lst_ptr;
	}
	else
		return nullptr;
}

Node_Pointer Parser::Statement(){
	std::unique_ptr<StatementNode> stmt_ptr= std::make_unique<StatementNode>();
	stmt_ptr->child_list.clear();
	if (NextIsAnyOfThese(Symbols::T_ID, Symbols::T_WRITE, Symbols::T_WRTLN, Symbols::T_READ)){
		stmt_ptr->child_list.push_back(Assign_or_Oper_Call());
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
	return stmt_ptr;
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

Node_Pointer Parser::Expression(){
	std::unique_ptr<ExpressionNode> expr_ptr= std::make_unique<ExpressionNode>();
	expr_ptr->child_list.clear();
	expr_ptr->child_list.push_back(Term0());
	auto t= Tier4_Loop();
	if (t)
		expr_ptr->child_list.push_back(std::move(t));
	return expr_ptr;
}

Node_Pointer Parser::Term0(){
	std::unique_ptr<Term0Node> trm_ptr= std::make_unique<Term0Node>();
	trm_ptr->child_list.clear();
	trm_ptr->child_list.push_back(Term1());
	auto t= Tier3_Loop();
	if (t)
		trm_ptr->child_list.push_back(std::move(t));
	return trm_ptr;
}

Node_Pointer Parser::Term1(){
	std::unique_ptr<Term1Node> trm_ptr= std::make_unique<Term1Node>();
	trm_ptr->child_list.clear();
	trm_ptr->child_list.push_back(Term2());
	auto t= Tier2_Loop();
	if (t)
		trm_ptr->child_list.push_back(std::move(t));
	return trm_ptr;
}

Node_Pointer Parser::Term2(){
	std::unique_ptr<Term2Node> trm_ptr= std::make_unique<Term2Node>();
	trm_ptr->child_list.clear();
	trm_ptr->child_list.push_back(Final());
	auto t= Tier1_Loop();
	if (t)
		trm_ptr->child_list.push_back(std::move(t));
	return trm_ptr;
}

Node_Pointer Parser::Assign_or_Oper_Call(){
	std::unique_ptr<AssignOperNode> assign_oper_ptr= std::make_unique<AssignOperNode>();
	assign_oper_ptr->child_list.clear();
	if (*current_token == Symbols::T_ID){
		auto t= CreatePrimitiveNode(*current_token, mylexer.GetCurrentLexeme());
		assign_oper_ptr->child_list.push_back(std::move(t));
		GetNextToken();
		if (*current_token == Symbols::T_EQUALS){
			GetNextToken();
			std::unique_ptr<AssignNode> assign_ptr= std::make_unique<AssignNode>(std::move(assign_oper_ptr->child_list[0]), Expression());
			return assign_ptr;
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
	return assign_oper_ptr;
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

Node_Pointer Parser::Final(){
	std::unique_ptr<FinalNode> final_ptr= std::make_unique<FinalNode>();
	final_ptr->child_list.clear();
	if (NextIsAnyOfThese(Symbols::T_ID, Symbols::T_WRITE, Symbols::T_WRTLN, Symbols::T_READ)){
		Left_Value_or_Oper_Call();
	}
	else if (NextIsAnyOfThese(Symbols::T_NUM, Symbols::T_TRUE, Symbols::T_FALSE)){
		final_ptr->child_list.push_back(Constant());
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
	return final_ptr;
}

Node_Pointer Parser::Constant(){
	Node_Pointer NP;
	if (NextIsAnyOfThese(Symbols::T_NUM, Symbols::T_CHAR_CONSTANT)){
		NP= CreatePrimitiveNode(*current_token, mylexer.GetCurrentLexeme());
		GetNextToken();
	}
	else{
		NP= BoolConstant();
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

Node_Pointer Parser::Tier1_Loop(){
	std::unique_ptr<Tier1_LoopNode> tier1_ptr= std::make_unique<Tier1_LoopNode>();
	tier1_ptr->child_list.clear();
	if (NextIsAnyOfThese(Symbols::T_OP_NOT)){
		while (NextIsAnyOfThese(Symbols::T_OP_NOT)){
			GetNextToken();
			Final();
		}
		return tier1_ptr;
	}
	else
		return nullptr;
}

Node_Pointer Parser::Tier2_Loop(){
	std::unique_ptr<Tier2_LoopNode> tier2_ptr= std::make_unique<Tier2_LoopNode>();
	tier2_ptr->child_list.clear();
	if (NextIsAnyOfThese(Symbols::T_OP_MULT, Symbols::T_OP_DIV, Symbols::T_OP_MOD, Symbols::T_OP_AND, Symbols::T_OP_SHL, Symbols::T_OP_SHR)){
		while (NextIsAnyOfThese(Symbols::T_OP_MULT, Symbols::T_OP_DIV, Symbols::T_OP_MOD, Symbols::T_OP_AND, Symbols::T_OP_SHL, Symbols::T_OP_SHR)){
			GetNextToken();
			Term2();
		}
		return tier2_ptr;
	}
	else
		return nullptr;
}

Node_Pointer Parser::Tier3_Loop(){
	std::unique_ptr<Tier3_LoopNode> tier3_ptr= std::make_unique<Tier3_LoopNode>();
	tier3_ptr->child_list.clear();
	if (NextIsAnyOfThese(Symbols::T_OP_ADD, Symbols::T_OP_SUB, Symbols::T_OP_OR, Symbols::T_OP_XOR)){
		while (NextIsAnyOfThese(Symbols::T_OP_ADD, Symbols::T_OP_SUB, Symbols::T_OP_OR, Symbols::T_OP_XOR)){
			GetNextToken();
			Term1();
		}
		return tier3_ptr;
	}
	else
		return nullptr;
}

Node_Pointer Parser::Tier4_Loop(){
	std::unique_ptr<Tier4_LoopNode> tier4_ptr= std::make_unique<Tier4_LoopNode>();
	tier4_ptr->child_list.clear();
	if (NextIsAnyOfThese(Symbols::T_EQUALS_TO, Symbols::T_NOT_ET, Symbols::T_LESS_THAN, Symbols::T_GREATER_THAN, Symbols::T_LT_OR_ET, Symbols::T_GT_OR_ET)){
		while (NextIsAnyOfThese(Symbols::T_EQUALS_TO, Symbols::T_NOT_ET, Symbols::T_LESS_THAN, Symbols::T_GREATER_THAN, Symbols::T_LT_OR_ET, Symbols::T_GT_OR_ET)){
			GetNextToken();
			Term0();
		}
		return tier4_ptr;
	}
	else
		return nullptr;
}

Node_Pointer Parser::Function_Header(){
	std::unique_ptr<FunctionNode> function_ptr= std::make_unique<FunctionNode>();
	function_ptr->child_list.clear();
	auto tmp_lst= CheckSequence(Symbols::T_ID);
	function_ptr->child_list.push_back(std::move(tmp_lst[0]));
	auto tmp_ptr= Argument_decl();
	if (tmp_ptr)
		function_ptr->child_list.push_back(std::move(tmp_ptr));
	CheckSequence(Symbols::T_COLON);
	function_ptr->child_list.push_back(DataType());
	CheckSequence(Symbols::T_EOE);
	return function_ptr;
}

Node_Pointer Parser::DataType(){
	auto t1= PrimitiveType();
	auto t2= ArrayType();
	if (!(t1  || t2)){
		err<<"Expected Primitive or Array type ";
		DisplayErr(err);
	}
	return (t1) ? std::move(t1) : std::move(t2);
}

Node_Pointer Parser::Procedure_Header(){
	std::unique_ptr<ProcedureNode> proc_ptr= std::make_unique<ProcedureNode>();
	proc_ptr->child_list.clear();
	auto tmp_lst= CheckSequence(Symbols::T_ID);
	proc_ptr->child_list.push_back(std::move(tmp_lst[0]));
	proc_ptr->child_list.push_back(Argument_decl());
	CheckSequence(Symbols::T_EOE);
	return proc_ptr;
}

Node_Pointer Parser::Argument_decl(){
	std::unique_ptr<ArgumentDeclNode> argmt_decl_ptr= std::make_unique<ArgumentDeclNode>();
	argmt_decl_ptr->child_list.clear();
	if (*current_token == Symbols::T_OPEN_PAR){
		GetNextToken();
		auto t= Argument_decl_List();
		if (t)
			argmt_decl_ptr->child_list.push_back(std::move(t));
		CheckSequence(Symbols::T_CLOSE_PAR);
		return argmt_decl_ptr;
	}
	else{
		return nullptr;
	}
}

Node_Pointer Parser::Argument_decl_List(){
	std::unique_ptr<ArgumentDeclListNode> argmt_decl_lst_ptr= std::make_unique<ArgumentDeclListNode>();
	argmt_decl_lst_ptr->child_list.clear();
	if (NextIsAnyOfThese(Symbols::T_VAR, Symbols::T_ID)){
		if (*current_token==Symbols::T_VAR)
			GetNextToken();
		auto t= CheckSequence(Symbols::T_ID, Symbols::T_COLON);
		argmt_decl_lst_ptr->child_list.push_back(std::move(t[0]));
		argmt_decl_lst_ptr->child_list.push_back(DataType());
		argmt_decl_lst_ptr->child_list.push_back(Id_List_B());
		return argmt_decl_lst_ptr;
	}
	else{
		return nullptr;
	}
}

Node_Pointer Parser::Id_List_B(){
	std::unique_ptr<IdListBNode> listb_ptr= std::make_unique<IdListBNode>();
	listb_ptr->child_list.clear();
	if (*current_token == Symbols::T_EOE){
		while (*current_token == Symbols::T_EOE){
			GetNextToken();
			if (NextIsAnyOfThese(Symbols::T_VAR, Symbols::T_ID)){
				if (*current_token==Symbols::T_VAR)
					GetNextToken();
				auto t= CheckSequence(Symbols::T_ID, Symbols::T_COLON);
				listb_ptr->child_list.push_back(std::move(t[0]));
				listb_ptr->child_list.push_back(DataType());
			}
			else{
				err<<"Expected var or identifier ";
				DisplayErr(err);
			}
		}
		return listb_ptr;
	}
	else
		return nullptr;
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
