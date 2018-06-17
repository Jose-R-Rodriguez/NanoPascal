#include "parser.hpp"
Parser::Parser(Lexer& mylexer) : mylexer(mylexer){
	Parse();
}
void Parser::Parse(){
  Start();
  if (current_token == TK_EOF){
    std::cout<<"Syntax ...ok"<<std::endl;
  }
  else{
    std::cout<<"Extra Token: '"<<Token_Names[current_token]<<"' LINE# "<<mylexer.get_current_line()<<std::endl;
  }
}
