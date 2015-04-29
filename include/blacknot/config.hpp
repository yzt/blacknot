//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================
#pragma once
//======================================================================

#if defined(_MSC_VER)
	#define _CRT_SECURE_NO_WARNINGS
	#pragma warning (disable: 4127)	// "conditional expression is constant"
	#pragma warning (disable: 4324)	// "structure was padded due to __declspec(align())"

	#define BKNT_COMPILER_IS_VC	1
#endif

//======================================================================

#if !defined(NDEBUG) //|| defined(_DEBUG)
	#define BKNT_ENABLE_FULL_ASSERTS
#endif

#define BKNT_ENABLE_ASSERT_LOG_TO_STDERR	1
//#define BKNT_ENABLE_ASSERT_LOG_TO_FILE		1
//#define BKNT_ENABLE_ASSERT_LOG_TO_MSGBOX	1

//======================================================================
//----------------------------------------------------------------------
//======================================================================
//======================================================================
