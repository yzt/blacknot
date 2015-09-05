//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================
#pragma once
//======================================================================
#include <blacknot/common.hpp>
//======================================================================

namespace Blacknot {
	namespace Platform {

//======================================================================
//----------------------------------------------------------------------

class VirtualMemoryArea
{
public:
	static U32 GetPageSize ();

public:
	explicit VirtualMemoryArea (U64 reserve_size);
	VirtualMemoryArea (VirtualMemoryArea const &);
	inline VirtualMemoryArea (VirtualMemoryArea && that);
	~VirtualMemoryArea ();

	VirtualMemoryArea & operator = (VirtualMemoryArea const & that) = delete;
	inline VirtualMemoryArea & operator = (VirtualMemoryArea && that) = delete;

	void * memory () {return m_memory;}
	void const * memory () const {return m_memory;}
	U64 capacity () const {return m_reserved_size;}
	U64 size () const {return m_committed_size;}
	bool empty () const {return 0 == size();}
	bool full () const {return size() >= capacity();}

	// Returns new size
	U64 resize (U64 new_commit_size);

private:
	void destroy ();

private:
	void * m_memory = nullptr;
	U64 m_reserved_size = 0;
	U64 m_committed_size = 0;
};

//======================================================================
//======================================================================
// Inline function implementatins:
//======================================================================

inline VirtualMemoryArea::VirtualMemoryArea (VirtualMemoryArea && that)
	: m_memory (that.m_memory)
	, m_reserved_size (that.m_reserved_size)
	, m_committed_size (that.m_committed_size)
{
	that.m_memory = nullptr;
	that.m_reserved_size = 0;
	that.m_committed_size = 0;
}

//----------------------------------------------------------------------

//inline VirtualMemoryArea & VirtualMemoryArea::operator = (VirtualMemoryArea && that)
//{
//	if (this != &that)
//	{
//		destroy ();
//
//		m_memory = that.m_memory;
//		m_reserved_size = that.m_reserved_size;
//		m_committed_size = that.m_committed_size;
//
//		that.m_memory = nullptr;
//		that.m_reserved_size = 0;
//		that.m_committed_size = 0;
//	}
//
//	return *this;
//}

//----------------------------------------------------------------------
//======================================================================

	}	// namespace Platform
}	// namespace Blacknot

//======================================================================
