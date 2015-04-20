//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================
#pragma once
//======================================================================

#include <cassert>	// temporary...

//======================================================================

#define BKNT_STATIC_ASSERT(cond, msg)			static_assert ((cond), msg)
#define BKNT_STATIC_ASSERT_SIZE(t, sz)			static_assert (sizeof(t) == (sz), "Type " BKNT_STRINGIZE(t) " must be " BKNT_STRINGIZE(sz) " bytes.")

#define BKNT_ASSERT(cond, ...)					assert (cond)
#define BKNT_ASSERT_STRONG(cond, ...)			assert (cond)
#define BKNT_ASSERT_PTR_VALID(ptr, ...)			assert (BKNT_PTR_VALID(ptr))
#define BKNT_ASSERT_PTR_VALID_STRONG(ptr, ...)	assert (BKNT_PTR_VALID(ptr))
#define BKNT_ASSERT_REPAIR(cond, ...)			(cond)

//----------------------------------------------------------------------
//======================================================================

namespace Blacknot {

//======================================================================
//----------------------------------------------------------------------
//======================================================================

}	// namespace Blacknot

//======================================================================
