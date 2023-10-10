#include "include/FHeapTest.hpp"

void SetTestMode(enum TestMode& inTestMode, int& inTestCase, enum PrintMode& inPrintMode)
{
	/*
		(1) Choose TestMode

		TestMode::Full				=>	Do UnitTest using full Cases (No Matter TestCase value)

		TestMode::Specific			=>	Test Specific Case (Select Case by Modifying TestCase value)

		TestMode::RunTestFunction	=>	Don't use Test Case, just call TestFunction()

		(1-*) If you chose TestMode::Specific in (1)  - Choose Specific Case Number

									inTestCase = 1	=> Do First Test Case
									inTestCase = 2	=> Do Second Test Case
									...

		(2) Choose PrintMode

		PrintMode::EveryCase		=>	Print Pass/Fail of Every Requirement of Specific Unit Test Case
		PrintMode::Summary			=>	Print Pass/Fail of Unit Test Case Only

	*/

	// (1) Choose TestMode
	inTestMode = TestMode::Full;

	// (1-*) Choose Specific Case Number
	inTestCase = 1;

	// (2) Choose PrintMode
	inPrintMode = PrintMode::Summary;
}

void TestFunction()
{
	/*
		  Do Whatever you want
	*/

	std::cout << "Hello, World" << std::endl;

}

/*
	================================================================================
	********** Do Not Modify Below **********
	================================================================================
*/

void SetFileAndTestName(std::string& ExeFileName, std::string& TestName, int& inTestTimeLimit)
{
	/*
		1) Set EXE_FILE_NAME as your project name
		2) Choose Test Mode by Modifying TestMode Variable
		3) Set Time Limit
	*/

	ExeFileName = "06-FibonacciHeap.exe";
	TestName = "Fibonacci Heap";
	inTestTimeLimit = 10000;
}

void RegisterTests()
{
	// Framework Check
	RegisterTest(module, FrameworkCheck);

	// FibonacciHeap Check
	RegisterTest(module, Construction);
	RegisterTest(module, Insertion);
	RegisterTest(module, SImpleExtraction);
	RegisterTest(module, RandomTest);


}

int _tmain(int argc, TCHAR* argv[])
{
	std::string EXE_FILE_NAME;
	std::string THIS_TEST_NAME;
	int TEST_TIMELIMIT = 10000;

	SetFileAndTestName(EXE_FILE_NAME, THIS_TEST_NAME, TEST_TIMELIMIT);

	UnitTest::getInstance().setExeName(EXE_FILE_NAME);
	UnitTest::getInstance().setTestName(THIS_TEST_NAME);
	UnitTest::getInstance().setTimeLimit(TEST_TIMELIMIT);

	enum TestMode TEST_MODE = TestMode::Full;
	int TEST_CASE = 0;
	enum PrintMode PRINT_MODE = PrintMode::Summary;


	SetTestMode(TEST_MODE, TEST_CASE, PRINT_MODE);
	UnitTest::getInstance().setPrintMode(PRINT_MODE);
	if (TEST_MODE == 2)
	{
		_CrtMemState sOld;
		_CrtMemState sNew;
		_CrtMemState sDiff;
		_CrtMemCheckpoint(&sOld);

		std::cout << std::endl;
		std::cout << "TestFunction() Start" << std::endl;
		std::cout << "------------------------------------------------" << std::endl;

		TestFunction();

		_CrtMemCheckpoint(&sNew);
		CheckMemoryState(&sDiff, &sOld, &sNew);
		std::cout << "================================================" << std::endl;
		return 0;
	}

	RegisterTests();

	if (TEST_MODE == 1)
	{
		if (TEST_CASE > 0 && TEST_CASE <= UnitTest::getInstance().getTestListSize())
		{
			std::string s = std::to_string(TEST_CASE);
			TCHAR* cz = StringToTCHAR(s);
			PrintTestCaseInfo(TEST_CASE);
			_CrtMemState sOld;
			_CrtMemState sNew;
			_CrtMemState sDiff;
			_CrtMemCheckpoint(&sOld);

			if (UnitTest::getInstance().runTest(cz)) {
				_CrtMemCheckpoint(&sNew);
				if (CheckMemoryState(&sDiff, &sOld, &sNew)) {
					PrintTestCaseResult(TEST_CASE, false);
				}
				else {
					PrintTestCaseResult(TEST_CASE, true);
				}
			}
			else {
				_CrtMemCheckpoint(&sNew);
				CheckMemoryState(&sDiff, &sOld, &sNew);
				PrintTestCaseResult(TEST_CASE, false);
			}
		}
		else
		{
			std::cout << "Wrong TestCase Number, Check TestCase Variable in main.cpp" << std::endl;
			std::cout << std::endl;
			return 0;
		}
	}
	else if (TEST_MODE == 0)
	{
		if (argc < 2) {
			RunTests();
		}
		else if (argc == 2) {
			_CrtMemState sOld;
			_CrtMemState sNew;
			_CrtMemState sDiff;
			_CrtMemCheckpoint(&sOld);

			if (UnitTest::getInstance().runTest(argv[1])) {
				_CrtMemCheckpoint(&sNew);
				if (CheckMemoryState(&sDiff, &sOld, &sNew))
					return 0;
				else
					return 1;
			}
			else {
				_CrtMemCheckpoint(&sNew);
				CheckMemoryState(&sDiff, &sOld, &sNew);
				return 0;
			}
		}
		else {
			std::cout << "Arg is wrong" << std::endl;
			return 0;
		}
	}
	else
	{

	}

	return 0;
}