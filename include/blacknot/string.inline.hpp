//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================
#pragma once
//======================================================================
#if !defined(BKNT__STRING_DO_THE_IMPLEMENTATION)
	#error "This file is not for direct inclusion."
#endif
//======================================================================

#include <blacknot/string.hpp>

#include <blacknot/assert.hpp>
#include <blacknot/types.hpp>

//======================================================================

namespace Blacknot {

//======================================================================
//----------------------------------------------------------------------

template <U16 Capacity_>
inline CappedString<Capacity_>::CappedString ()
	: m_size (0)
{
	m_chars[0] = '\0';
}

//----------------------------------------------------------------------

template <U16 Capacity_>
inline CappedString<Capacity_>::CappedString (SizeType size, CharType fill)
	: m_size (Min(size, Capacity_))
{
	for (SizeType i = 0; i < m_size; ++i)
		m_chars[i] = fill;
	m_chars[m_size] = '\0';
}

//----------------------------------------------------------------------

template <U16 Capacity_>
inline CappedString<Capacity_>::CappedString (CharType const * cstr)
{
	SizeType i = 0;
	for ( ; i < Capacity_ && cstr[i] != '\0'; ++i)
		m_chars[i] = cstr[i];
	m_size = i;
	m_chars[i] = '\0';
}

//----------------------------------------------------------------------

template <U16 Capacity_>
template <U16 ThatCapacity>
inline CappedString<Capacity_>::CappedString (CappedString<ThatCapacity> const & that)
{
	SizeType i = 0;
	for ( ; i < Capacity_ && i < that.size(); ++i)
		m_chars[i] = that[i];
	m_size = i;
	m_chars[i] = '\0';
}

//----------------------------------------------------------------------

template <U16 Capacity_>
template <U16 ThatCapacity>
inline auto CappedString<Capacity_>::operator = (CappedString<ThatCapacity> const & that) -> MyType &
{
	SizeType i = 0;
	for ( ; i < Capacity_ && i < that.size(); ++i)
		m_chars[i] = that[i];
	m_size = i;
	m_chars[i] = '\0';

	return *this;
}

//----------------------------------------------------------------------

template <U16 Capacity_>
inline auto CappedString<Capacity_>::operator = (CharType const * cstr) -> MyType &
{
	SizeType i = 0;
	for ( ; i < Capacity_ && cstr[i] != '\0'; ++i)
		m_chars[i] = cstr[i];
	m_size = i;
	m_chars[i] = '\0';

	return *this;
}

//----------------------------------------------------------------------

template <U16 Capacity_>
template <U16 ThatCapacity>
inline bool CappedString<Capacity_>::operator == (CappedString<ThatCapacity> const & that)
{
	if (m_size != that.m_size)
		return false;

	SizeType i = 0;
	for ( ; i < m_size /*&& i < that.m_size*/; ++i)
		if (m_chars[i] != that.m_chars[i])
			return false;

	return true;
}

//----------------------------------------------------------------------

template <U16 Capacity_>
inline bool CappedString<Capacity_>::operator == (CharType const * cstr)
{
	SizeType i = 0;
	for ( ; i < m_size && cstr[i] != '\0'; ++i)
		if (m_chars[i] != cstr[i])
			return false;

	return i >= m_size && cstr[i] == '\0';
}

//----------------------------------------------------------------------

template <U16 Capacity_>
template <U16 ThatCapacity>
inline bool CappedString<Capacity_>::operator < (CappedString<ThatCapacity> const & that)
{
	SizeType i = 0;
	for ( ; i < m_size && i < that.size(); ++i)
		if (m_chars[i] != that[i])
			return m_chars[i] < that[i];

	return m_chars[i] < that[i];
}

//----------------------------------------------------------------------

template <U16 Capacity_>
inline bool CappedString<Capacity_>::operator < (CharType const * cstr)
{
	SizeType i = 0;
	for ( ; i < m_size && cstr[i] != '\0'; ++i)
		if (m_chars[i] != cstr[i])
			return m_chars[i] < cstr[i];

	return m_chars[i] < cstr[i];
}

//----------------------------------------------------------------------

template <U16 Capacity_>
inline auto CappedString<Capacity_>::operator [] (SizeType idx) const -> CharType
{
	BKNT_ASSERT (idx < m_size, "String index out of range: %d vs. %d.", int(idx), int(m_size));
	return m_chars[idx];
}

//----------------------------------------------------------------------

template <U16 Capacity_>
inline auto CappedString<Capacity_>::operator [] (SizeType idx) -> CharType &
{
	BKNT_ASSERT (idx < m_size, "String index out of range: %d vs. %d.", int(idx), int(m_size));
	return m_chars[idx];
}

//----------------------------------------------------------------------

template <U16 Capacity_>
inline auto CappedString<Capacity_>::operator += (CharType c) -> MyType &
{
	if (!full())
	{
		m_chars[m_size++] = c;
		m_chars[m_size] = '\0';
	}
	return *this;
}

//----------------------------------------------------------------------

template <U16 Capacity_>
inline auto CappedString<Capacity_>::operator += (CharType const * cstr) -> MyType &
{
	SizeType i = 0;
	for ( ; cstr[i] != '\0' && m_size + i < Capacity_; ++i)
		m_chars[m_size + i] = cstr[i];
	
	if (i > 0)
	{
		m_chars[m_size + i] = '\0';
		m_size += i;
	}

	return *this;
}

//----------------------------------------------------------------------

template <U16 Capacity_>
template <U16 ThatCapacity>
inline auto CappedString<Capacity_>::operator += (CappedString<ThatCapacity> const & that) -> MyType &
{
	SizeType i = 0;
	for ( ; i < that.size() && m_size + i < Capacity_; ++i)
		m_chars[m_size + i] = that[i];
	
	if (i > 0)
	{
		m_chars[m_size + i] = '\0';
		m_size += i;
	}

	return *this;
}

//----------------------------------------------------------------------

template <U16 Capacity_>
inline auto CappedString<Capacity_>::capacity () const -> SizeType
{
	return Capacity_;
}

//----------------------------------------------------------------------

template <U16 Capacity_>
inline auto CappedString<Capacity_>::size () const -> SizeType
{
	return m_size;
}

//----------------------------------------------------------------------

template <U16 Capacity_>
inline auto CappedString<Capacity_>::c_str () const -> CharType const *
{
	return m_chars;
}

//----------------------------------------------------------------------

template <U16 Capacity_>
inline bool CappedString<Capacity_>::empty () const
{
	return m_size == 0;
}

//----------------------------------------------------------------------

template <U16 Capacity_>
inline bool CappedString<Capacity_>::full () const
{
	return m_size >= Capacity_;
}

//----------------------------------------------------------------------

template <U16 Capacity_>
inline void CappedString<Capacity_>::clear ()
{
	m_size = 0;
	m_chars[0] = '\0';
}

//----------------------------------------------------------------------

template <U16 Capacity_>
inline auto CappedString<Capacity_>::begin () -> Iterator
{
	return m_chars;
}

//----------------------------------------------------------------------

template <U16 Capacity_>
inline auto CappedString<Capacity_>::begin () const -> ConstIterator
{
	return m_chars;
}

//----------------------------------------------------------------------

template <U16 Capacity_>
inline auto CappedString<Capacity_>::cbegin () const -> ConstIterator
{
	return m_chars;
}

//----------------------------------------------------------------------

template <U16 Capacity_>
inline auto CappedString<Capacity_>::end () -> Iterator
{
	return m_chars + m_size;
}

//----------------------------------------------------------------------

template <U16 Capacity_>
inline auto CappedString<Capacity_>::end () const -> ConstIterator
{
	return m_chars + m_size;
}

//----------------------------------------------------------------------

template <U16 Capacity_>
inline auto CappedString<Capacity_>::cend () const -> ConstIterator
{
	return m_chars + m_size;
}

//----------------------------------------------------------------------
//======================================================================

}	// namespace Blacknot

//======================================================================
