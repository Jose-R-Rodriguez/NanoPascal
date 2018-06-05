#include "symbols.hpp"
Symbol::Symbol(int id, std::string name){
	this->id= id;
	this->name= name;
}
Symbol& Symbol::SetSymbolLexeme(std::string lexeme){
	current_lexeme= lexeme;
	return *this;
}
