#include "common.hpp"

std::ostringstream err(std::ostringstream::ate);
void DisplayError(std::ostringstream& error_message){
	std::cout<<error_message.str()<<std::endl;
	exit(1);
}
