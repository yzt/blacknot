//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================
#pragma once
//======================================================================

#include <blacknot/types.hpp>
#include <blacknot/assert.hpp>
#include <blacknot/memory.hpp>

//======================================================================

namespace Blacknot {

//======================================================================
//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
class CappedArray
{
public:
	using MyType = CappedArray<Type_, Capacity_>;
	using ValueType = Type_;
	using SizeType = U16;
	using Iterator = ValueType *;
	using ConstIterator = ValueType const *;
	static SizeType const Capacity = Capacity_;

public:
	inline CappedArray ();
	explicit inline CappedArray (SizeType size);
	inline CappedArray (SizeType size, ValueType const & fill);
	template <typename ConstIterType>
	inline CappedArray (ConstIterType begin_, ConstIterType end_);
	template <typename OtherType, U16 OtherCapacity>
	inline CappedArray (CappedArray<OtherType, OtherCapacity> const & that);
	inline CappedArray (MyType && that);

	inline ~CappedArray ();

	template <typename OtherType, U16 OtherCapacity>
	inline MyType & operator = (CappedArray<OtherType, OtherCapacity> const & that);
	inline MyType & operator = (MyType && that);

	inline ValueType const & operator [] (SizeType idx) const;
	inline ValueType & operator [] (SizeType idx);

	inline SizeType capacity () const;
	inline SizeType size () const;
	inline bool empty () const;
	inline bool full () const;

	inline void clear ();

	inline Iterator begin ();
	inline ConstIterator begin () const;
	inline ConstIterator cbegin () const;

	inline Iterator end ();
	inline ConstIterator end () const;
	inline ConstIterator cend () const;

	inline ValueType const & front () const;
	inline ValueType & front ();
	inline ValueType const & back () const;
	inline ValueType & back ();

	inline bool push_back ();
	inline bool push_back (ValueType const & value);
	inline bool push_back (ValueType && value);

	inline void pop_back ();

private:
	inline ValueType const * data () const;
	inline ValueType * data ();

private:
	SizeType m_size = 0;
	BKNT_ALIGN_AS_TYPE(ValueType) char m_data [Capacity * sizeof(ValueType)];

	BKNT_STATIC_ASSERT (Capacity_ > 0, "Capacity cannot be 0.");
};

//======================================================================

template <typename Type_>
class VirtualVector
{
public:
	using MyType = VirtualVector<Type_>;
	using ValueType = Type_;
	using SizeType = U64;
	using Iterator = ValueType *;
	using ConstIterator = ValueType const *;

public:
	explicit inline VirtualVector (SizeType capacity);
	explicit inline VirtualVector (SizeType capacity, SizeType size);
	inline VirtualVector (SizeType capacity, SizeType size, ValueType const & fill);
	inline VirtualVector (SizeType capacity, ConstIterator data, SizeType count);
	template <typename OtherType>
	inline VirtualVector (VirtualVector<OtherType> const & that);
	inline VirtualVector (MyType && that);

	inline ~VirtualVector ();

	template <typename OtherType>
	inline MyType & operator = (VirtualVector<OtherType> const & that) = delete;
	inline MyType & operator = (MyType && that) = delete;

	inline ValueType const & operator [] (SizeType idx) const;
	inline ValueType & operator [] (SizeType idx);

	inline SizeType capacity () const;
	inline SizeType size () const;
	inline bool empty () const;
	inline bool full () const;

	inline void clear ();

	inline Iterator begin ();
	inline ConstIterator begin () const;
	inline ConstIterator cbegin () const;

	inline Iterator end ();
	inline ConstIterator end () const;
	inline ConstIterator cend () const;

	inline ValueType const & front () const;
	inline ValueType & front ();
	inline ValueType const & back () const;
	inline ValueType & back ();

	inline bool push_back ();
	inline bool push_back (ValueType const & value);
	inline bool push_back (ValueType && value);

	inline void pop_back ();

private:
	inline ValueType const * data () const;
	inline ValueType * data ();
	inline ValueType const * datum (SizeType idx) const;
	inline ValueType * datum (SizeType idx);
	inline void resize_to_n_elements (SizeType n);

private:
	SizeType m_size;
	SizeType m_capacity;
	Platform::VirtualMemoryArea m_memory;
};

//======================================================================

}	// namespace Blacknot

//======================================================================

#define BKNT__ARRAY_DO_THE_IMPLEMENTATION
#include <blacknot/array.inline.hpp>
#undef  BKNT__ARRAY_DO_THE_IMPLEMENTATION

//======================================================================
