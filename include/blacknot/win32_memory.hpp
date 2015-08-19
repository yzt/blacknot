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
	~VirtualMemoryArea ();

	void * memory () {return m_memory;}
	void const * memory () const {return m_memory;}
	U64 reserved () const {return m_reserved_size;}
	U64 committed () const {return m_committed_size;}
	bool empty () const {return 0 == committed();}
	bool full () const {return committed() >= reserved();}

	// Returns new size
	U64 resize (U64 new_commit_size);

private:
	void * m_memory = nullptr;
	U64 m_reserved_size = 0;
	U64 m_committed_size = 0;
};

//======================================================================

	}	// namespace Platform
}	// namespace Blacknot

//======================================================================
