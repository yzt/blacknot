//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================

#include <blacknot/allocator.hpp>
#include <blacknot/macros.hpp>
#include <blacknot/types.hpp>
#include <blacknot/assert.hpp>

#include <cstdio>	// implicitly, via BKNT_REPORT
#include <cstdlib>

//======================================================================

namespace Blacknot {

//======================================================================

RootAllocator::RootAllocator (SizeType maximum_size)
	: m_size_cap (maximum_size)
{
}

//----------------------------------------------------------------------

RootAllocator::~RootAllocator ()
{
	if (0 != m_current_size)
	{
		BKNT_REPORT ("[ERROR] The root allocator is not empty upon destruction...\n");
		DebugBreak ();
	}
}

//----------------------------------------------------------------------

char const * RootAllocator::name () const
{
	return "RootAllocator";
}

//----------------------------------------------------------------------

Allocator::SizeType RootAllocator::capacity () const
{
	return m_size_cap;
}

//----------------------------------------------------------------------

Allocator::SizeType RootAllocator::allocated () const
{
	return m_current_size;
}

//----------------------------------------------------------------------

Allocator::SizeType RootAllocator::unallocated () const
{
	return m_size_cap - m_current_size;
}

//----------------------------------------------------------------------

void * RootAllocator::alloc (SizeType size)
{
	if (m_current_size + size > m_size_cap)
	{
		BKNT_REPORT ("[WARN] Failing allocation due to max size limit violation.\n");
		return nullptr;
	}

	void * ret = ::malloc (size);

	if (BKNT_PTR_VALID(ret))
	{
		m_current_size += size;
	}

	return ret;
}

//----------------------------------------------------------------------

void * RootAllocator::realloc (void * ptr, SizeType old_size, SizeType new_size)
{
	if (m_current_size + new_size - old_size > m_size_cap)
	{
		BKNT_REPORT ("[WARN] Failing allocation due to max size limit violation.\n");
		return nullptr;
	}

	// TODO: Find out the exact semantics of realloc
	void * ret = ::realloc (ptr, new_size);

	if (BKNT_PTR_VALID(ret))
	{
		m_current_size += new_size;
		m_current_size -= old_size;
	}

	return ret;
}

//----------------------------------------------------------------------

void RootAllocator::free (void * ptr, SizeType size)
{
	BKNT_ASSERT (size <= m_current_size);
	if (BKNT_PTR_VALID(ptr))
	{
		::free (ptr);
		m_current_size -= size;
	}
}

//----------------------------------------------------------------------
//======================================================================
//----------------------------------------------------------------------

HeapAllocator::HeapAllocator (Allocator * upstream, SizeType maximum_size)
	: m_upstream (upstream)
	, m_size_cap (Min(maximum_size, upstream->capacity()))
	, m_current_size (0)
{
	BKNT_ASSERT_PTR_VALID (upstream);
}

//----------------------------------------------------------------------

HeapAllocator::~HeapAllocator ()
{
	BKNT_ASSERT (0 == m_current_size, "Allocator \"%s\" is not empty upon destruction (%llu bytes remain.)", name(), currentSize());
}

//----------------------------------------------------------------------

char const * HeapAllocator::name () const
{
	return "HeapAllocator";
}

//----------------------------------------------------------------------

Allocator::SizeType HeapAllocator::capacity () const
{
	return m_size_cap;
}

//----------------------------------------------------------------------

Allocator::SizeType HeapAllocator::allocated () const
{
	return m_current_size;
}

//----------------------------------------------------------------------

Allocator::SizeType HeapAllocator::unallocated () const
{
	return m_size_cap - m_current_size;
}

//----------------------------------------------------------------------

void * HeapAllocator::alloc (SizeType size)
{
	if (m_current_size + size > m_size_cap)
	{
		BKNT_REPORT ("[WARN] Failing allocation due to max size limit violation.\n");
		return nullptr;
	}

	void * ret = m_upstream->alloc (size);

	if (BKNT_PTR_VALID(ret))
	{
		m_current_size += size;
	}

	return ret;
}

//----------------------------------------------------------------------

void * HeapAllocator::realloc (void * ptr, SizeType old_size, SizeType new_size)
{
	if (m_current_size + new_size - old_size > m_size_cap)
	{
		BKNT_REPORT ("[WARN] Failing allocation due to max size limit violation.\n");
		return nullptr;
	}

	// TODO: Find out the exact semantics of realloc
	void * ret = m_upstream->realloc (ptr, old_size, new_size);

	if (BKNT_PTR_VALID(ret))
	{
		m_current_size += new_size;
		m_current_size -= old_size;
	}

	return ret;
}

//----------------------------------------------------------------------

void HeapAllocator::free (void * ptr, SizeType size)
{
	BKNT_ASSERT (size <= m_current_size);
	if (BKNT_PTR_VALID(ptr))
	{
		m_upstream->free (ptr, size);
		m_current_size -= size;
	}
}

//----------------------------------------------------------------------
//======================================================================

}	// namespace Blacknot

//======================================================================
