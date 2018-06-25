#ifndef AST_HPP
#define AST_HPP
#include <deque>
#include <memory>
#include <functional>
#include "common.hpp"
#define DEFINE_N_ARYNODE(name)\
	class name##Node : public Node{\
	public:\
		NP_List child_list;\
		name##Node(){\
			child_list.clear();\
		}\
		template<typename Args>\
		name##Node(Args&& arg){\
			child_list.push_back(std::move(arg));\
		}\
		template<typename First, typename ... Args>\
		name##Node(First&& f, Args&& ... args){\
			std::cout<<"THIS IS ONE"<<std::endl;\
			child_list.push_back(f);\
			name##Node(args...);\
		}\
		std::string toString();\
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

class N_aryNode : public Node{
public:
	NP_List child_list;
};

DEFINE_PRIMITIVE_NODE(Number, int);
DEFINE_PRIMITIVE_NODE(Character, char);
DEFINE_PRIMITIVE_NODE(Boolean, bool);
DEFINE_PRIMITIVE_NODE(Id, std::string);
DEFINE_PRIMITIVE_NODE(IntegerType, std::string);
DEFINE_PRIMITIVE_NODE(BooleanType, std::string);
DEFINE_PRIMITIVE_NODE(CharacterType, std::string);
DEFINE_PRIMITIVE_NODE(BeginBody, std::string);
DEFINE_PRIMITIVE_NODE(EndBody,std::string);
DEFINE_N_ARYNODE(Start);
DEFINE_N_ARYNODE(Variables);
DEFINE_N_ARYNODE(IdList);
DEFINE_N_ARYNODE(VariableDecl);
DEFINE_N_ARYNODE(ArrayType);

class AST{
public:
	AST(NP_List&& pointer_list) : pointer_list(std::move(pointer_list)) {};
	void Print();
	NP_List pointer_list;
};

/*
Non specialized template to help me create unique pointers to simple node types
*/
template<typename T>
Node_Pointer CreatePrimitiveNode(Symbol& symb ,T value){
	if (symb == Symbols::T_NUM){
		return Node_Pointer(new NumberNode(std::stoi(value)));
	}
	else if(symb == Symbols::T_CHAR_CONSTANT){
		return Node_Pointer(new CharacterNode(value[1]));
	}
	else if(symb == Symbols::T_TRUE || symb == Symbols::T_FALSE){
		if (value == "true")
			return Node_Pointer(new BooleanNode(true));
		else
			return Node_Pointer(new BooleanNode(false));
	}
	else if(symb == Symbols::T_ID){
		return Node_Pointer(new IdNode(value));
	}
	else if(symb == Symbols::T_BOOL_TYPE){
		return Node_Pointer(new BooleanTypeNode(value));
	}
	else if(symb == Symbols::T_INT_TYPE){
		return Node_Pointer(new IntegerTypeNode(value));
	}
	else if(symb == Symbols::T_CHAR_TYPE){
		return Node_Pointer(new CharacterTypeNode(value));
	}
	else{
		//std::cout<<"No need to save: "<<value<<std::endl;
		return nullptr;
	}
}
#endif
