//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================
#pragma once
//======================================================================

#include <blacknot/config.hpp>
#include <blacknot/types.hpp>

//======================================================================

namespace Blacknot {

//======================================================================
//----------------------------------------------------------------------

template <U16 Capacity_>
class CappedString
{
public:
	using MyType = CappedString<Capacity_>;
	using SizeType = U16;
	using CharType = char;
	static SizeType const Capacity = Capacity_;

public:
	inline CappedString ();
	inline CappedString (SizeType size, CharType fill);
	explicit inline CappedString (CharType const * cstr);
	//inline CappedString (MyType const & that);
	template <U16 ThatCapacity>
	inline CappedString (CappedString<ThatCapacity> const & that);

	template <U16 ThatCapacity>
	inline CappedString & operator = (CappedString<ThatCapacity> const & that);
	inline CappedString & operator = (CharType const * cstr);

	template <U16 ThatCapacity>
	inline bool operator == (CappedString<ThatCapacity> const & that);
	inline bool operator == (CharType const * cstr);
	template <U16 ThatCapacity>
	inline bool operator < (CappedString<ThatCapacity> const & that);
	inline bool operator < (CharType const * cstr);
	// TODO: Other comparison operators

	inline CharType operator [] (SizeType idx) const;
	inline CharType & operator [] (SizeType idx);

	inline MyType & operator += (CharType c);
	inline MyType & operator += (CharType const * cstr);
	template <U16 ThatCapacity>
	inline MyType & operator += (CappedString<ThatCapacity> const & that);

	inline SizeType capacity () const;
	inline SizeType size () const;
	inline CharType const * c_str () const;
	inline bool empty () const;
	inline bool full () const;
	
private:
	SizeType m_size = 0;
	CharType m_chars [Capacity + 1];
};

//======================================================================

}	// namespace Blacknot

//======================================================================

#define BKNT__STRING_DO_THE_IMPLEMENTATION
#include <blacknot/string.inline.hpp>
#undef  BKNT__STRING_DO_THE_IMPLEMENTATION
//======================================================================
