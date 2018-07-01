#include "ast.hpp"
bool AST::Interpret(){
	for (const auto& x : pointer_list){
		//std::cout<<x->toString()<<std::endl;
		x->interpret();
	}
	return true;
}

Nanopascal_Types StatementNode::interpret(){
	child_list[0]->interpret();
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types AssignOperNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types StatementListNode::interpret(){
	for (const auto& x : child_list){
		x->interpret();
	}
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types StartNode::interpret(){
	Nanopascal_Types x(true);
	return x;
}

Nanopascal_Types VariablesNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types IdListNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types VariableDeclNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ArrayTypeNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types OperationsListNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types FunctionNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ProcedureNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types IdListBNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ArgumentDeclNode::interpret(){
	std::cout<<"Called argument decl node"<<std::endl;
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ArgumentDeclListNode::interpret(){
	std::cout<<"Called argument decl list node "<<std::endl;
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ExpressionNode::interpret(){
	std::cout<<"CALLED EXPRESSION NODE"<<std::endl;
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types Term0Node::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types Term1Node::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types Term2Node::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types Tier1_LoopNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types Tier2_LoopNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types Tier3_LoopNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types Tier4_LoopNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types FinalNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types AssignNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types EqualsNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types NotEqualsNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types LT_ETNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types GT_ETNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types GreaterThanNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types LessThanNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types OrNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types XorNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types AddNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types SubNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ModNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types MultNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types DivNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ShiftRightNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ShiftLeftNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types AndNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types NotNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ArrayAssignNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types WritelnNode::interpret(){
	for (const auto& x : child_list){
		std::cout<<"Interpretting"<<std::endl;
		std::cout<<x->interpret().str;
	}
	std::cout<<std::endl;
	Nanopascal_Types x(true);
	return x;
}

Nanopascal_Types ArgumentsNode::interpret(){
	Nanopascal_Types x(child_list[0]->interpret());
	std::cout<<child_list[1]->toString();
	std::cout<<"CALLED ARGUMENTS NODE"<<std::endl;
	return x;
}

Nanopascal_Types ArgListNode::interpret(){
	std::cout<<"CAlled arglist node"<<std::endl;
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types WriteNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ReadNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ExprListBNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ExprListNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types LeftV_OperCallNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types LeftVArrayNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types ForNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types BlockNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types WhileNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types RepeatNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types StringLiteralNode::interpret(){
	Nanopascal_Types x(value);
	return x;
}

Nanopascal_Types BeginBodyNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types EndBodyNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types EndOperationNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types NumberNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types CharacterNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types BooleanNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types IdNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types BooleanTypeNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types IntegerTypeNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}

Nanopascal_Types CharacterTypeNode::interpret(){
	Nanopascal_Types x(1);
	return x;
}
