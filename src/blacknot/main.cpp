//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================

#include <blacknot/allocator.hpp>
#include <blacknot/configuration_file.hpp>
#include <blacknot/engine.hpp>

//======================================================================
//======================================================================
//----------------------------------------------------------------------
//======================================================================

int main (int /*argc*/, char * /*argv*/ [], char * /*envp*/ [])
{
	using namespace Blacknot;

	/* Load config file and save a default version and a current version */
	char const config_file_name [] = "blacknot.cfg";

	ConfigFile cfg (config_file_name);
	cfg.save ("blacknot_current.cfg");
	ConfigFile().save ("blacknot_default.cfg");
	if (cfg.isOK() == false)
		BKNT_REPORT ("[ERROR] Can't load configuration file (%s).\n", config_file_name);

	/* Make the root allocator */
	RootAllocator root_allocator (U64(cfg.engine_max_memory_mb()) * 1048576);

	Engine engine (cfg, &root_allocator);


	return 0;
}

//======================================================================
