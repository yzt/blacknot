//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================

#include <blacknot/types.hpp>
#include <blacknot/assert.hpp>

//======================================================================

namespace Blacknot {

//======================================================================

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
BKNT_STATIC_ASSERT_SIZE (Byte, 1);

//----------------------------------------------------------------------
//======================================================================

}	// namespace Blacknot

//======================================================================
