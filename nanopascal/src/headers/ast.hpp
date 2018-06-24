#ifndef AST_HPP
#define AST_HPP
#include <deque>
#include <memory>
#include <functional>
#include "common.hpp"
#define DEFINE_N_ARYNODE(name)\
	class name##Node : public N_aryNode{\
	public:\
		template<typename ... Args>\
		name##Node(Args ... args){\
			for (auto x : {args...}){\
				child_list.push_back(x);\
			}\
		}\
	};
#define DEFINE_PRIMITIVE_NODE(name, type)\
	class name##Node : public Node{\
		public:\
		type value;\
		name##Node(type value) : value(value){}\
		std::string toString();\
	};

class Node{
public:
	virtual std::string toString() =0;
	virtual ~Node(){}
};

using Node_Pointer= std::unique_ptr<Node>;
using NP_List= std::deque<Node_Pointer>;

class N_aryNode : Node{
public:
	NP_List child_list;
};

DEFINE_PRIMITIVE_NODE(Number, int);
DEFINE_PRIMITIVE_NODE(Character, char);
DEFINE_PRIMITIVE_NODE(Boolean, bool);
DEFINE_N_ARYNODE(Add);
DEFINE_N_ARYNODE(Subtract);

/*
Non specialized template to help me create unique pointers to simple node types
*/
template<typename T>
Node_Pointer CreatePrimitiveNode(Symbol& symb ,T value){
	if (symb == Symbols::T_NUM){
		return Node_Pointer(new NumberNode(stoi(value)));
	}
	else if(symb == Symbols::T_CHAR_TYPE){
		return Node_Pointer(new CharacterNode(value[1]));
	}
	else if(symb == Symbols::T_TRUE || symb == Symbols::T_FALSE){
		if (value == "true")
			return Node_Pointer(new BooleanNode(true));
		else
			return Node_Pointer(new BooleanNode(false));
	}
	else{
		err<<"Attempting to create a node which has no support yet";
		std::cout<<err.str()<<std::endl;
		exit(1);
	}
}
#endif
