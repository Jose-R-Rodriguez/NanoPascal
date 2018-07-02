#include "ast.hpp"
#include <optional>
Context global_context;
void PrintNanopascalType(Nanopascal_Types param, Context& local_context);
int GetNanopascalIntegralType(Nanopascal_Types param, Context& local_context);
bool AST::Interpret(){
	for (const auto& x : pointer_list){
		//std::cout<<x->toString(Context& local_context)<<std::endl;
		x->interpret(global_context);
	}
	//global_context.PrintContext();
	return true;
}

Nanopascal_Types StatementNode::interpret(Context& local_context){
	//std::cout<<"Calling a statement node"<<std::endl;
	child_list[0]->interpret(local_context);
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types AssignOperNode::interpret(Context& local_context){
	std::cout<<"Callin assign oper node"<<std::endl;
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types StatementListNode::interpret(Context& local_context){
	//std::cout<<"Calling statementlist"<<std::endl;
	for (const auto& x : child_list){
		x->interpret(local_context);
	}
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types StartNode::interpret(Context& local_context){
	Nanopascal_Types x(true);
	return x;
}

Nanopascal_Types VariablesNode::interpret(Context& local_context){

	for (int x= 0 ; x<child_list.size(); x++){
		std::string variable_name= std::get<std::string>(child_list[x]->interpret(local_context));
		local_context.variables[variable_name]= 0;
		x++;
		if (child_list[x]){
			child_list[x]->interpret(local_context);
		}
	}
	Nanopascal_Types x(true);
	return x;
}

Nanopascal_Types IdListNode::interpret(Context& local_context){
	std::cout<<"Calling id list "<<std::endl;
	for (const auto& x : child_list){
		x->interpret(local_context);
	}
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types VariableDeclNode::interpret(Context& local_context){
	IdListNode * id_list= dynamic_cast<IdListNode*>(child_list[0].get());
	for (const auto& id : id_list->child_list){
		auto id_name= std::get<std::string>(id->interpret(local_context));
		local_context.variables[id_name]= 0;
	}

	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ArrayTypeNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types OperationsListNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types FunctionNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ProcedureNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types IdListBNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ArgumentDeclNode::interpret(Context& local_context){
	std::cout<<"Called argument decl node"<<std::endl;
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ArgumentDeclListNode::interpret(Context& local_context){
	std::cout<<"Called argument decl list node "<<std::endl;
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ExpressionNode::interpret(Context& local_context){
	std::cout<<"CALLED EXPRESSION NODE"<<std::endl;
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types Term0Node::interpret(Context& local_context){
	std::cout<<"Called term0 node "<<std::endl;
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types Term1Node::interpret(Context& local_context){
	std::cout<<"Called term1 node "<<std::endl;
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types Term2Node::interpret(Context& local_context){
	std::cout<<"Called term2 node "<<std::endl;
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types Tier1_LoopNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types Tier2_LoopNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types Tier3_LoopNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types Tier4_LoopNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types FinalNode::interpret(Context& local_context){
	Nanopascal_Types x(child_list[0]->interpret(local_context));
	return x;
}

int GetNanopascalIntegralType(Nanopascal_Types param, Context& local_context){
	switch (param.index()) {
		case 0:
			return std::get<int>(param);
		case 1:
			return std::get<char>(param);
		case 2:
			return std::get<bool>(param);
		case 3:{
			std::string temp= std::get<std::string>(param);
			if (ExistsInIterable(local_context.variables, temp)){
				return local_context.variables[temp];
			}
			else{
				err<<"Variable name doesn't exist";
				//DisplayError(err, 0 ,0);
			}
		}
		default:
			std::cout<<"Non integral type"<<std::endl;
			return -99;
	}
}

Nanopascal_Types AssignNode::interpret(Context& local_context){
	//std::cout<<"Calling assign node"<<std::endl;
	auto variable_name= std::get<std::string>(child_list[0]->interpret(local_context));
	auto expression_result= child_list[1]->interpret(local_context);
	if (ExistsInIterable(local_context.variables, variable_name)){
		local_context.variables[variable_name]= GetNanopascalIntegralType(expression_result, local_context);
	}
	if (child_list[2]){
		std::cout<<"assign has third child wtf double check this"<<std::endl;
		child_list[2]->interpret(local_context);
	}
	Nanopascal_Types x(true);
	return x;
}

Nanopascal_Types EqualsNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types NotEqualsNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types LT_ETNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types GT_ETNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types GreaterThanNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types LessThanNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types OrNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types XorNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types AddNode::interpret(Context& local_context){
	auto t= child_list[0]->interpret(local_context);
	auto t1= child_list[1]->interpret(local_context);
	int first= GetNanopascalIntegralType(t, local_context);
	int second= GetNanopascalIntegralType(t1, local_context);
	int result= first+second;
	Nanopascal_Types x(result);
	return x;
}

Nanopascal_Types SubNode::interpret(Context& local_context){
	auto t= child_list[0]->interpret(local_context);
	auto t1= child_list[1]->interpret(local_context);
	int first= GetNanopascalIntegralType(t, local_context);
	int second= GetNanopascalIntegralType(t1, local_context);
	int result= first-second;
	Nanopascal_Types x(result);
	return x;
}

Nanopascal_Types ModNode::interpret(Context& local_context){
	auto t= child_list[0]->interpret(local_context);
	auto t1= child_list[1]->interpret(local_context);
	int first= GetNanopascalIntegralType(t, local_context);
	int second= GetNanopascalIntegralType(t1, local_context);
	int result= first%second;
	Nanopascal_Types x(result);
	return x;
}

Nanopascal_Types MultNode::interpret(Context& local_context){
	auto t= child_list[0]->interpret(local_context);
	auto t1= child_list[1]->interpret(local_context);
	int first= GetNanopascalIntegralType(t, local_context);
	int second= GetNanopascalIntegralType(t1, local_context);
	int result= first*second;
	Nanopascal_Types x(result);
	return x;
}

Nanopascal_Types DivNode::interpret(Context& local_context){
	auto t= child_list[0]->interpret(local_context);
	auto t1= child_list[1]->interpret(local_context);
	int first= GetNanopascalIntegralType(t, local_context);
	int second= GetNanopascalIntegralType(t1, local_context);
	int result= first/second;
	Nanopascal_Types x(result);
	return x;
}

Nanopascal_Types ShiftRightNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ShiftLeftNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types AndNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types NotNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ArrayAssignNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

void PrintNanopascalType(Nanopascal_Types param, Context& local_context){
	switch (param.index()) {
		case 0:
			std::cout<<std::get<int>(param);
		break;
		case 1:
			std::cout<<std::get<char>(param);
		break;
		case 2:
			std::cout<<std::get<bool>(param);
		break;
		case 3:
			std::string temp= std::get<std::string>(param);
			if (temp[0] == '\''){
				std::cout<<temp.substr(1, temp.size()-2);;
			}
			else{
				if (ExistsInIterable(local_context.variables, temp)){
					std::cout<<local_context.variables[temp];
				}
				else{
					err<<"Variable name doesn't exist";
					//DisplayError(err, 0 ,0);
				}
			}
		break;
	}
}

void PrintArguments(ArgumentsNode* initial_node, Context& local_context){
	if (initial_node->child_list[0]){
		auto result= initial_node->child_list[0]->interpret(local_context);
		PrintNanopascalType(result, local_context);
		if (initial_node->child_list[1]){
			ArgListNode* arg_list= dynamic_cast<ArgListNode*>(initial_node->child_list[1].get());
			ArgumentsNode* argmt_ptr= dynamic_cast<ArgumentsNode*>(arg_list->child_list[0].get());
			PrintArguments(argmt_ptr, local_context);
		}
	}
}

Nanopascal_Types WritelnNode::interpret(Context& local_context){
	if (child_list[0]){
		ArgumentsNode* argmt_ptr= dynamic_cast<ArgumentsNode*>(child_list[0].get());
		PrintArguments(argmt_ptr, local_context);
	}
	std::cout<<std::endl;
	Nanopascal_Types x(true);
	return x;
}

Nanopascal_Types ArgumentsNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	//std::cout<<"Second in list"<<child_list[1]->toString(Context& local_context);
	std::cout<<"CALLED ARGUMENTS NODE"<<std::endl;
	return x;
}

Nanopascal_Types ArgListNode::interpret(Context& local_context){
	std::cout<<"Called arglist node"<<std::endl;
	//std::cout<<child_list[0]->toString(Context& local_context)<<std::endl;
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types WriteNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ReadNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ExprListBNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ExprListNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types LeftV_OperCallNode::interpret(Context& local_context){
	Nanopascal_Types x(child_list[0]->interpret(local_context));
	return x;
}

Nanopascal_Types LeftVArrayNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ForNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types BlockNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types WhileNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types RepeatNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types StringLiteralNode::interpret(Context& local_context){
	Nanopascal_Types x(value);
	return x;
}

Nanopascal_Types BeginBodyNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types EndBodyNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types EndOperationNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types NumberNode::interpret(Context& local_context){
	Nanopascal_Types x(value);
	return x;
}

Nanopascal_Types CharacterNode::interpret(Context& local_context){
	Nanopascal_Types x(value);
	return x;
}

Nanopascal_Types BooleanNode::interpret(Context& local_context){
	Nanopascal_Types x(value);
	return x;
}

Nanopascal_Types IdNode::interpret(Context& local_context){
	Nanopascal_Types x(value);
	return x;
}

Nanopascal_Types BooleanTypeNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types IntegerTypeNode::interpret(Context& local_context){
	std::cout<<"Integer type node being called"<<std::endl;
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types CharacterTypeNode::interpret(Context& local_context){
	Nanopascal_Types x(1);
	return x;
}
