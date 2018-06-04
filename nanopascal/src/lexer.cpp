#include <iostream>
#include "lexer.hpp"

Lexer::Lexer(std::ifstream& input) : input(input){
	current_row= 0, current_column= 1;
}
char Lexer::GetNextChar(){
	char t;
	return input.get(t) ? t : 0
}
unsigned int Lexer::ResolveToken(){
	current_char= GetNextChar();
	while (1){
		
	}
}
