#include <iostream>
#include "Event.h"
#include "tests.h"

void all_tests()

{
	try
	{
		test_void_string_int();
		test_invoke_with_results();
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Test failed " << ex.what() << "\n";
	}
}


//////////////////////////////test 1 - testing invoke , += , -= 
void test_func_1_test1(const std::string& msg, int a)
{
	std::cout << "1" << msg << a << "\n";
}
void test_func_2_test1(const std::string& msg, int a)
{
	std::cout << "2" << msg << a << "\n";
}

void test_void_string_int()
{
	std::cout << "test 1 - test_void_string_int\n";


	gms::Event<void, const std::string&, int> ev;

	ev += ([](const std::string& s, int a) {std::cout << s << a << "lambd\n";});
	ev += (std::move(test_func_1_test1));
	ev += (&test_func_2_test1);

	ev -= (&test_func_2_test1);


	ev.invoke("test", 3);

	if (ev.get_functions_number() != 2) throw std::exception("test 1 failed");
	std::cout << "=================================\n\n";
}


//////////////////////////////test2 - testing invoke_with_results 
int test_func_1_test2(const std::string& msg, int a)
{
	std::cout << "1" << msg << a << "\n";
	return 1;
}
int test_func_2_test2(const std::string& msg, int a)
{
	std::cout << "2" << msg << a << "\n";
	return 2;
}

void test_invoke_with_results()
{
	std::cout << "test 2 - test_invoke_with_results\n";

	gms::Event<int, const std::string&, int> ev;

	ev += test_func_1_test2;
	ev += test_func_2_test2;

	auto res = ev.invoke_with_results("test2",2);

	if (res[0] != 1 || res[1] != 2) throw std::exception("test 2 failed");

	std::cout << "=================================\n\n";
}

