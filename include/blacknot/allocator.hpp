//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================
#pragma once
//======================================================================
//======================================================================

namespace Blacknot {

//======================================================================
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

	virtual void * alloc (SizeType size) = 0;
	virtual void * realloc (void * ptr, SizeType old_size, SizeType new_size) = 0;
	virtual void free (void * ptr, SizeType size) = 0;
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
	virtual void * alloc (SizeType size) override;
	virtual void * realloc (void * ptr, SizeType old_size, SizeType new_size) override;
	virtual void free (void * ptr, SizeType size) override;

private:
	SizeType m_size_cap = 0;
	SizeType m_current_size = 0;
};

//======================================================================
//----------------------------------------------------------------------

class HeapAllocator
	: public Allocator
{
public:
	explicit HeapAllocator (Allocator * upstream, SizeType maximum_size);
	~HeapAllocator ();

	virtual char const * name () const override;
	virtual SizeType capacity () const override;
	virtual SizeType allocated () const override;
	virtual SizeType unallocated () const override;
	virtual void * alloc (SizeType size) override;
	virtual void * realloc (void * ptr, SizeType old_size, SizeType new_size) override;
	virtual void free (void * ptr, SizeType size) override;

private:
	Allocator * m_upstream;
	SizeType m_size_cap = 0;
	SizeType m_current_size = 0;
};

//======================================================================

}	// namespace Blacknot

//======================================================================
