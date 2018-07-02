#include <fstream>
#include <iomanip>
#include "common.hpp"
#include "lexer.hpp"
#include "parser.hpp"

int main(int argc, char const *argv[]) {
	std::ifstream input_file(argv[argc-1]);
	Lexer mylexer(input_file);

	Parser myparser(mylexer);
	AST myast= myparser.Parse();
	myast.Interpret();
	//myast.Print();
	input_file.close();

	return 0;
}
