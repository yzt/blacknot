//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================

#include <blacknot/string_util.hpp>

#include <cstdlib>

//======================================================================

namespace Blacknot {

//======================================================================

template <> int FromString<int> (char const * s) {return atoi(s);}
template <> bool FromString<bool> (char const * s) {return 0 != FromString<int>(s);}
template <> double FromString<double> (char const * s) {return atof(s);}
template <> char const * FromString<char const *> (char const * s) {return s;}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Explicit instantiations:
template int FromString<int> (char const *);
template bool FromString<bool> (char const *);
template double FromString<double> (char const *);
template char const * FromString<char const *> (char const *);

//======================================================================

}	// namespace Blacknot

//======================================================================
