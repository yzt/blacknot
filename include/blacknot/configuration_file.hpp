//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================
#pragma once
//======================================================================

#include <blacknot/common.hpp>
#include <blacknot/parsers.hpp>

#include <cstdio>	// for FILE

//======================================================================
// section, entry, type, default value, format string, save_type
#define BKNT__DEF_CONFIG_ENTRIES(action)							\
	action (gfx,    fullscreen,     bool, false, "%d", int)			\
	action (gfx,    res_horizontal, int,  1024,  "%d", int)			\
	action (gfx,    res_vertical,   int,  768,   "%d", int)			\
	action (gfx,    refresh_rate,   int,  60,    "%d", int)			\
	action (game,   difficulty,     int,  0,     "%d", int)			\
	action (engine, max_memory_mb,  int,  128,   "%d", int)			\
	/**/

//======================================================================

namespace Blacknot {

//======================================================================
//----------------------------------------------------------------------

class ConfigFile
	: public ConfigParser
{
public:
	ConfigFile ();
	explicit ConfigFile (char const * file_name);
	virtual ~ConfigFile ();
	bool isOK () const {return m_ok;}

	bool save (char const * file_name) const;

	#define BKNT__DO_CONFIG_ENTRY_ACCSSRS(s,e,t,v,f,st)	t s##_##e () const {return m_##s##_##e;}
	BKNT__DEF_CONFIG_ENTRIES(BKNT__DO_CONFIG_ENTRY_ACCSSRS)
	#undef  BKNT__DO_CONFIG_ENTRY_ACCSSRS

private:
	virtual bool pop () override;
	virtual char curr () const override;
	virtual bool eoi () const override;
	virtual bool start () override;
	virtual bool end () override;
	virtual bool newSection (char const * section, unsigned size) override;
	virtual bool newEntry (char const * section, unsigned section_size, char const * name, unsigned name_size, char const * value, unsigned value_size) override;
	virtual bool error (Error err, int line, int column, int byte) override;
	
private:
	#define BKNT__DO_CONFIG_ENTRY_VARS(s,e,t,v,f,st)	t m_##s##_##e = v;
	BKNT__DEF_CONFIG_ENTRIES(BKNT__DO_CONFIG_ENTRY_VARS)
	#undef  BKNT__DO_CONFIG_ENTRY_VARS

	FILE * m_file = nullptr;
	bool m_ok = false;
	char m_curr = '\0';
	bool m_reached_eoi = false;
};

//======================================================================

}	// namespace Blacknot

//======================================================================
