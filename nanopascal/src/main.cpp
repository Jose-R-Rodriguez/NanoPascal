#include <fstream>
#include <iomanip>
#include "common.hpp"
#include "lexer.hpp"

void LexerTester(Lexer& mylexer){
	Symbol& token= mylexer.ResolveToken();
	std::cout<<"ID"<<std::setw(25)<<"Tokens"<<std::setw(25)<<"Symbol\n";
	do {
		std::cout<<token<<std::setw(25)<<mylexer.GetCurrentLexeme()<<'\n';
		token= mylexer.ResolveToken();
	} while(token.id != Symbols::T_EOF.id);
	std::cout<<token<<std::setw(25)<<mylexer.GetCurrentLexeme()<<'\n';
}

int main(int argc, char const *argv[]) {
	int x;
	while (argc>2){
		std::cout<<"Choose file to test with"<<std::endl;
		int i;
		for(i= 1; i<argc; i++){
			if (std::string(argv[i]) == "-d")
				break;
			std::cout<<i<<"."<<argv[i]<<std::endl;
		}

		std::cin>>x;
		std::ifstream input_file(argv[x]);
		Lexer mylexer(input_file);
		for (i++ ; i<argc ; i++){
			std::cout<<"Adding directive: "<<argv[i]<<std::endl;
			mylexer.AddDirective(argv[i]);
		}
		LexerTester(mylexer);
		input_file.close();
	}
	if (argc<=2){
		std::ifstream input_file(argv[argc-1]);
		Lexer mylexer(input_file);
		LexerTester(mylexer);
		input_file.close();
}

	return 0;
}
