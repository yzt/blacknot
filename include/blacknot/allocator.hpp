//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================
#pragma once
//======================================================================

#define BKNT_DBG_ALLOC_PARAMS(ud)	{__FILE__, __LINE__, (ud)}
#define BKNT_DBGP(ud)				BKNT_DBG_ALLOC_PARAMS(ud)

//======================================================================

namespace Blacknot {

//======================================================================

struct AllocDebugParams
{
	char const * file;
	//char const * func;
	int line;
	int user_data;
};

//----------------------------------------------------------------------

class Allocator
{
public:
	using SizeType = unsigned long long;

public:
	virtual char const * name () const = 0;
	virtual SizeType capacity () const = 0;
	virtual SizeType allocated () const = 0;
	virtual SizeType unallocated () const = 0;

	virtual void * alloc (SizeType size, AllocDebugParams const & dbg_params) = 0;
	virtual void * realloc (void * ptr, SizeType old_size, SizeType new_size, AllocDebugParams const & dbg_params) = 0;
	virtual void free (void * ptr, SizeType size, AllocDebugParams const & dbg_params) = 0;

	// Convenience interface for dealing with non-PODs
	template <typename T, typename... ArgTypes>
	inline T * create (AllocDebugParams const & dbg_params, ArgTypes &&... args);

	template <typename T>
	inline void destroy (T * p, AllocDebugParams const & dbg_params);

	template <typename T, typename... ArgTypes>
	inline T * createArray (size_t n, AllocDebugParams const & dbg_params, ArgTypes &&... args);
	
	template <typename T>
	inline void destroyArray (T * p, size_t n, AllocDebugParams const & dbg_params);
};

//======================================================================
//======================================================================
//----------------------------------------------------------------------

class RootAllocator
	: public Allocator
{
public:
	explicit RootAllocator (SizeType maximum_size);
	~RootAllocator ();

	virtual char const * name () const override;
	virtual SizeType capacity () const override;
	virtual SizeType allocated () const override;
	virtual SizeType unallocated () const override;
	virtual void * alloc (SizeType size, AllocDebugParams const & dbg_params) override;
	virtual void * realloc (void * ptr, SizeType old_size, SizeType new_size, AllocDebugParams const & dbg_params) override;
	virtual void free (void * ptr, SizeType size, AllocDebugParams const & dbg_params) override;

private:
	SizeType m_size_cap = 0;
	SizeType m_current_size = 0;
};

//======================================================================
//----------------------------------------------------------------------
// Forwarding allocator: forwards all alloc/free requests to its upstream allocator.
class FwdAllocator
	: public Allocator
{
public:
	explicit FwdAllocator (Allocator * upstream, SizeType maximum_size);
	~FwdAllocator ();

	virtual char const * name () const override;
	virtual SizeType capacity () const override;
	virtual SizeType allocated () const override;
	virtual SizeType unallocated () const override;
	virtual void * alloc (SizeType size, AllocDebugParams const & dbg_params) override;
	virtual void * realloc (void * ptr, SizeType old_size, SizeType new_size, AllocDebugParams const & dbg_params) override;
	virtual void free (void * ptr, SizeType size, AllocDebugParams const & dbg_params) override;

private:
	Allocator * m_upstream;
	SizeType m_size_cap = 0;
	SizeType m_current_size = 0;
};

//======================================================================
//----------------------------------------------------------------------

class DebuggingAllocator
	: public Allocator
{
	static SizeType const msc_SentinelValue = 0xDEAD42424242C0DE;

	struct Header
	{
		Header * next;
		Header * prev;
		AllocDebugParams dbg_params;
		SizeType block_size;
		SizeType sentinel;
	};

	struct Footer
	{
		SizeType sentinel;
	};

public:
	explicit DebuggingAllocator (Allocator * upstream, SizeType maximum_size);
	~DebuggingAllocator ();

	virtual char const * name () const override;
	virtual SizeType capacity () const override;
	virtual SizeType allocated () const override;
	virtual SizeType unallocated () const override;
	virtual void * alloc (SizeType size, AllocDebugParams const & dbg_params) override;
	virtual void * realloc (void * ptr, SizeType old_size, SizeType new_size, AllocDebugParams const & dbg_params) override;
	virtual void free (void * ptr, SizeType size, AllocDebugParams const & dbg_params) override;

	// Returns the number of still-allocated blocks
	int report () const;

private:
	void clear ();

	SizeType calcActualSize (SizeType req_size) const
	{
		return req_size + sizeof(Header) + sizeof(Footer);
	}

	void * userArea (void * block) const
	{
		return (void *)((char *)block + sizeof(Header));
	}

	Header * header (void * user_block) const
	{
		return (Header *)((char *)user_block - sizeof(Header));
	}

	Footer * footer (void * user_block, SizeType user_size) const
	{
		return (Footer *)((char *)user_block + user_size);
	}

private:
	Allocator * m_upstream = nullptr;
	SizeType m_size_cap = 0;
	SizeType m_current_size = 0;
	Header * m_first = nullptr;
};

//======================================================================

}	// namespace Blacknot

//======================================================================

#define BKNT__ALLOCATOR_DO_THE_IMPLEMENTATION
#include <blacknot/allocator.inline.hpp>
#undef  BKNT__ALLOCATOR_DO_THE_IMPLEMENTATION

//======================================================================
