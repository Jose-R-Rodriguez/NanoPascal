#include <fstream>
#include <iomanip>
#include "common.hpp"
#include "lexer.hpp"
int main(int argc, char const *argv[]) {
	std::ifstream input_file(argv[argc-1]);
	Lexer mylexer(input_file);
	Symbol& token= mylexer.ResolveToken();
	std::cout<<"ID"<<std::setw(25)<<"Tokens"<<std::setw(25)<<"Symbol\n";
	do {
		std::cout<<token<<std::setw(25)<<mylexer.GetCurrentLexeme()<<'\n';
		token= mylexer.ResolveToken();
	} while(token.id != Symbols::T_EOF.id);

	input_file.close();
	return 0;
}
