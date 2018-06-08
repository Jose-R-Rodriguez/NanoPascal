#include "lexer.hpp"
#define RETURN_TOKEN(tk)\
	do{lexeme= current_char;\
	return tk;}while(0)
Lexer::Lexer(std::ifstream& input) : input(input){
	current_row= 1, current_column= 1;
}

char Lexer::GetNextChar(){
	char t;
	if (input.get(t)){
		(t == '\n') ? (current_row++, current_column= 1) : current_column++;
		return tolower(t);
	}
	else
		return 0;
}

std::string Lexer::GetCurrentLexeme(){
	return lexeme;
}

void Lexer::ConsumeSequence(std::function <bool(char)> func, bool unget= false) {
	do{
		if (!current_char){
			err<<"Unexpected End of File, sequence left open: "<<lexeme;
			DisplayError(err, current_row, current_column);
		}
		lexeme+= current_char;
		current_char= GetNextChar();
	}while(func(current_char));
	if (unget){
		input.unget();
		current_column--;
	}
}

bool Lexer::PeekAndCompare(char t){
	return (input.peek()==t) ? (GetNextChar(), true) : false;
}

Symbol& Lexer::ResolveToken(){
	while (1){
		current_char= GetNextChar(), lexeme= "";
		switch (current_char) {
			case '.': RETURN_TOKEN(Symbols::T_END_OF_PROG);
			case 0:
				std::cout<<"END OF FILE"<<std::endl;
				return Symbols::T_EOF;
			case ';': RETURN_TOKEN(Symbols::T_EOE);
			case '+': RETURN_TOKEN(Symbols::T_OP_ADD);
			case '-': RETURN_TOKEN(Symbols::T_OP_SUB);
			case ')': RETURN_TOKEN(Symbols::T_CLOSE_PAR);
			case ',': RETURN_TOKEN(Symbols::T_COMMA);
			case '*': RETURN_TOKEN(Symbols::T_OP_MULT);
			case '$':
					ConsumeSequence([](char digit){return isxdigit(digit);}, true);
					return Symbols::T_Hex_Num;
			case '>':
				if(PeekAndCompare('=')){
					lexeme+=">=";
					return Symbols::T_GT_OR_ET;
				}
			RETURN_TOKEN(Symbols::T_GREATER_THAN);
			case '<':
				if(PeekAndCompare('=')){
					lexeme+="<=";
					return Symbols::T_LT_OR_ET;
				}
				RETURN_TOKEN(Symbols::T_LESS_THAN);
			case '=': RETURN_TOKEN(Symbols::T_EQUALS_TO);
			case ':':
				if (PeekAndCompare('=')){
					lexeme+= ":=";
					return Symbols::T_EQUALS;
				}
				RETURN_TOKEN(Symbols::T_SEMICOLON);
			case '{':
				if(PeekAndCompare('$')){
					std::cout<<"FOUND A DIRECTIVE"<<std::endl;
					ConsumeSequence(
						[this](char character){return (character == '}') ? ((lexeme+= character), false): true;}
					);continue;
				}
				else{
					ConsumeSequence([this](char character){return !(character == '}');});
				}continue;
				RETURN_TOKEN(Symbols::T_OPEN_CURLY);
			case '}': RETURN_TOKEN(Symbols::T_CLOSE_CURLY);
			case '(':
				if (PeekAndCompare('*')){
					ConsumeSequence([this](char character){
						return (PeekAndCompare(')') && character == '*') ? false : true;});
					continue;
				}
			RETURN_TOKEN(Symbols::T_OPEN_PAR);
			case '\'':
				ConsumeSequence(//When it finds ' then break out of loop and save that char to lexeme
					[this](char character){return (character == '\'') ? (lexeme+= character), false: true;}
				);
				return Symbols::T_STR_LIT;
			case '/':
				if(PeekAndCompare('/')){
					ConsumeSequence([this](char character){return !(character=='\n');});
					continue;
				}
			case '\n':
			case ' ': continue;
			default:
				if (isdigit(current_char)){
					ConsumeSequence([](char digit){return isdigit(digit);}, true);
					return Symbols::T_NUM;
				}
				else if(isalpha(current_char) || current_char == '_'){
					ConsumeSequence(
						[](char alpha){return isalpha(alpha) || alpha == '_';}, true);
					try{
						return Keywords.at(lexeme);
					}
					catch(const std::exception&){
						return Symbols::T_ID;
					}
				}
				else{
					err<<"Unknown Token: "<<current_char;
					DisplayError(err, current_row, current_column);
				}
		}
	}
}
