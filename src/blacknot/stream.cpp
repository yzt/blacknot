//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================

#include <blacknot/stream.hpp>

//======================================================================

namespace Blacknot {
	namespace Stream {

//======================================================================
//----------------------------------------------------------------------
//======================================================================

FileInput::FileInput (char const * file_path)
	: m_file (::fopen(file_path, "rb"))
{
}

//----------------------------------------------------------------------

FileInput::~FileInput ()
{
	::fclose (m_file);
	m_file = nullptr;
}

//----------------------------------------------------------------------

bool FileInput::error () const
{
	return !BKNT_PTR_VALID(m_file) || 0 != ::ferror(m_file);
}

//----------------------------------------------------------------------

bool FileInput::eof () const
{
	return !BKNT_PTR_VALID(m_file) || 0 != ::feof(m_file);
}

//----------------------------------------------------------------------

I64 FileInput::read (Buffer dest_buff)
{
	if (!BKNT_PTR_VALID(m_file) || !dest_buff.valid())
		return 0;

	size_t r = fread (dest_buff.data, 1, dest_buff.size, m_file);

	return I64(r);
}

//----------------------------------------------------------------------

I64 FileInput::tell () const
{
	return BKNT_PTR_VALID(m_file) ? ::ftell(m_file) : 0;
}

//----------------------------------------------------------------------
//======================================================================

	}	// namespace Stream
}	// namespace Blacknot

//======================================================================
