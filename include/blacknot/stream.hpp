//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================
#pragma once
//======================================================================

#include <blacknot/common.hpp>

#include <cstdio>
#include <cstring>

//======================================================================

namespace Blacknot {
	namespace Stream {

//======================================================================
//----------------------------------------------------------------------
//----------------------------------------------------------------------

class Input
{
public:
	virtual ~Input () {}

	virtual bool error () const = 0;
	virtual bool eof () const = 0;
	virtual I64 read (Buffer buffer) = 0;
	//virtual I64 skip (I64 relative_offset) = 0;
	//virtual I64 seek (I64 absolute_offset) = 0;
	virtual I64 tell () const = 0;
};

//----------------------------------------------------------------------

class Output
{
public:
	virtual ~Output () {}

	virtual bool error () const = 0;
	virtual I64 write (Buffer buffer) = 0;
	//virtual I64 skip (I64 relative_offset) = 0;
	//virtual I64 seek (I64 absolute_offset) = 0;
	//virtual I64 tell () const = 0;
};

//======================================================================

class MemoryInput
	: public Input
{
public:
	inline explicit MemoryInput (Buffer buffer);
	inline virtual ~MemoryInput ();

	inline virtual bool error () const override;
	inline virtual bool eof () const override;
	inline virtual I64 read (Buffer buffer) override;
	inline virtual I64 tell () const override;

private:
	Buffer m_buffer;
	I64 m_position = 0;
};

//======================================================================

class FileInput
	: public Input
{
public:
	explicit FileInput (char const * file_path);
	virtual ~FileInput ();

	virtual bool error () const override;
	virtual bool eof () const override;
	virtual I64 read (Buffer buffer) override;
	virtual I64 tell () const override;

private:
	FILE * m_file = nullptr;
};

//======================================================================
//======================================================================
// Inline implementations:
//======================================================================

inline MemoryInput::MemoryInput (Buffer buffer)
	: m_buffer (buffer)
{
	BKNT_ASSERT (
		(m_buffer.data == nullptr && m_buffer.size == 0) ||
		(m_buffer.data != nullptr && m_buffer.size >= 0)
	);
}

//----------------------------------------------------------------------

inline MemoryInput::~MemoryInput ()
{
}

//----------------------------------------------------------------------

inline bool MemoryInput::error () const
{
	return !((m_buffer.data == nullptr && m_buffer.size == 0) || m_buffer.data != nullptr);
}

//----------------------------------------------------------------------

inline bool MemoryInput::eof () const
{
	return m_position >= m_buffer.size;
}

//----------------------------------------------------------------------

inline I64 MemoryInput::read (Buffer dest_buff)
{
	if (!dest_buff.valid())
		return 0;

	I64 b = Min(dest_buff.size, Max<I64>(0, m_buffer.size - m_position));
	::memcpy (dest_buff.data, m_buffer.data + m_position, b);
	m_position += b;

	return b;
}

//----------------------------------------------------------------------

inline I64 MemoryInput::tell () const
{
	return m_position;
}

//----------------------------------------------------------------------
//======================================================================

	}	// namespace Stream
}	// namespace Blacknot

//======================================================================
