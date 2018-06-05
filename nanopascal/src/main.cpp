#include <iostream>
#include <fstream>
#include "lexer.hpp"
int main(int argc, char const *argv[]) {
	std::ifstream input_file(argv[argc-1]);
	Lexer mylexer(input_file);
	Symbol& token= mylexer.ResolveToken();
	std::cout<<"ID\t\tSymbol\t\tTokens\n";
	do {
		std::cout<<token.id<<"\t\t"<<token.current_lexeme<<"\t\t"<<token.name<<'\n';
		token= mylexer.ResolveToken();
	} while(token.id != Symbols::T_EOF.id);

	input_file.close();
	return 0;
}
