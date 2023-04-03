#include "CppUTest/TestHarness.h"

extern "C"
{
	#include "common.h"
}

TEST_GROUP(Common){
	void setup()
	{
	}

	void teardown()
    {
	}
};

TEST(Common, test_init_str_array)
{
	STR_ARRAY *sa = init_str_array();

	// "Newly initialized STR_ARRAY has len == 0", allocated == 0, strs == NULL
	CHECK_EQUAL(0, sa->len);
	CHECK_EQUAL(0, sa->allocated);
	CHECK_EQUAL(NULL, sa->strs);

	free_str_array(sa);
}

TEST(Common, test_append_str_array)
{
	STR_ARRAY *sa = init_str_array();
	
    char *s1 = (char*)"Uno";

	sa = append_str_array(sa, s1);

	CHECK_EQUAL(1, sa->len);
	CHECK_EQUAL(GROWTH_FACTOR, sa->allocated);
	STRCMP_EQUAL("Uno", sa->strs[0]);
	CHECK(s1 != sa->strs[0]); // make sure content is copied, not just reusing pointer
	CHECK_EQUAL(NULL, sa->strs[1]);

    char *s2 = (char*)"Dos";
	sa = append_str_array(sa, s2);

	CHECK_EQUAL(2, sa->len);
	CHECK_EQUAL(GROWTH_FACTOR, sa->allocated);
	STRCMP_EQUAL("Uno", sa->strs[0]);
	STRCMP_EQUAL("Dos", sa->strs[1]);
	CHECK_EQUAL(NULL, sa->strs[2]);

	for(int i = sa->len; i < 100; i++)
	{
		sa = append_str_array(sa, (char*)"XXX");
		
		CHECK(sa->len <= sa->allocated); // Allocated should always be more than len
	}

	free_str_array(sa);
	CHECK_EQUAL(0, sa->len);
	CHECK_EQUAL(0, sa->allocated);
	CHECK_EQUAL(NULL, sa->strs);
}

TEST(Common, test_find_str_array)
{
	STR_ARRAY *sa = init_str_array();

	int i_dos = find_str_array(sa, (char*)"Dos");
	CHECK_EQUAL(-1, i_dos);
	
	sa = append_str_array(sa, (char*)"Uno");
	sa = append_str_array(sa, (char*)"Dos");
	sa = append_str_array(sa, (char*)"Tres");

	i_dos = find_str_array(sa, (char*)"Dos");
	CHECK_EQUAL(1, i_dos);

	free_str_array(sa);
}

TEST(Common, test_remove_str_array)
{
	STR_ARRAY *sa = init_str_array();

	sa = append_str_array(sa, (char*)"Uno");
	sa = append_str_array(sa, (char*)"Dos");
	sa = append_str_array(sa, (char*)"Tres");

	int i_dos = find_str_array(sa, (char*)"Dos");
	CHECK_EQUAL(3, sa->len);
	CHECK_EQUAL(1, i_dos);

	remove_str_array(sa, 1);

	CHECK_EQUAL(2, sa->len);
	i_dos = find_str_array(sa, (char*)"Dos");
	CHECK_EQUAL(-1, i_dos);
	STRCMP_EQUAL("Tres", sa->strs[1]);

	free_str_array(sa);
}

TEST(Common, test_insert_str_array)
{
	STR_ARRAY *sa = init_str_array();

	sa = append_str_array(sa, (char*)"Uno");
	sa = append_str_array(sa, (char*)"Tres");
	char* tres = sa->strs[1];
	STRCMP_EQUAL("Tres", tres);

	int i_dos = find_str_array(sa, (char*)"Dos");
	CHECK_EQUAL(2, sa->len);
	CHECK_EQUAL(-1, i_dos);

	insert_str_array(sa, 1, (char*)"Dos");

	CHECK_EQUAL(3, sa->len);
	i_dos = find_str_array(sa, (char*)"Dos");
	CHECK_EQUAL(1, i_dos);
	CHECK_EQUAL(tres, sa->strs[2]);

	free_str_array(sa);
}
