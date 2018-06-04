#include <iostream>
#include <fstream>
#include "lexer.hpp"
int main(int argc, char const *argv[]) {
	std::ifstream input_file(argv[argc-1]);
	Lexer mylexer(input_file);
	input_file.close();
	return 0;
}
