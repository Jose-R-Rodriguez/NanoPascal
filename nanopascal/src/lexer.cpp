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

void Lexer::ConsumeSequence(std::function <bool(char)> func) {
		do{
			lexeme+= current_char;
			current_char= GetNextChar();
		}while(func(current_char));
		input.unget();
		//in.unget();
}


Symbol& Lexer::ResolveToken(){
	while (1){
		current_char= GetNextChar(), lexeme= "";
		switch (current_char) {
			case 0:
			std::cout<<"END OF FILE"<<std::endl;
			return Symbols::T_EOF;
			case '+':
				return Symbols::T_OP_ADD.SetSymbolLexeme(std::string(1, current_char));
			case '-':
				return Symbols::T_OP_SUB.SetSymbolLexeme(std::string(1, current_char));
			case '.':
				return Symbols::T_EOF;
			case '\n':
			case ' ': continue;
			default:
				if (isdigit(current_char)){
					int placeholder;
				}
				else if(isalpha(current_char) || current_char == '_'){
					ConsumeSequence(
						[](char digit){return isalpha(digit) || digit == '_';}
					);
					try{
						return Keywords.at(lexeme).SetSymbolLexeme(lexeme);
					}
					catch(const std::exception&){
						return Symbols::T_ID.SetSymbolLexeme(lexeme);
					}
				}
		}
	}
}
