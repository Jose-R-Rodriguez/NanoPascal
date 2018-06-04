#include <iostream>
#include "lexer.hpp"

Lexer::Lexer(std::ifstream& input) : input(input){
	current_row= 0, current_column= 1;
}

char Lexer::GetNextChar(){
	char t;
	return input.get(t) ? t : 0;
}

Symbol Lexer::ResolveToken(){
	while (1){
		current_char= GetNextChar();
		switch (current_char) {
			case 's':return Symbols::T_EOF;
		}
	}
}
