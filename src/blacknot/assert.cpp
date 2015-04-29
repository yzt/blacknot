//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================

#include <blacknot/assert.hpp>

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#if defined(BKNT_COMPILER_IS_VC)
	#include <intrin.h>
#endif

//======================================================================

namespace Blacknot {

//======================================================================

void DebugBreak ()
{
#if defined(BKNT_COMPILER_IS_VC)
	__debugbreak ();
#else
	__asm__ volatile ("int $0x03");
#endif
}

//----------------------------------------------------------------------

bool Assert (
	bool cond, char const * cond_str,
	char const * file, int line, char const * func,
	char const * fmt /*= nullptr*/, ...
)
{
	if (cond)
		return true;

#if defined(BKNT_ENABLE_ASSERT_LOG_TO_STDERR) || \
	defined(BKNT_ENABLE_ASSERT_LOG_TO_FILE) || \
	defined(BKNT_ENABLE_ASSERT_LOG_TO_MSGBOX)

	int const BufferSize = 4000;
	char buffer [BufferSize];

	int used = 0;
	used += snprintf (buffer + used, BufferSize - used, "ASSERTION FAILURE: %s in %s:%d\n\t%s\n"
		, func, file, line, cond_str);

	if (nullptr != fmt)
	{
		used += snprintf (buffer + used, BufferSize - used, "\n");
		va_list args;
		va_start (args, fmt);
		used += vsnprintf (buffer + used, BufferSize - used, fmt, args);
		used += snprintf (buffer + used, BufferSize - used, "\n");
	}
#endif

#if defined(BKNT_ENABLE_ASSERT_LOG_TO_STDERR)
	fputs (buffer, stderr);
	fflush (stderr);
#endif

#if defined(BKNT_ENABLE_ASSERT_LOG_TO_FILE)
	#error Not implemented yet...
#endif

#if defined(BKNT_ENABLE_ASSERT_LOG_TO_MSGBOX)
	#error Not implemented yet...
#endif

	DebugBreak ();

	exit (0);
	//__assume (0);
	//return false;
}

//----------------------------------------------------------------------
//======================================================================

}	// namespace Blacknot

//======================================================================
