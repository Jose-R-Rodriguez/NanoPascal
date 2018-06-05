#include <iostream>
#include "lexer.hpp"

Lexer::Lexer(std::ifstream& input) : input(input){
	current_row= 0, current_column= 0;
}

char Lexer::GetNextChar(){
	char t;
	if (input.get(t)){
		(t == '\n') ? current_row++, current_column= 0 : current_column++;
		return tolower(t);
	}
	else
		return 0;
}

Symbol& Lexer::ResolveToken(){
	while (1){
		current_char= GetNextChar(), lexeme= "";
		switch (current_char) {
			case 0: return Symbols::T_EOF;
			case '+':
				return Symbols::T_OP_ADD.SetSymbolLexeme(std::string(1, current_char));;
			case '\n':
			case ' ': continue;
			default: return Symbols::T_OP_SUB;
		}
	}
}
