#include <fstream>
class Lexer{
public:
	char current_char;
	Lexer(std::ifstream& input);
	unsigned int ResolveToken();

private:
	int current_column, current_row;
	std::ifstream& input;
	char GetNextChar();
};
