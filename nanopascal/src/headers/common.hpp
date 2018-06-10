#ifndef COMMON_HPP
#define COMMON_HPP
#include <string>
#include <iostream>
#include <sstream>
void DisplayError(std::ostringstream&, int, int);
extern std::ostringstream err;

template<typename iterable, typename key>
bool ExistsInIterable(iterable iter, key looking_for){
	return (iter.find(looking_for) != iter.end());
}
#endif
