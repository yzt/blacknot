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

template <typename Type_>
inline VirtualVector<Type_>::VirtualVector (SizeType capacity)
	: m_size (0)
	, m_capacity (capacity)
	, m_memory (capacity * sizeof(ValueType))
{
}

//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::VirtualVector (SizeType capacity, SizeType size)
	: m_size (Min(capacity, size))
	, m_capacity (capacity)
	, m_memory (capacity * sizeof(ValueType))
{
	resize_to_n_elements (m_size);
	for (SizeType i = 0; i < m_size; ++i)
		BKNT_PLACEMENT_NEW(datum(i), ValueType);
}

//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::VirtualVector (SizeType capacity, SizeType size, ValueType const & fill)
	: m_size (Min(capacity, size))
	, m_capacity (capacity)
	, m_memory (capacity * sizeof(ValueType))
{
	resize_to_n_elements (m_size);
	for (SizeType i = 0; i < m_size; ++i)
		BKNT_PLACEMENT_NEW(datum(i), ValueType, fill);
}

//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::VirtualVector (SizeType capacity, ConstIterator data, SizeType count)
	: m_size (Min(capacity, count))
	, m_capacity (capacity)
	, m_memory (capacity * sizeof(ValueType))
{
	resize_to_n_elements (m_size);
	for (SizeType i = 0; i < m_size; ++i)
		BKNT_PLACEMENT_NEW(datum(i), ValueType, data[i]);
}

//----------------------------------------------------------------------

template <typename Type_>
template <typename OtherType>
inline VirtualVector<Type_>::VirtualVector (VirtualVector<OtherType> const & that)
	: m_size (that.m_size)
	, m_capacity (that.m_capacity)
	, m_memory (that.m_capacity * sizeof(ValueType))
{
	resize_to_n_elements (m_size);
	
	ValueType * p = data();
	VirtualVector<OtherType>::ValueType const * q = that.data();
	for (SizeType i = 0, n = m_size; i < n; ++i, ++p, ++q)
		BKNT_PLACEMENT_NEW (p, ValueType, *q);
}

//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::VirtualVector (MyType && that)
	: m_size (that.m_size)
	, m_capacity (that.m_capacity)
	, m_memory (std::move(that.m_memory))
{
	that.m_size = 0;
	that.m_capacity = 0;
}

//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::~VirtualVector ()
{
	clear ();
}

//----------------------------------------------------------------------

//template <typename Type_>
//template <typename OtherType>
//inline VirtualVector<Type_>::MyType & VirtualVector<Type_>::operator = (VirtualVector<OtherType> const & that)
//{
//}
//
////----------------------------------------------------------------------
//
//template <typename Type_>
//inline VirtualVector<Type_>::MyType & VirtualVector<Type_>::operator = (MyType && that)
//{
//}

//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::ValueType const & VirtualVector<Type_>::operator [] (SizeType idx) const
{
	return *datum(idx);
}

//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::ValueType & VirtualVector<Type_>::operator [] (SizeType idx)
{
	return *datum(idx);
}

//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::SizeType VirtualVector<Type_>::capacity () const
{
	return m_capacity;
}

//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::SizeType VirtualVector<Type_>::size () const
{
	return m_size;
}

//----------------------------------------------------------------------

template <typename Type_>
inline bool VirtualVector<Type_>::empty () const
{
	return 0 == m_size;
}

//----------------------------------------------------------------------

template <typename Type_>
inline bool VirtualVector<Type_>::full () const
{
	return m_size >= m_capacity;
}

//----------------------------------------------------------------------

template <typename Type_>
inline void VirtualVector<Type_>::clear ()
{
	resize_to_n_elements (0);
}

//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::Iterator VirtualVector<Type_>::begin ()
{
	return data();
}

//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::ConstIterator VirtualVector<Type_>::begin () const
{
	return data();
}

//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::ConstIterator VirtualVector<Type_>::cbegin () const
{
	return data();
}

//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::Iterator VirtualVector<Type_>::end ()
{
	return data() + size();
}

//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::ConstIterator VirtualVector<Type_>::end () const
{
	return data() + size();
}

//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::ConstIterator VirtualVector<Type_>::cend () const
{
	return data() + size();
}

//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::ValueType const & VirtualVector<Type_>::front () const
{
	BKNT_ASSERT (!empty());
	return *datum(0);
}

//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::ValueType & VirtualVector<Type_>::front ()
{
	BKNT_ASSERT (!empty());
	return *datum(0);
}

//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::ValueType const & VirtualVector<Type_>::back () const
{
	BKNT_ASSERT (!empty());
	return *datum(size() - 1);
}

//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::ValueType & VirtualVector<Type_>::back ()
{
	BKNT_ASSERT (!empty());
	return *datum(size() - 1);
}

//----------------------------------------------------------------------

template <typename Type_>
inline bool VirtualVector<Type_>::push_back ()
{
	...
}

//----------------------------------------------------------------------

template <typename Type_>
inline bool VirtualVector<Type_>::push_back (ValueType const & value)
{
	...
}

//----------------------------------------------------------------------

template <typename Type_>
inline bool VirtualVector<Type_>::push_back (ValueType && value)
{
	...
}

//----------------------------------------------------------------------

template <typename Type_>
inline void VirtualVector<Type_>::pop_back ()
{
	...
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::ValueType const * VirtualVector<Type_>::data () const
{
	return reinterpret_cast<ValueType const *>(m_memory.memory());
}

//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::ValueType * VirtualVector<Type_>::data ()
{
	return reinterpret_cast<ValueType *>(m_memory.memory());
}

//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::ValueType const * VirtualVector<Type_>::datum (SizeType idx) const
{
	BKNT_ASSERT (idx < m_size, "VirtualVector index out of range: %u vs. %u.", unsigned(idx), unsigned(m_size));
	return data()[i];
}

//----------------------------------------------------------------------

template <typename Type_>
inline VirtualVector<Type_>::ValueType * VirtualVector<Type_>::datum (SizeType idx)
{
	BKNT_ASSERT (idx < m_size, "VirtualVector index out of range: %u vs. %u.", unsigned(idx), unsigned(m_size));
	return data()[i];
}

//----------------------------------------------------------------------

template <typename Type_>
inline void VirtualVector<Type_>::resize_to_n_elements (SizeType n)
{
	auto new_size = m_memory.resize (n * sizeof(ValueType));
	BKNT_ASSERT (n * sizeof(ValueType) <= new_size);
	BKNT_CONSUME_ARG (new_size);
	m_size = n;
}

//----------------------------------------------------------------------
//======================================================================

}	// namespace Blacknot

//======================================================================
