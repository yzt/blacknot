//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================
#pragma once
//======================================================================

#include <blacknot/common.hpp>

//======================================================================

namespace Blacknot {

//======================================================================
/*
Configuration file format:

[section1]
name1 = "value1"
name2="value 2"
# comments start from the beginning of a line
[section2]
name3      ="  value 3"

*/
//----------------------------------------------------------------------

class ConfigParser
{
public:
	static unsigned const msc_MaxSectionNameSize = 64;
	static unsigned const msc_MaxNameSize = 64;
	static unsigned const msc_MaxValueSize = 256;

	enum class Error
	{
		None,
		IllegalChar,
		SizeViolation,
		EqualSignExpected,
		DoubleQuoteExpected,
	};

private:
	static bool IsWhiteSpace (char c);
	static bool IsNameStarter (char c);
	static bool IsNameContinuer (char c);

public:
	ConfigParser ();
	virtual ~ConfigParser ();

	bool parse ();

protected:
	virtual bool pop () = 0;	// Make next char in the stream the current char, return false on EOI. Is called before reading of the first char.
	virtual char curr () const = 0;	// Returns current character in the input stream
	virtual bool eoi () const = 0;	// Returns true iff there is no more input

	// If any of these returns false, the parsing is aborted
	virtual bool start () = 0;
	virtual bool end () = 0;
	virtual bool newSection (char const * section, unsigned size) = 0;
	virtual bool newEntry (
		char const * section, unsigned section_size,
		char const * name, unsigned name_size,
		char const * value, unsigned value_size
	) = 0;
	virtual bool error (Error err, int line, int column, int byte) = 0;

private:
	bool popChar ();
	void skipWS ();
	bool parseSectionHeader ();
	bool parseComment ();
	bool parseNameValuePair ();
	bool parseName (char * buffer, unsigned & size);
	bool parseValue (char * buffer, unsigned & size);

private:
	int m_line;
	int m_column;
	int m_byte;
	unsigned m_curr_section_size;
	char m_curr_section [msc_MaxSectionNameSize + 1];
};

//======================================================================

}	// namespace Blacknot

//======================================================================
