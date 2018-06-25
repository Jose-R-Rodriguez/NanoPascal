#include <stdlib.h>
#include "ast.hpp"

void AST::Print(){
	for (auto&& x : pointer_list){
		std::cout<<x->toString()<<std::endl;
	}
}
std::string NumberNode::toString(){
	return std::to_string(value);
}

std::string CharacterNode::toString(){
	return std::string(1, value);
}

std::string BooleanNode::toString(){
	return (value) ? "true" : "false";
}

std::string IdNode::toString(){
	return value;
}

std::string StartNode::toString(){
	std::string output= "program ";
	for (const auto& x : child_list){
		output+= x->toString();
	}
	output+= ";";
	return output;
}

std::string IdListNode::toString(){
	std::string output= "";
	for (const auto& x : child_list){
		output+= ",";
		output+= x->toString();
	}
	return output+ ":";
}

std::string VariableDeclNode::toString(){
	std::string output= "";
	for (const auto& x : child_list){
		output+= x->toString();
	}
	return output+";\n";
}

std::string IntegerTypeNode::toString(){
	return "Integer";
}

std::string CharacterTypeNode::toString(){
	return "char";
}

std::string BooleanTypeNode::toString(){
	return "Boolean";
}

std::string ArrayTypeNode::toString(){
	std::string output= "array[";
	output+= child_list[0]->toString();
	output+= "..";
	output+= child_list[1]->toString();
	output+="] of ";
	output+= child_list[2]->toString();
	return output;
}
std::string VariablesNode::toString(){
	std::string output= "var ";
	for (const auto& x : child_list){
		output+= x->toString();
	}
	return output;
}

std::string BeginBodyNode::toString(){
	return "begin\n";
}

std::string EndBodyNode::toString(){
	return "End.\n";
}

std::string OperationsListNode::toString(){
	std::string output= "";
	for (int x= 0 ; x<child_list.size() ; x++){
		output+= child_list[x]->toString();
	}
	return output;
}

std::string FunctionNode::toString(){
	std::string output= "function ";
	output+= child_list[0]->toString();
	if (child_list[2]){
		output+= "(";
		output+= child_list[1]->toString();
		output+= ") : ";
		output+= child_list[2]->toString();
		output+= ";";
	}
	else{
		output+= ":";
		output+= child_list[1]->toString();
		output+= ";";
	}
	return output+"\n";
}

std::string IdListBNode::toString(){
	std::string output= "";
	for (int x= 0; x<child_list.size() ; x++){
		output+= child_list[x]->toString();
		x++;
		output+= ":";
		output+= child_list[x]->toString();
		output+=";";
	}
	output.pop_back();
	return output;
}

std::string ArgumentDeclListNode::toString(){
	std::string output= "";
	char temp= ':';
	output+= child_list[0]->toString();
	output+= ":";
	output+= child_list[1]->toString();
	if (child_list[2]){
		output+= ";";
		output+= child_list[2]->toString();
	}
	return output;
}

std::string ArgumentDeclNode::toString(){
	std::string output= "";
	for (const auto& x : child_list){
		output+= x->toString();
	}
	return output;
}

std::string ProcedureNode::toString(){
	std::string output= "procedure ";
	output+= child_list[0]->toString();
	if (child_list[1]){
		output+= "(";
		output+= child_list[1]->toString();
		output+= ");";
	}
	else{
		output+=";";
	}
	return output+"\n";
}
