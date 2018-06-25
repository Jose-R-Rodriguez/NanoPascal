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
