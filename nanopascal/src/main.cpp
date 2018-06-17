#include <fstream>
#include <iomanip>
#include "common.hpp"
#include "lexer.hpp"
#include "parser.hpp"

void LexerTester(Lexer& mylexer){
	Symbol* token;
	do{
		token= &mylexer.ResolveToken();
		std::cout<<*token<<std::setw(25)<<mylexer.GetCurrentLexeme()<<std::endl;
	}while(*token != Symbols::T_EOF);
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
		//LexerTester(mylexer);
		Parser myparser(mylexer);
		input_file.close();
	}
	if (argc<=2){
		std::ifstream input_file(argv[argc-1]);
		Lexer mylexer(input_file);
		//LexerTester(mylexer);
		Parser myparser(mylexer);
		input_file.close();
}

	return 0;
}
