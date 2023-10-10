#include "include/FrameworkTest.hpp"

DefineTest(module, test1) {
	expectTrue(1 + 1 == 2);
}

DefineTest(module, test2) {

	int* a = new int;
	delete a;

	expectTrue(true);
	expectTrue(false);
	expectTrue(1 + 1 == 2);

}

DefineTest(module, test3)
{
	int* a = new int;
	expectTrue(1 + 1 == 2);
}