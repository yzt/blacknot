//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================

#include <blacknot/common.hpp>
#include <blacknot/configuration_file.hpp>

#include <blacknot/string.hpp>
#include <blacknot/array.hpp>

//======================================================================

struct TestClass
{
	TestClass () {BKNT_REPORT ("[TestClass default ctor...]\n");}
	~TestClass () {BKNT_REPORT ("[TestClass dtor.]\n");}

	int x = -42;
	double p = 13.2;
};

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
	s4[7] = '^';
	for (auto c : s4)
		BKNT_REPORT ("%c\n", c);

	BKNT_REPORT ("--------------------\n");

	Blacknot::CappedArray<TestClass, 5> a (2);
	BKNT_STATIC_ASSERT_SIZE (a, 88);

	a.pop_back();
	a.push_back();
	a.push_back();
	a.push_back();
	a.push_back();
	a.push_back();

	int b [] = {1, 1, 2, 3, 5, 8, 13};
	Blacknot::CappedArray<int, 10> c (b, b + 7);

	for (Blacknot::U16 i = 0; i < c.size(); ++i)
		BKNT_REPORT (" %d", c[i]);
	BKNT_REPORT ("\n");

	for (auto x : c)
		BKNT_REPORT (" %d", x);
	BKNT_REPORT ("\n");

	return 0;
}

//======================================================================
