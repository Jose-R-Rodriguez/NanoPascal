#include "symbols.hpp"
Symbol::Symbol(int id, std::string name){
	this->id= id;
	this->name= name;
}
Symbol& Symbol::SetSymbolLexeme(std::string lexeme){
	current_lexeme= lexeme;
	return *this;
}
std::ostream& operator<<(std::ostream& os, const Symbol& symb){
	os<<symb.id<<"\t\t"<<symb.current_lexeme<<"\t\t"<<symb.name;
	return os;
}
