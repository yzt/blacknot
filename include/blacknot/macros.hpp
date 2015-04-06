//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================
#pragma once
//======================================================================
//----------------------------------------------------------------------

#define BKNT__STRINGIZE_DO(x)	#x
#define BKNT_STRINGIZE(x)		BKNT__STRINGIZE_DO(x)

//----------------------------------------------------------------------

#define BKNT_PTR_VALID(ptr)				(nullptr != (ptr))

//----------------------------------------------------------------------

#define BKNT_STATIC_ASSERT(cond, msg)			static_assert ((cond), msg)
#define BKNT_STATIC_ASSERT_SIZE(t, sz)			static_assert (sizeof(t) == (sz), "Type " BKNT_STRINGIZE(t) " must be " BKNT_STRINGIZE(sz) " bytes.")

#define BKNT_ASSERT(cond, ...)					assert (cond)
#define BKNT_ASSERT_STRONG(cond, ...)			assert (cond)
#define BKNT_ASSERT_PTR_VALID(ptr, ...)			assert (BKNT_PTR_VALID(ptr))
#define BKNT_ASSERT_PTR_VALID_STRONG(ptr, ...)	assert (BKNT_PTR_VALID(ptr))
#define BKNT_ASSERT_REPAIR(cond, ...)			(cond)

//----------------------------------------------------------------------

#if defined(_MSC_VER)
	#define BKNT_REPORT(fmt, ...)	printf(fmt, __VA_ARGS__)
#else
	#define BKNT_REPORT(fmt, ...)	printf(fmt, ## __VA_ARGS__)
#endif

//======================================================================

namespace Blacknot {

//======================================================================
//----------------------------------------------------------------------
//======================================================================

}	// namespace Blacknot

//======================================================================
