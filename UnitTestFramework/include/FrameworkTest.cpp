#include "include/FrameworkTest.hpp"

DefineTest(module, FrameWorkCheck) {
	expectTrue(1 + 1 == 2);
}

DefineTest(module, FalseCheck) {

	int* a = new int;
	delete a;

	expectTrue(true);
	expectTrue(false);
	expectTrue(1 + 1 == 2);

}

DefineTest(module, MemLeakCheck)
{
	int* a = new int;
	expectTrue(1 + 1 == 2);
}

DefineTest(module, AccessViolationCheck)
{
	expectTrue(true);

	int* a = nullptr;
	(*a)++;
}

DefineTest(module, TimeOutCheck)
{
	expectTrue(true);

	while (1)
	{

	}

	expectTrue(true);
}

