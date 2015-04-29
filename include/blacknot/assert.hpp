//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================
#pragma once
//======================================================================

#include <blacknot/config.hpp>
#include <blacknot/macros.hpp>

//======================================================================

#define BKNT_STATIC_ASSERT(cond, msg)			static_assert ((cond), msg)
#define BKNT_STATIC_ASSERT_SIZE(t, sz)			static_assert (sizeof(t) == (sz), "Type " BKNT_STRINGIZE(t) " must be " BKNT_STRINGIZE(sz) " bytes.")

//----------------------------------------------------------------------

#if defined(BKNT_ENABLE_FULL_ASSERTS)
	#if defined(BKNT_COMPILER_IS_VC)
		#define BKNT_ASSERT(cond, ...)					do{if (!(cond))::Blacknot::Assert(false,BKNT_STRINGIZE(cond),__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__);}while(false)
		#define BKNT_ASSERT_REPAIR(cond, ...)			(::Blacknot::Assert(cond,BKNT_STRINGIZE(cond),__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__))
		#define BKNT_ASSERT_PTR_VALID(ptr, ...)			BKNT_ASSERT(BKNT_PTR_VALID(ptr),__VA_ARGS__)
	#else
		#define BKNT_ASSERT(cond, ...)					do{if (!(cond))::Blacknot::Assert(false,BKNT_STRINGIZE(cond),__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__);}while(false)
		#define BKNT_ASSERT_REPAIR(cond, ...)			(::Blacknot::Assert(cond,BKNT_STRINGIZE(cond),__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__))
		#define BKNT_ASSERT_PTR_VALID(ptr, ...)			BKNT_ASSERT(BKNT_PTR_VALID(ptr),##__VA_ARGS__)
	#endif
#else
	#define BKNT_ASSERT(cond, ...)					/**/
	#define BKNT_ASSERT_REPAIR(cond, ...)			(cond)
	#define BKNT_ASSERT_PTR_VALID(ptr, ...)			/**/
#endif

#if defined(BKNT_COMPILER_IS_VC)
	#define BKNT_ASSERT_STRONG(cond, ...)			do{if (!(cond))::Blacknot::Assert(false,BKNT_STRINGIZE(cond),__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__);}while(false)
	#define BKNT_ASSERT_PTR_VALID_STRONG(ptr, ...)	BKNT_ASSERT_STRONG(BKNT_PTR_VALID(ptr),__VA_ARGS__)
#else
	#define BKNT_ASSERT_STRONG(cond, ...)			do{if (!(cond))::Blacknot::Assert(false,BKNT_STRINGIZE(cond),__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__);}while(false)
	#define BKNT_ASSERT_PTR_VALID_STRONG(ptr, ...)	BKNT_ASSERT_STRONG(BKNT_PTR_VALID(ptr),##__VA_ARGS__)
#endif

//----------------------------------------------------------------------
//======================================================================

namespace Blacknot {

//======================================================================

void DebugBreak ();

//----------------------------------------------------------------------

bool Assert (
	bool cond, char const * cond_str,
	char const * file, int line, char const * func,
	char const * fmt = nullptr, ...
);

//----------------------------------------------------------------------
//======================================================================

}	// namespace Blacknot

//======================================================================
