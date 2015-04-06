//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================

#include <blacknot/configuration_file.hpp>

//======================================================================

namespace Blacknot {

//======================================================================

ConfigFile::ConfigFile (char const * file_name)
{
	m_file = fopen(file_name, "rb");
	m_ok = BKNT_PTR_VALID(m_file);

	if (m_ok)
		m_ok = parse ();
	else
		BKNT_REPORT ("[ERROR] File \"%s\" not found.\n", file_name);

	if (BKNT_PTR_VALID(m_file))
		fclose (m_file);
}

//----------------------------------------------------------------------

ConfigFile::~ConfigFile ()
{
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------

bool ConfigFile::pop ()
{
	if (!isOK())
		return false;

	auto c = fgetc (m_file);
	if (EOF == c)
	{
		m_reached_eoi = true;
		return false;
	}

	m_curr = char(c);
	return true;
}

//----------------------------------------------------------------------

char ConfigFile::curr () const
{
	return m_curr;
}

//----------------------------------------------------------------------

bool ConfigFile::eoi () const
{
	return m_reached_eoi;
}

//----------------------------------------------------------------------

bool ConfigFile::start ()
{
	return true;
}

//----------------------------------------------------------------------

bool ConfigFile::end ()
{
	return true;
}

//----------------------------------------------------------------------

bool ConfigFile::newSection (char const * section, unsigned size)
{
	BKNT_REPORT ("New section: (%u)%s.\n", size, section);
	return true;
}

//----------------------------------------------------------------------

bool ConfigFile::newEntry (char const * section, unsigned section_size, char const * name, unsigned name_size, char const * value, unsigned value_size)
{
	BKNT_REPORT ("New entry: (%u)%s, (%u)%s, (%u)%s.\n"
		, section_size, section, name_size, name, value_size, value
	);
	return true;
}

//----------------------------------------------------------------------

bool ConfigFile::error (Error err, int line, int column, int byte)
{
	BKNT_REPORT ("[ERROR] Parse error: %d, %d, %d, %d, %d\n", int(err), line, column, byte, int(curr()));
	return false;
}

//----------------------------------------------------------------------
//======================================================================

}	// namespace Blacknot

//======================================================================
