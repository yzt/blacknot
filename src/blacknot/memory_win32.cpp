//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================

#include <blacknot/memory.hpp>

//======================================================================

#include <cstring>		// memcpy()
#include <Windows.h>

//======================================================================

namespace Blacknot {
	namespace Platform {

//======================================================================

U32 VirtualMemoryArea::GetPageSize ()
{
	::SYSTEM_INFO si;
	::GetSystemInfo (&si);
	return si.dwPageSize;
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------

VirtualMemoryArea::VirtualMemoryArea (U64 reserve_size)
{
	U32 const page_size = GetPageSize();
	m_reserved_size = (reserve_size + page_size - 1) / page_size * page_size;
	
	m_memory = ::VirtualAlloc (nullptr, m_reserved_size, MEM_RESERVE, PAGE_NOACCESS);
	BKNT_ASSERT_PTR_VALID (m_memory, "VirtualAlloc() for %llu bytes failed: %u.", m_reserved_size, unsigned(::GetLastError()));
}

//----------------------------------------------------------------------

VirtualMemoryArea::VirtualMemoryArea (VirtualMemoryArea const & that)
	: m_memory (nullptr)
	, m_reserved_size (that.m_reserved_size)
{
	m_memory = ::VirtualAlloc (nullptr, m_reserved_size, MEM_RESERVE, PAGE_NOACCESS);
	BKNT_ASSERT_PTR_VALID (m_memory, "VirtualAlloc() for %llu bytes failed: %u.", m_reserved_size, unsigned(::GetLastError()));

	if (BKNT_PTR_VALID(m_memory))
	{
		resize (that.m_committed_size);
		BKNT_ASSERT (m_committed_size == that.m_committed_size);
		::memcpy (m_memory, that.m_memory, m_committed_size);
	}
}

//----------------------------------------------------------------------

VirtualMemoryArea::~VirtualMemoryArea ()
{
	destroy ();
}

//----------------------------------------------------------------------

//VirtualMemoryArea & VirtualMemoryArea::operator = (VirtualMemoryArea const & that)
//{
//	if (this != &that)
//	{
//		destroy ();
//
//		m_reserved_size = that.m_reserved_size;
//
//		m_memory = ::VirtualAlloc (nullptr, m_reserved_size, MEM_RESERVE, PAGE_NOACCESS);
//		BKNT_ASSERT_PTR_VALID (m_memory, "VirtualAlloc() for %llu bytes failed: %u.", m_reserved_size, unsigned(::GetLastError()));
//
//		if (BKNT_PTR_VALID(m_memory))
//		{
//			resize (that.m_committed_size);
//			BKNT_ASSERT (m_committed_size == that.m_committed_size);
//			::memcpy (m_memory, that.m_memory, m_committed_size);
//		}
//	}
//
//	return *this;
//}

//----------------------------------------------------------------------

U64 VirtualMemoryArea::resize (U64 new_commit_size)
{
	if (!BKNT_PTR_VALID(m_memory))
		return m_committed_size;

	U32 const page_size = GetPageSize();

	// Round up to the nearest multiple of PageSize...
	U64 newer_commit_size = (new_commit_size + page_size - 1) / page_size * page_size;

	if (newer_commit_size < m_committed_size)	// We need to decommit...
	{
		//BKNT_ASSERT ((m_committed_size - newer_commit_size) % page_size == 0);
		::VirtualFree (((Byte *)m_memory) + newer_commit_size, m_committed_size - newer_commit_size, MEM_DECOMMIT);
		m_committed_size = newer_commit_size; 
	}
	else if (newer_commit_size > m_committed_size)	// We need to commit...
	{
		//BKNT_ASSERT ((newer_commit_size - m_committed_size) % page_size == 0);
		::VirtualAlloc (((Byte *)m_memory) + m_committed_size, newer_commit_size - m_committed_size, MEM_COMMIT, PAGE_READWRITE);
		m_committed_size = newer_commit_size;
	}

	return m_committed_size;
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------

void VirtualMemoryArea::destroy ()
{
	if (BKNT_PTR_VALID(m_memory))
		::VirtualFree (m_memory, 0, MEM_RELEASE);
	m_memory = nullptr;
	m_reserved_size = 0;
	m_committed_size = 0;
}

//======================================================================

	}	// namespace Platform
}	// namespace Blacknot

//======================================================================
