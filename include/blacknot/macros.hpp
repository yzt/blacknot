//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================
#pragma once
//======================================================================

#define BKNT_CONSUME_ARG(x)		((void)x)

//----------------------------------------------------------------------

#define BKNT__STRINGIZE_DO(x)	#x
#define BKNT_STRINGIZE(x)		BKNT__STRINGIZE_DO(x)

//----------------------------------------------------------------------

#define BKNT_PTR_VALID(ptr)				(nullptr != (ptr))

//----------------------------------------------------------------------

#if defined(BKNT_COMPILER_IS_VC)
	#define BKNT_PLACEMENT_NEW(ptr, type, ...)	new (ptr) type (__VA_ARGS__)
#else
	#define BKNT_PLACEMENT_NEW(ptr, type, ...)	new (ptr) type ( ## __VA_ARGS__)
#endif

//----------------------------------------------------------------------

// TODO: Check whether "alignas(t)" is enough...
#define BKNT_ALIGN_AS_TYPE(t)			alignas(alignof(t))

//----------------------------------------------------------------------

#if defined(BKNT_COMPILER_IS_VC)
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
