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
	

	DebuggingAllocator dbg_alloc {&root_allocator, 10000000};

	auto p = dbg_alloc.alloc (1000, BKNT_DBG_ALLOC_PARAMS(0));
	auto q = dbg_alloc.alloc (1300, BKNT_DBG_ALLOC_PARAMS(0));
	auto r = dbg_alloc.alloc ( 800, BKNT_DBG_ALLOC_PARAMS(0));

	dbg_alloc.free (q, 1300, BKNT_DBG_ALLOC_PARAMS(0));
	dbg_alloc.free (p, 1000, BKNT_DBG_ALLOC_PARAMS(0));
	dbg_alloc.free (r,  800, BKNT_DBG_ALLOC_PARAMS(0));

	return 0;
}

//======================================================================
