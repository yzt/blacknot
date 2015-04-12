//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================

#include <blacknot/configuration_file.hpp>
#include <blacknot/string_util.hpp>

#include <cstring>

//======================================================================

namespace Blacknot {

//======================================================================

ConfigFile::ConfigFile ()
	: m_ok (true)
{
}

//----------------------------------------------------------------------

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

bool ConfigFile::save (char const * file_name) const
{
	FILE * file = fopen(file_name, "wb");
	if (!BKNT_PTR_VALID(file))
		return false;
	
	char curr_section [msc_MaxSectionNameSize + 1] = "";
	#define BKNT__DO_CONFIG_ENTRY_SAVE(s,e,t,v,f,st)							\
		if (0 != strcmp(curr_section, BKNT_STRINGIZE(s)))						\
		{																		\
			fprintf (file, "[" BKNT_STRINGIZE(s) "]\n");						\
			strncpy (curr_section, BKNT_STRINGIZE(s), msc_MaxSectionNameSize);	\
		}																		\
		fprintf (file, "  " BKNT_STRINGIZE(e) " = \"" f "\"\n", st(s##_##e()));
	BKNT__DEF_CONFIG_ENTRIES(BKNT__DO_CONFIG_ENTRY_SAVE)
	#undef  BKNT__DO_CONFIG_ENTRY_SAVE

	fclose (file);
	return true;
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

bool ConfigFile::newSection (char const * /*section*/, unsigned /*size*/)
{
	//BKNT_REPORT ("New section: (%u)%s.\n", size, section);
	return true;
}

//----------------------------------------------------------------------

bool ConfigFile::newEntry (
	char const * section, unsigned /*section_size*/,
	char const * name, unsigned /*name_size*/,
	char const * value, unsigned /*value_size*/
)
{
	#define BKNT__DO_CONFIG_ENTRY_LOAD(s,e,t,v,f,st)	else if (0 == strcmp(BKNT_STRINGIZE(s), section) && 0 == strcmp(BKNT_STRINGIZE(e), name)) m_##s##_##e = FromString<t>(value);
	if (false) {}
	BKNT__DEF_CONFIG_ENTRIES(BKNT__DO_CONFIG_ENTRY_LOAD)
	else BKNT_REPORT ("[WARNING] Unrecognized configuration entry: \"%s\":\"%s\"=\"%s\".\n", section, name, value);
	#undef  BKNT__DO_CONFIG_ENTRY_LOAD

	return true;
}

//----------------------------------------------------------------------

bool ConfigFile::error (Error err, int line, int column, int byte)
{
	BKNT_REPORT ("[ERROR] Parse error: err:%d, line:%d, col:%d, byte:%d, char:%d\n", int(err), line, column, byte, int(curr()));
	return false;
}

//----------------------------------------------------------------------
//======================================================================

}	// namespace Blacknot

//======================================================================
