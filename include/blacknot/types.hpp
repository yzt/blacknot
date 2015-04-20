//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================
#pragma once
//======================================================================

#include <blacknot/config.hpp>
#include <blacknot/assert.hpp>

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

BKNT_STATIC_ASSERT_SIZE ( I8, 1);
BKNT_STATIC_ASSERT_SIZE ( U8, 1);
BKNT_STATIC_ASSERT_SIZE (I16, 2);
BKNT_STATIC_ASSERT_SIZE (U16, 2);
BKNT_STATIC_ASSERT_SIZE (I32, 4);
BKNT_STATIC_ASSERT_SIZE (U32, 4);
BKNT_STATIC_ASSERT_SIZE (I64, 8);
BKNT_STATIC_ASSERT_SIZE (U64, 8);
BKNT_STATIC_ASSERT_SIZE (F32, 4);
BKNT_STATIC_ASSERT_SIZE (F64, 8);

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

}	// namespace Blacknot

//======================================================================
