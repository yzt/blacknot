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
#include <cstring>

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
		BKNT_REPORT ("[ERROR] The root allocator is not empty upon destruction (remaining: %llu.)\n", m_current_size);
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

void * RootAllocator::alloc (SizeType size, AllocDebugParams const & /*dbg_params*/)
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

void * RootAllocator::realloc (void * ptr, SizeType old_size, SizeType new_size, AllocDebugParams const & /*dbg_params*/)
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

void RootAllocator::free (void * ptr, SizeType size, AllocDebugParams const & /*dbg_params*/)
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

FwdAllocator::FwdAllocator (Allocator * upstream, SizeType maximum_size)
	: m_upstream (upstream)
	, m_size_cap (Min(maximum_size, upstream->capacity()))
	, m_current_size (0)
{
	BKNT_ASSERT_PTR_VALID (upstream);
}

//----------------------------------------------------------------------

FwdAllocator::~FwdAllocator ()
{
	BKNT_ASSERT (0 == allocated(), "Allocator \"%s\" is not empty upon destruction (%llu bytes remain.)", name(), allocated());
}

//----------------------------------------------------------------------

char const * FwdAllocator::name () const
{
	return "HeapAllocator";
}

//----------------------------------------------------------------------

Allocator::SizeType FwdAllocator::capacity () const
{
	return m_size_cap;
}

//----------------------------------------------------------------------

Allocator::SizeType FwdAllocator::allocated () const
{
	return m_current_size;
}

//----------------------------------------------------------------------

Allocator::SizeType FwdAllocator::unallocated () const
{
	return m_size_cap - m_current_size;
}

//----------------------------------------------------------------------

void * FwdAllocator::alloc (SizeType size, AllocDebugParams const & dbg_params)
{
	if (m_current_size + size > m_size_cap)
	{
		BKNT_REPORT ("[WARN] Failing allocation due to max size limit violation.\n");
		return nullptr;
	}

	void * ret = m_upstream->alloc (size, dbg_params);

	if (BKNT_PTR_VALID(ret))
	{
		m_current_size += size;
	}

	return ret;
}

//----------------------------------------------------------------------

void * FwdAllocator::realloc (void * ptr, SizeType old_size, SizeType new_size, AllocDebugParams const & dbg_params)
{
	if (m_current_size + new_size - old_size > m_size_cap)
	{
		BKNT_REPORT ("[WARN] Failing allocation due to max size limit violation.\n");
		return nullptr;
	}

	// TODO: Find out the exact semantics of realloc
	void * ret = m_upstream->realloc (ptr, old_size, new_size, dbg_params);

	if (BKNT_PTR_VALID(ret))
	{
		m_current_size += new_size;
		m_current_size -= old_size;
	}

	return ret;
}

//----------------------------------------------------------------------

void FwdAllocator::free (void * ptr, SizeType size, AllocDebugParams const & dbg_params)
{
	BKNT_ASSERT (size <= m_current_size);
	if (BKNT_PTR_VALID(ptr))
	{
		m_upstream->free (ptr, size, dbg_params);
		m_current_size -= size;
	}
}

//----------------------------------------------------------------------
//======================================================================

DebuggingAllocator::DebuggingAllocator (Allocator * upstream, SizeType maximum_size)
	: m_upstream (upstream)
	, m_size_cap (Min(maximum_size, upstream->capacity()))
	, m_current_size (0)
{
	BKNT_ASSERT_PTR_VALID (upstream);
}

//----------------------------------------------------------------------

DebuggingAllocator::~DebuggingAllocator ()
{
	report ();
	BKNT_ASSERT_STRONG (m_current_size == 0, "Still %llu bytes remain unfreed.", m_current_size);
	clear ();
}

//----------------------------------------------------------------------

char const * DebuggingAllocator::name () const
{
	return "DebuggingAllocator";
}

//----------------------------------------------------------------------

Allocator::SizeType DebuggingAllocator::capacity () const
{
	return m_size_cap;
}

//----------------------------------------------------------------------

Allocator::SizeType DebuggingAllocator::allocated () const
{
	return m_current_size;
}

//----------------------------------------------------------------------

Allocator::SizeType DebuggingAllocator::unallocated () const
{
	return m_size_cap - m_current_size;
}

//----------------------------------------------------------------------

void * DebuggingAllocator::alloc (SizeType req_size, AllocDebugParams const & dbg_params)
{
	SizeType act_size = calcActualSize (req_size);

	if (m_current_size + act_size > m_size_cap)
	{
		BKNT_REPORT ("[WARN] Failing allocation due to max size limit violation.\n");
		return nullptr;
	}

	void * block = m_upstream->alloc (act_size, dbg_params);

	if (!BKNT_PTR_VALID(block))
		return nullptr;

	m_current_size += act_size;

	auto ret = userArea (block);
	auto hdr = header (ret);
	auto ftr = footer (ret, req_size);

	hdr->next = m_first;
	hdr->prev = nullptr;
	hdr->dbg_params = dbg_params;
	hdr->block_size = req_size;
	hdr->sentinel = msc_SentinelValue;
	ftr->sentinel = msc_SentinelValue;

	m_first = hdr;
	if (BKNT_PTR_VALID(hdr->next))
		hdr->next->prev = hdr;

	return ret;
}

//----------------------------------------------------------------------

void * DebuggingAllocator::realloc (void * ptr, SizeType old_size, SizeType new_size, AllocDebugParams const & dbg_params)
{
	auto p = alloc (new_size, dbg_params);
	if (!BKNT_PTR_VALID(p))
		return nullptr;

	if (BKNT_PTR_VALID(ptr))
	{
		::memcpy (p, ptr, old_size);
		free (ptr, old_size, dbg_params);
	}

	return p;
}

//----------------------------------------------------------------------

void DebuggingAllocator::free (void * ptr, SizeType size, AllocDebugParams const & dbg_params)
{
	if (!BKNT_PTR_VALID(ptr))
		return;

	BKNT_ASSERT_STRONG (size <= m_current_size);

	auto hdr = header (ptr);
	auto ftr = footer (ptr, size);
	
	BKNT_ASSERT_STRONG (msc_SentinelValue == hdr->sentinel, "Buffer underflow, or invalid pointer passed to free().");
	BKNT_ASSERT_STRONG (size == hdr->block_size, "Buffer size mismatch, expected size %llu, given %llu.", hdr->block_size, size);
	BKNT_ASSERT_STRONG (msc_SentinelValue == ftr->sentinel, "Buffer overflow, or invalid pointer passed to free().");

	if (BKNT_PTR_VALID(hdr->next))
		hdr->next->prev = hdr->prev;
	if (BKNT_PTR_VALID(hdr->prev))
		hdr->prev->next = hdr->next;
	if (m_first == hdr)
		m_first = hdr->next;

	hdr->sentinel = ~msc_SentinelValue;
	ftr->sentinel = ~msc_SentinelValue;

	auto act_size = calcActualSize(size);
	m_upstream->free (hdr, act_size, dbg_params);
	m_current_size -= act_size;
}

//----------------------------------------------------------------------

int DebuggingAllocator::report () const
{
	if (!BKNT_PTR_VALID(m_first))
		return 0;

	BKNT_REPORT ("List of memory leaks for allocator \"%s\":\n", name());
	int cnt = 0;
	for (auto p = m_first; BKNT_PTR_VALID(p); p = p->next, ++cnt)
	{
		BKNT_REPORT ("  Block at 0x%p: size = %llu, user data = %d, allocated from %s:%d.\n"
			, userArea(p), p->block_size, p->dbg_params.user_data
			, p->dbg_params.file, p->dbg_params.line
		);
	}
	BKNT_REPORT ("\n");

	return cnt;
}

//----------------------------------------------------------------------

void DebuggingAllocator::clear ()
{
	auto p = m_first;
	while (BKNT_PTR_VALID(p))
	{
		auto q = p->next;
		m_upstream->free (p, calcActualSize(p->block_size), BKNT_DBG_ALLOC_PARAMS(-1));
		p = q;
	}
}

//======================================================================

}	// namespace Blacknot

//======================================================================
