#ifndef VECT_HPP
#define VECT_HPP

#include <iostream>
#include <assert.h>
#include <algorithm>
#include <initializer_list>
using namespace std;

#ifdef _WIN32
typedef unsigned int uint;
#endif





template<class T = double, uint N = 2>
class vect {
	T values [N];
public:
	typedef T CType;		// component type
	static const uint dims = N;	// vector size (number of dimensions)

	// initializes the vector in the origin
	vect ( const T value = 0 ) {
		fill( values, values + N, value );
	}

	// initializes vector with random components uniformly drawn from a specific range
	vect ( Random& gen, const vect& minvalues = vect(-32), const vect& maxvalues = vect(32) ) {
		for ( uint i = 0; i < N; ++i )
			values[i] = ( maxvalues[i] - minvalues[i] ) * gen.real() + minvalues[i];
	}

	// copy constructor
	vect ( const vect& v ) {
		copy( v.values, v.values + N, values );
	}

#ifdef __GXX_EXPERIMENTAL_CXX0X__
	// initializes from a list (only works with c++0x)
	vect ( std::initializer_list<T> v ) {
		assert( v.size() == sizeof( values ) / sizeof( T ) );
		copy( v.begin(), v.end(), values );
	}
#endif

	T* bits ( ) {
		return values;
	}

	// returns a reference to the i-th component. I avoid checks for more speed
	T& operator[] ( uint i ) {
		return values[i];
	}

	T operator[] ( uint i ) const {
		return values[i];
	}

	// assign v to *this
	vect& operator= ( const vect& v ) {
		copy( v.values, v.values + N, values );
		return *this;
	}

	// subtract v from *this
	vect& operator-= ( const vect& v ) {
		for ( uint i = 0; i < N; ++i )
			values[i] -= v.values[i];
		return *this;
	}

	// returns the difference vector (*this - v)
	vect operator- ( const vect& v ) const {
		return vect(*this) -= v;
	}

	// unary - (changes sign to the components)
	vect operator- ( ) const {
		return vect() -= (*this);
	}

	vect& operator+= ( const vect& v ) {
		for ( uint i = 0; i < N; ++i )
			values[i] += v.values[i];
		return *this;
	}

	vect operator+ ( const vect& v ) const {
		return vect(*this) += v;
	}

	// euclidean norm
	T norm ( ) const {
		/*T out = 0.0;
		for ( uint i = 0; i < N; ++i )
			out += values[i] * values[i];*/

		return sqrt( (*this) * (*this) );
	}

	// dot product of two vectors
	T operator* ( const vect& v ) const {
		//return inner_product( values, values + N, v.values, 0 );
		T out = 0;
		for ( uint i = 0; i < N; ++i )
			out += values[i] * v[i];

		return out;
	}

	// multiplication by a scalar
	vect& operator*= ( const T& s ) {
		for ( uint i = 0; i < N; ++i )
			values[i] *= s;
		return *this;
	}

	friend vect operator* ( T s, const vect& v ) {
		return vect(v) *= s;
	}

	friend ostream& operator<< ( ostream & os, const vect& v ) {
		os << "[";
		//os.precision( 2 );

		for ( uint i = 0; i < N - 1; ++i )
			os << v.values[i] << ", ";

		return os << v.values[N-1] << "]";
	}
};

#endif // VECT_HPP
