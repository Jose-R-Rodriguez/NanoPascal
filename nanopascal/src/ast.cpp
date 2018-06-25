#include <stdlib.h>
#include "ast.hpp"


std::string NumberNode::toString(){
	return std::to_string(value);
}

std::string CharacterNode::toString(){
	return std::string(1, value);
}

std::string BooleanNode::toString(){
	return (value) ? "true" : "false";
}

std::string ProgramNode::toString(){
	return value;
}

std::string IdNode::toString(){
	return value;
}

std::string SemicolonNode::toString(){
	return std::string(1, value);
}
