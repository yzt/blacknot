//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================

#include <blacknot/common.hpp>
#include <blacknot/configuration_file.hpp>

#include <blacknot/string.hpp>

//======================================================================
//======================================================================
//----------------------------------------------------------------------
//======================================================================

int main (int /*argc*/, char * /*argv*/ [], char * /*envp*/ [])
{
	Blacknot::ConfigFile cfg ("config_test.txt");

	BKNT_REPORT ("  fullscreen: %s\n", cfg.gfx_fullscreen() ? "yes" : "no");
	BKNT_REPORT ("        size: %dx%d\n", cfg.gfx_res_horizontal(), cfg.gfx_res_vertical());
	BKNT_REPORT ("refresh rate: %d\n", cfg.gfx_refresh_rate());
	BKNT_REPORT ("  difficulty: %d\n", cfg.game_difficulty());

	cfg.save ("blacknot_current.cfg");
	Blacknot::ConfigFile().save ("blacknot_default.cfg");

	Blacknot::CappedString<7> s1;
	Blacknot::CappedString<7> s2 (12, '*');
	Blacknot::CappedString<7> s3 ("Hello...");
	Blacknot::CappedString<12> s4 (s2);

	s4 += "!!";
	s4 += s1;
	s4 += 'y';
	s4 += s4;

	BKNT_REPORT ("-%s-%s-%s-%s-\n", s1.c_str(), s2.c_str(), s3.c_str(), s4.c_str());

	return 0;
}

//======================================================================
