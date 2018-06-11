#include <fstream>
#include <iomanip>
#include "common.hpp"
#include "lexer.hpp"
void LexerTester(std::string file_name){
	std::ifstream input_file(file_name);
	Lexer mylexer(input_file);
	Symbol& token= mylexer.ResolveToken();
	std::cout<<"ID"<<std::setw(25)<<"Tokens"<<std::setw(25)<<"Symbol\n";
	do {
		std::cout<<token<<std::setw(25)<<mylexer.GetCurrentLexeme()<<'\n';
		token= mylexer.ResolveToken();
	} while(token.id != Symbols::T_EOF.id);
	std::cout<<token<<std::setw(25)<<mylexer.GetCurrentLexeme()<<'\n';

	input_file.close();
}

int main(int argc, char const *argv[]) {
	int x;
	while (argc>2){
		std::cout<<"Choose file to test with"<<std::endl;
		for(int i= 1; i<argc; i++){
			std::cout<<i<<"."<<argv[i]<<std::endl;
		}
		std::cin>>x;
		LexerTester(argv[x]);
	}
	if (argc<=2)
		LexerTester(argv[argc-1]);
	return 0;
}
