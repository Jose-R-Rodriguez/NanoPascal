#include "lexer.hpp"
#define RETURN_TOKEN(tk)\
	do{lexeme= current_char;\
	return tk;}while(0)

Lexer::Lexer(std::ifstream& input) : input(input){
	current_row= 1, current_column= 1;
}

std::set<std::string> Lexer::declared_directives={
	{"nanopascal"}
};

char Lexer::GetNextChar(bool case_sensitive= false){
	char t;
	if (input.get(t)){
		(t == '\n') ? (current_row++, current_column= 1) : current_column++;
		return (case_sensitive) ? t : tolower(t);
	}
	else
		return 0;
}
void Lexer::LookAheadOne(char t){
	int placeholder;
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
void Lexer::PrintActiveDirectives(){
	std::stack<directive_structure> temp;
	temp= active_directives;
	while(!temp.empty()){
		std::cout<<std::get<0>(temp.top())<<std::get<1>(temp.top())<<std::endl;
		temp.pop();
	}
}

bool Lexer::PeekAndCompare(char t){
	if (input.peek()==t){
		lexeme= lexeme+current_char+ GetNextChar();
		current_char= GetNextChar();
		return true;
	}
	else
		return false;
}

Symbol& Lexer::ResolveToken(){
	return GetNextToken();
}
Symbol& Lexer::ProcessDirective(){
	Symbol& directive_action= Keywords.at(lexeme);
	if(directive_action.id == Symbols::Pre_If_Def.id){
		Symbol& next_token= GetNextToken();
		GetNextChar();
		if (next_token.id == Symbols::T_ID.id){
			if (ExistsInIterable(declared_directives, lexeme)){
				//if the identifier that came in exists in our declared directives
				//then push that we haven't found an else and it is active
				directive_structure temp= {lexeme, false, true};
				active_directives.push(temp);
				return ResolveToken();
			}
			else{
				//if the identifier can't be found then push it and we havent found an else and its
				//not active
				directive_structure temp= {lexeme, false, false};
				active_directives.push(temp);
				bool found_else;
				do{
					std::cout<<"--------------"<<std::endl;
					PrintActiveDirectives();
					//Else is resolving the endif token, therefore removing it from the top of the stack
					//even though that else belonged to him and not someone else
					next_token= ResolveToken();
					found_else= std::get<1>(active_directives.top());
				}while (!found_else);
				return next_token;
			}
		}
		else{
			err<<"Invalid Identifier in Directive: \""<<lexeme<<"\"";
			DisplayError(err, current_row, current_column);
		}
	}
	else if(directive_action.id == Symbols::Pre_Else.id){
		directive_structure changing= active_directives.top();
		active_directives.pop();
		if (std::get<1>(changing)){
			err<<"Extra else for directive";
			DisplayError(err, current_row, current_column);
		}
		else{
			std::get<1>(changing)= true;
			active_directives.push(changing);
			return ResolveToken();
		}
	}
	else if(directive_action.id == Symbols::Pre_End_If.id){
		active_directives.pop();
		return ResolveToken();
	}
}

Symbol& Lexer::EvaluateDirective(){
	ConsumeSequence(
		[this](char character){return (character != '}') && (character != ' ') ? true : (lexeme+= character,false);}
	);
	if (!ExistsInIterable(Keywords, lexeme)){
		err<<"Unknown Directive Command: \""<<lexeme<<"\"";
		DisplayError(err, current_row, current_column);
	}
	return ProcessDirective();
}

Symbol& Lexer::GetNextToken(){
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
			case '=': RETURN_TOKEN(Symbols::T_EQUALS_TO);
			case '}': RETURN_TOKEN(Symbols::T_CLOSE_CURLY);
			case '$':
					ConsumeSequence(isxdigit, true);
					return Symbols::T_Hex_Num;
			case '>':
				if(PeekAndCompare('='))
					return Symbols::T_GT_OR_ET;
				RETURN_TOKEN(Symbols::T_GREATER_THAN);
			case ':':
				if (PeekAndCompare('='))
					return Symbols::T_EQUALS;
				RETURN_TOKEN(Symbols::T_SEMICOLON);
			case '<':
				if(PeekAndCompare('='))
					return Symbols::T_LT_OR_ET;
				else if(PeekAndCompare('>'))
					return Symbols::T_NOT_ET;
				RETURN_TOKEN(Symbols::T_LESS_THAN);
			case '{':
				if(PeekAndCompare('$')){
					return EvaluateDirective();
				}
				else{
					ConsumeSequence([this](char character){return !(character == '}');});
				}continue;
				RETURN_TOKEN(Symbols::T_OPEN_CURLY);
			case '(':
				if (PeekAndCompare('*')){
					ConsumeSequence([this](char character){
						return (PeekAndCompare(')') && character == '*') ? false : true;});
					std::cout<<lexeme<<std::endl;
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
					ConsumeSequence([this](char character){return (character!='\n');});
					continue;
				}
			case '\n':
			case ' ': continue;
			default:
				if (isdigit(current_char)){
					ConsumeSequence(isdigit, true);
					return Symbols::T_NUM;
				}
				else if(isalpha(current_char) || current_char == '_'){
					ConsumeSequence(
						[](char alpha){return isalpha(alpha) || alpha == '_';}, true);
					return (ExistsInIterable(Keywords, lexeme)) ? Keywords.at(lexeme) : Symbols::T_ID;
				}
				else{
					err<<"Unknown Token: "<<current_char;
					DisplayError(err, current_row, current_column);
				}
		}
	}
}
