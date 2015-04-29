//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================
#pragma once
//======================================================================

#include <blacknot/config.hpp>

//======================================================================

namespace Blacknot {

//======================================================================

using  I8 = char;
using  U8 = unsigned char;
using I16 = short;
using U16 = unsigned short;
using I32 = int;
using U32 = unsigned int;
using I64 = long long;
using U64 = unsigned long long;

using F32 = float;
using F64 = double;

using Byte = U8;

//----------------------------------------------------------------------
//======================================================================

template <typename T>
inline T const & Min (T const & a, T const & b)
{
	if (b < a) return b;
	else return a;
}

//----------------------------------------------------------------------

template <typename T>
inline T const & Max (T const & a, T const & b)
{
	if (b < a) return a;
	else return b;
}

//======================================================================

struct NonCopyable
{
	NonCopyable () = default;
	NonCopyable (NonCopyable const &) = delete;
	NonCopyable & operator = (NonCopyable const &) = delete;
};

struct NonMovable
{
	NonMovable () = default;
	NonMovable (NonMovable &&) = delete;
	NonMovable & operator = (NonMovable &&) = delete;
};

struct NonMovableNonCopyable
{
	NonMovableNonCopyable () = default;
	NonMovableNonCopyable (NonMovableNonCopyable const &) = delete;
	NonMovableNonCopyable (NonMovableNonCopyable &&) = delete;
	NonMovableNonCopyable & operator = (NonMovableNonCopyable const &) = delete;
	NonMovableNonCopyable & operator = (NonMovableNonCopyable &&) = delete;
};

//----------------------------------------------------------------------
//======================================================================

}	// namespace Blacknot

//======================================================================
