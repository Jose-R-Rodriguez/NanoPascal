#include "lexer.hpp"
#define RETURN_TOKEN(tk)\
	do{lexeme= current_char;\
	return tk;}while(0)
Lexer::Lexer(std::ifstream& input) : input(input){
	current_row= 0, current_column= 0;
}

char Lexer::GetNextChar(){
	char t;
	if (input.get(t)){
		(t == '\n') ? current_column++ : current_row++, current_column= 0;
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
		lexeme+= current_char;
		current_char= GetNextChar();
		if (!current_char){
			err<<"Failed to close sequence "<<lexeme;
			DisplayError(err, current_row, current_column);
		}
	}while(func(current_char));
	if (unget)
		input.unget();
}


Symbol& Lexer::ResolveToken(){
	while (1){
		current_char= GetNextChar(), lexeme= "";
		switch (current_char) {
			case '.':
			case 0:
				std::cout<<"END OF FILE"<<std::endl;
				return Symbols::T_EOF;
			case ';': RETURN_TOKEN(Symbols::T_EOE);
			case '+': RETURN_TOKEN(Symbols::T_OP_ADD);
			case '-': RETURN_TOKEN(Symbols::T_OP_SUB);
			case ')': RETURN_TOKEN(Symbols::T_CLOSE_PAR);
			case '(':
				if (GetNextChar() == '*'){
					ConsumeSequence([this](char character){
						return (character == '*' && GetNextChar() == ')') ? false : (input.unget(),true);});
					continue;
				}
				input.unget();
			RETURN_TOKEN(Symbols::T_OPEN_PAR);
			case '\'':
				ConsumeSequence(//When it finds ' then break out of loop and save that char to lexeme
					[this](char character){return (character == '\'') ? (lexeme+= character), false: true;}
				);
				return Symbols::T_STR_LIT;
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
