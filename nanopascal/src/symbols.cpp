#include <iomanip>
#include "symbols.hpp"
Symbol::Symbol(int id, std::string name){
	this->id= id;
	this->name= name;
}

std::ostream& operator<<(std::ostream& os, const Symbol& symb){
	os<<symb.id<<std::setw(25)<<symb.name;
	return os;
}
PreprocessorSymbols::PreprocessorSymbols(int id, std::string name, Symbol& symb):Symbol(id, name), els(symb){
}
