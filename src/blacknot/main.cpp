//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================

#include <blacknot/common.hpp>
#include <blacknot/configuration_file.hpp>

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

	return 0;
}

//======================================================================
