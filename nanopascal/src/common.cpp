#include "common.hpp"

std::ostringstream err(std::ostringstream::ate);
void DisplayError(std::ostringstream& error_message, int row, int column){
	std::cout<<error_message.str()<<std::endl<<"Error: Line: "<<column<<"Row: "<<row<<std::endl;
	exit(1);
}
