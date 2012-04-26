#ifndef UTILS_HPP
#define UTILS_HPP


#include <iostream>
#include <string>
#include <sstream>
#include <boost/integer_traits.hpp>
using namespace boost;
using namespace std;

typedef unsigned int  uint;
typedef unsigned char uchar;


// to make it more fast I simply check with constants
static const uint ndigits10 ( uint n ) {
	if ( n < 10 )		return 1;
	else if ( n < 100 )	return 2;
	else if ( n < 1000 )	return 3;
	else if ( n < 10000 )	return 4;
	else			return 5;
}







template<class T>
class typeholder {
protected:
	T _value;
public:
	typeholder( ) { _value = 0; }
	typeholder( const T& value ) : _value( value ) { }

	T& operator++ ( ) { return ++_value; }

	// this overloads the cast operator
	// and should allow to read the value (inside the class) as usual
	operator T() const { return _value; }
};






// A definition of a symbol lies normally inside a certain set of possible states
// that here is assumed to be from 0 to NStates. This allows to test for example
// if a state is an halting state, or simply print it as "H".
// If I don't have such information I set the number of states to the max
// for that type. But here a problem arises: how to find such max, at compile time?

// I cannot use numeric_limits<T>::max() because is a non-const expression (so cannot
// be evaluated at compile time). But boost solves the problem:
// http://www.boost.org/doc/libs/1_41_0/libs/integer/integer_traits.html
template<class T, T NStates = integer_traits<T>::const_max>
class state : public typeholder<T> {
public:
	state( ) : typeholder<T> ( ) { }
	state( const T& value ) : typeholder<T>( value ) { }

	bool ishalt ( ) const { return this->_value == NStates; }
	bool isrunning ( ) const { return this->_value != NStates; }

	static const uint maxwidth ( ) {
		return ndigits10( NStates - 1 ) + 1;
	}

	friend ostream& operator<< ( ostream& os, const state& a ) {
		stringstream tmp;
		if ( a.ishalt() )	tmp << left << "H";
		else			tmp << "S" << (uint) a._value;
		return os << tmp.str();
	}
};







// same thing for the NSymbols, but actually here I use this constant
// only for printing reasons
template<class T, T NSymbols = integer_traits<T>::const_max>
class symbol : public typeholder<T> {
public:
	symbol( ) : typeholder<T> ( ) { }
	symbol( const T& value ) : typeholder<T>( value ) { }

	static const uint maxwidth ( ) {
		return ndigits10( NSymbols - 1 );
	}

	friend ostream& operator<< ( ostream& os, const symbol& s ) {
		return (os << (uint) s._value);
	}
};






class direction : public typeholder<bool> {
public:
	direction( ) : typeholder<bool> ( ) { }
	direction( const bool& value ) : typeholder<bool>( value ) { }

	bool isleft ( ) const { return !_value; }
	bool isright ( ) const { return _value; }

	static const uint maxwidth ( ) {
		return 1;
	}

	friend ostream& operator<< ( ostream& os, const direction& d ) {
		return (os << ( d._value ? "R" : "L" ));
	}
};

#endif // UTILS_HPP
