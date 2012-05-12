#ifndef LIMITS_HPP
#define LIMITS_HPP

#include <iostream>
using namespace std;


// In this like I would like to put all the informations we have on the
// already known busy beavers

// default: no information
template<uint NStates, uint NSymbols>
struct slimits {
  const static uint upper = 0;
  const static uint lower = 0;
  const static bool has_upper = false;
  const static bool has_lower = false;
};

// specializations for the known cases
template<>
struct slimits<2,2> {
  const static uint upper = 6;
  const static uint lower = 0;
  const static bool has_upper = true;
  const static bool has_lower = false;
};

template<>
struct slimits<3,2> {
  const static uint upper = 21;
  const static uint lower = 0;
  const static bool has_upper = true;
  const static bool has_lower = false;
};

template<>
struct slimits<4,2> {
  const static uint upper = 107;
  const static uint lower = 0;
  const static bool has_upper = true;
  const static bool has_lower = false;
};

template<>
struct slimits<5,2> {
  const static uint lower = 47176870;
  const static uint upper = 0;
  const static bool has_upper = false;
  const static bool has_lower = true;
};

template<>
struct slimits<2,3> {
  const static uint upper = 38;
  const static uint lower = 0;
  const static bool has_upper = true;
  const static bool has_lower = false;
};

template<>
struct slimits<2,4> {
  const static uint lower = 3932964;
  const static uint upper = 0;
  const static bool has_upper = false;
  const static bool has_lower = true;
};

template<>
struct slimits<3,3> {
  constexpr static double lower = 1.19112334170342540e+17;
  const static uint upper = 0;
  const static bool has_upper = false;
  const static bool has_lower = true;
};


#endif // LIMITS_HPP
