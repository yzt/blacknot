//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================

#include <blacknot/allocator.hpp>
#include <blacknot/configuration_file.hpp>
#include <blacknot/engine.hpp>

//======================================================================

void TestAllocator (Blacknot::Allocator * alctr);

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

	//DebuggingAllocator dbg_allocator {&root_allocator, 10000000};
	//TestAllocator (&dbg_allocator);

	BKNT_REPORT ("%d, %d, %d\n"
		, int(Blacknot::In(0, 42, 0.0, 13))
		, int(Blacknot::In(1, 42, 0.0, 13, 2, 3, 4, 5, 6, 7, 7, 7, 7))
		, int(Blacknot::In(1, 42, 0.0, 0 == 0))
	);

	return 0;
}

//======================================================================

void TestAllocator (Blacknot::Allocator * alctr)
{
	struct TestType
	{
		TestType (int & foo, double bar) {BKNT_REPORT("TestType ctor for %d, %f...\n", foo, bar);}
		TestType (TestType const &) {BKNT_REPORT("TestType copy-ctor...\n");}
		~TestType () {BKNT_REPORT("TestType dtor.\n");}
	};
//*
	struct ExperimentalType
	{
		char a;		// 0-1		 0-1
		int b;		// 1-5		 4-8
		char c;		// 5-6		 8-9
		short d;	// 6-8		10-12
		char e;		// 8-9		12-13
		int f;		// 9-13		16-20
		char g;		// 13-14	20-21
		double h;	// 14-22	24-32
	};
/*/
	struct ExperimentalType
	{
		double h;
		int b;
		int f;
		short d;
		char a;
		char c;
		char e;
		char g;
	};
//*/
	BKNT_REPORT ("%u, %u\n", unsigned(sizeof(TestType)), unsigned(sizeof(ExperimentalType)));

	int foo1 = 42, foo2 = 105;
	auto tt = alctr->create<TestType>(BKNT_DBGP(0), foo1, -0.5);
	alctr->destroy (tt, BKNT_DBGP(0));

	TestType ss {foo2, 3.14};

	auto ss_a = alctr->createArray<TestType>(10, BKNT_DBGP(0), ss);
	alctr->destroyArray(ss_a, 10, BKNT_DBGP(0));
	//alctr->destroy(ss_a, BKNT_DBG_ALLOC_PARAMS(0));

	auto p = alctr->alloc (1000, BKNT_DBGP(0));
	auto q = alctr->alloc (1300, BKNT_DBGP(0));
	auto r = alctr->alloc ( 800, BKNT_DBGP(0));

	alctr->free (q, 1300, BKNT_DBGP(0));
	alctr->free (p, 1000, BKNT_DBGP(0));
	alctr->free (r,  800, BKNT_DBGP(0));
}

//======================================================================
