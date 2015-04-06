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

namespace Blacknot {

//======================================================================
//----------------------------------------------------------------------

class ConfigFile
	: public ConfigParser
{
public:
	explicit ConfigFile (char const * file_name);
	virtual ~ConfigFile ();
	bool isOK () const {return m_ok;}

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
	FILE * m_file = nullptr;
	bool m_ok = false;
	char m_curr = '\0';
	bool m_reached_eoi = false;
};

//======================================================================

}	// namespace Blacknot

//======================================================================
