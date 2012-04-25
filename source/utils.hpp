#ifndef UTILS_HPP
#define UTILS_HPP


#include <iostream>
#include <string>
using namespace std;


typedef unsigned int  uint;
typedef unsigned char uchar;


inline uint ndigits ( uint n ) {
	uint out = 1;
	for ( ; n > 9; n /= 10 ) out++;
	return out;
}

string print_sym( uint n ) {
	stringstream s;
	s << n;
	return s.str();
}

string print_state( uint n ) {
	stringstream s;
	s << "S" << n;
	return s.str();
}

string print_dir ( bool d ) {
	return d ? "R" : "L";
}


#endif // UTILS_HPP
