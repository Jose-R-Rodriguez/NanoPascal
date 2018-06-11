#include "lexer.hpp"
#define RETURN_TOKEN(tk)\
	do{lexeme= current_char;\
	return tk;}while(0)

Lexer::Lexer(std::ifstream& input) : input(input){
	current_row= 1, current_column= 1;
}


char Lexer::GetNextChar(bool case_sensitive= false){
	char t;
	if (input.get(t)){
		(t == '\n') ? (current_row++, current_column= 1) : current_column++;
		return (case_sensitive) ? t : tolower(t);
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

Symbol& Lexer::DoIfDef(){
	Symbol& next_token= GetNextToken();
	GetNextChar();
	if (next_token.id == Symbols::T_ID.id){
		if (ExistsInIterable(declared_directives, lexeme)){
			//name, has an else, activated by else
			directive_structure add_to_stack= {lexeme, false, false};
			active_directives.push(add_to_stack);
			return ResolveToken();
		}
		else{
			//rewrite this block
			//name, has an else, activated by else
			directive_structure add_to_stack= {lexeme, false, true};
			active_directives.push(add_to_stack);
			bool& found_else= std::get<1>(add_to_stack);
			int current_stack_size, orig_stack_size= active_directives.size();
			Symbol *temp;
			do {
				current_stack_size= active_directives.size();
				temp= &ResolveToken();
			} while(!std::get<1>(add_to_stack) && current_stack_size >= orig_stack_size);
			return *temp;
		}
	}
	else{
 			err<<"Invalid Identifier in Directive: \""<<lexeme<<"\"";
 			DisplayError(err, current_row, current_column);
 		}
	std::cout<<"Fatal error"<<std::endl;
	exit(99);
}

Symbol& Lexer::DoElse(){
	directive_structure top_of_stack= active_directives.top();
	if(!std::get<1>(top_of_stack)){
		active_directives.pop();
		std::get<1>(top_of_stack)= true;
		active_directives.push(top_of_stack);
	}
	if (!std::get<2>(top_of_stack)){
		int current_stack_size, orig_stack_size= active_directives.size();
		Symbol* temp;
		do {
			temp= &ResolveToken();
			current_stack_size= active_directives.size();
		} while(current_stack_size >= orig_stack_size);
		return *temp;
	}
	else{
		return ResolveToken();
	}
	err<<"Extra else in directive";
	DisplayError(err, current_row, current_column);
	exit(1);
}

Symbol& Lexer::DoEndIf(){
	active_directives.pop();
	return ResolveToken();
}

Symbol& Lexer::ProcessDirective(){
	Symbol& directive_action= Keywords.at(lexeme);
	if(directive_action.id == Symbols::Pre_If_Def.id){
		return DoIfDef();
	}
	else if(directive_action.id == Symbols::Pre_Else.id){
		return DoElse();
	}
	else if(directive_action.id == Symbols::Pre_End_If.id){
		return DoEndIf();
	}
	else{
		std::cout<<"Panic critical failure"<<std::endl;
		exit(1);
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
