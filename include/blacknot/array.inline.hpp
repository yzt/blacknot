//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================
#pragma once
//======================================================================
#if !defined(BKNT__ARRAY_DO_THE_IMPLEMENTATION)
	#error "This file is not for direct inclusion."
#endif
//======================================================================

#include <blacknot/array.hpp>

#include <blacknot/assert.hpp>
#include <blacknot/macros.hpp>
#include <blacknot/types.hpp>

#include <new>
#include <utility>	// TODO: Implement std::move yourself and remove this dependency

//======================================================================

namespace Blacknot {

//======================================================================
//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline CappedArray<Type_, Capacity_>::CappedArray ()
	: m_size (0)
{
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline CappedArray<Type_, Capacity_>::CappedArray (SizeType size)
	: m_size (Min(Capacity, size))
{
	for (SizeType i = 0; i < m_size; ++i)
		BKNT_PLACEMENT_NEW(data() + i) ValueType;
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline CappedArray<Type_, Capacity_>::CappedArray (SizeType size, ValueType const & fill)
	: m_size (Min(Capacity, size))
{
	for (SizeType i = 0; i < m_size; ++i)
		BKNT_PLACEMENT_NEW(data() + i) ValueType (fill);
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
template <typename ConstIterType>
inline CappedArray<Type_, Capacity_>::CappedArray (ConstIterType begin_, ConstIterType end_)
	: m_size (0)
{
	SizeType cnt = 0;
	for (; begin_ != end_ && cnt < Capacity; ++begin_, ++cnt)
		BKNT_PLACEMENT_NEW(data() + cnt) ValueType (*begin_);
	m_size = cnt;
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
template <typename OtherType, U16 OtherCapacity>
inline CappedArray<Type_, Capacity_>::CappedArray (CappedArray<OtherType, OtherCapacity> const & that)
	: m_size (Min(Capacity, that.size()))
{
	for (SizeType i = 0; i < m_size; ++i)
		BKNT_PLACEMENT_NEW(data() + i) ValueType (that[i]);
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline CappedArray<Type_, Capacity_>::CappedArray (MyType && that)
	: m_size (that.size())
{
	for (SizeType i = 0; i < m_size; ++i)
		BKNT_PLACEMENT_NEW(data() + i) ValueType (std::move(that[i]));
	that.clear ();
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline CappedArray<Type_, Capacity_>::~CappedArray ()
{
	clear ();
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
template <typename OtherType, U16 OtherCapacity>
inline auto CappedArray<Type_, Capacity_>::operator = (CappedArray<OtherType, OtherCapacity> const & that) -> MyType & 
{
	if (this != &that)
	{
		clear ();
		m_size = Min(Capacity, that.size());
		for (SizeType i = 0; i < m_size; ++i)
			BKNT_PLACEMENT_NEW(data() + i) ValueType (that[i]);
	}
	return *this;
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline auto CappedArray<Type_, Capacity_>::operator = (MyType && that) -> MyType & 
{
	if (this != &that)
	{
		clear ();
		m_size = that.size();
		for (SizeType i = 0; i < m_size; ++i)
			BKNT_PLACEMENT_NEW(data() + i) ValueType (std::move(that[i]));
		that.clear ();
	}
	return *this;
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline auto CappedArray<Type_, Capacity_>::operator [] (SizeType idx) const -> ValueType const &
{
	BKNT_ASSERT (idx < m_size, "Array index out of range: %d vs. %d.", int(idx), int(m_size));
	return data()[idx];
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline auto CappedArray<Type_, Capacity_>::operator [] (SizeType idx) -> ValueType &
{
	BKNT_ASSERT (idx < m_size, "Array index out of range: %d vs. %d.", int(idx), int(m_size));
	return data()[idx];
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline auto CappedArray<Type_, Capacity_>::capacity () const -> SizeType
{
	return Capacity;
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline auto CappedArray<Type_, Capacity_>::size () const -> SizeType
{
	return m_size;
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline bool CappedArray<Type_, Capacity_>::empty () const
{
	return m_size == 0;
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline bool CappedArray<Type_, Capacity_>::full () const
{
	return m_size >= Capacity;
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline void CappedArray<Type_, Capacity_>::clear ()
{
	for (SizeType i = m_size; i > 0; --i)
		(data() + i - 1)->~ValueType();
	m_size = 0;
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline auto CappedArray<Type_, Capacity_>::begin () -> Iterator
{
	return data();
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline auto CappedArray<Type_, Capacity_>::begin () const -> ConstIterator
{
	return data();
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline auto CappedArray<Type_, Capacity_>::cbegin () const -> ConstIterator
{
	return data();
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline auto CappedArray<Type_, Capacity_>::end () -> Iterator
{
	return data() + m_size;
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline auto CappedArray<Type_, Capacity_>::end () const -> ConstIterator
{
	return data() + m_size;
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline auto CappedArray<Type_, Capacity_>::cend () const -> ConstIterator
{
	return data() + m_size;
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline auto CappedArray<Type_, Capacity_>::front () const -> ValueType const &
{
	BKNT_ASSERT (!empty(), "Array is empty!");
	return *data();
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline auto CappedArray<Type_, Capacity_>::front () -> ValueType &
{
	BKNT_ASSERT (!empty(), "Array is empty!");
	return *data();
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline auto CappedArray<Type_, Capacity_>::back () const -> ValueType const &
{
	BKNT_ASSERT (!empty(), "Array is empty!");
	return *(data() + m_size - 1);
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline auto CappedArray<Type_, Capacity_>::back () -> ValueType &
{
	BKNT_ASSERT (!empty(), "Array is empty!");
	return *(data() + m_size - 1);
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline bool CappedArray<Type_, Capacity_>::push_back ()
{
	if (!full())
	{
		BKNT_PLACEMENT_NEW(data() + m_size) ValueType;
		m_size += 1;
		return true;
	}
	else
		return false;
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline bool CappedArray<Type_, Capacity_>::push_back (ValueType const & value)
{
	if (!full())
	{
		BKNT_PLACEMENT_NEW(data() + m_size) ValueType (value);
		m_size += 1;
		return true;
	}
	else
		return false;
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline bool CappedArray<Type_, Capacity_>::push_back (ValueType && value)
{
	if (!full())
	{
		BKNT_PLACEMENT_NEW(data() + m_size) ValueType (std::move(value));
		m_size += 1;
		return true;
	}
	else
		return false;
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline void CappedArray<Type_, Capacity_>::pop_back ()
{
	if (!empty())
	{
		m_size -= 1;
		(data() + m_size)->~ValueType();
	}
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline auto CappedArray<Type_, Capacity_>::data () const -> ValueType const *
{
	return reinterpret_cast<ValueType const *>(m_data);
}

//----------------------------------------------------------------------

template <typename Type_, U16 Capacity_>
inline auto CappedArray<Type_, Capacity_>::data () -> ValueType *
{
	return reinterpret_cast<ValueType *>(m_data);
}

//----------------------------------------------------------------------
//======================================================================

}	// namespace Blacknot

//======================================================================
